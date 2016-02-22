#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>

#include "df_mini_player.h"
#include "baby_mobile_pins.h"

static SoftwareSerial df_mini_player_serial(DF_MINI_PLAYER_RX_PIN,
                                            DF_MINI_PLAYER_TX_PIN);

static uint16_t current_volume = DF_MINI_PLAYER_DEFAULT_VOLUME;
static volatile bool is_free = false;

static void df_mini_player_busy_rising_handler(void) {
    is_free = true;
}

void df_mini_player_setup(void) {
    /* Start DFMiniPlayer Serial port */
    df_mini_player_serial.begin(DF_MINI_PLAYER_SERIAL_SPEED);

    /* Add some delay to give a time for DFMiniPlayer Device Initialization */
    delay(DF_MINI_PLAYER_INIT_DELAY);

    /* Connect DFMiniPlayer Serial port with library */
    mp3_set_serial(df_mini_player_serial);

    /* Set default DFMiniPlayer volume */
    mp3_set_volume(current_volume);

    /* Add some delay to give a time for DFMiniPlayer Device Set Volume */
    delay(DF_MINI_PLAYER_SET_VOLUME_DELAY);

    /* Change BUSY pin to INPUT mode */
    pinMode(DF_MINI_PLAYER_BUSY_PIN, INPUT);

    attachInterrupt(DF_MINI_PLAYER_BUSY_INT,
                    df_mini_player_busy_rising_handler,
                    RISING);

    return;
}

void df_mini_player_play_pause(void) {
    bool is_df_mini_player_busy = false;

    /**
     * Low means playing, High means no.
     * Datasheet, page 2, Table 2.2 Pin Description.
     */
    is_df_mini_player_busy = !digitalRead(DF_MINI_PLAYER_BUSY_PIN);

    Serial.print("DFMiniPlayer BUSY pin: ");
    Serial.println(is_df_mini_player_busy);
    if (is_df_mini_player_busy) {
        Serial.println("Send PAUSE to DFMiniPlayer");
        detachInterrupt(DF_MINI_PLAYER_BUSY_INT);
        mp3_pause();
    } else {
        Serial.println("Send PLAY to DFMiniPlayer");
        /* Clear interrupt for DFMiniPlayer int */
        EIFR |= 1 << DF_MINI_PLAYER_BUSY_INT;
        attachInterrupt(DF_MINI_PLAYER_BUSY_INT,
                        df_mini_player_busy_rising_handler,
                        RISING);
        mp3_play();
    }

    return;
}

void df_mini_player_decrease_volume(void) {
    current_volume -= (current_volume != DF_MINI_PLAYER_MIN_VOLUME) ? 1 : 0;

    mp3_set_volume(current_volume);
}

void df_mini_player_increase_volume(void) {
    current_volume += (current_volume != DF_MINI_PLAYER_MAX_VOLUME) ? 1 : 0;

    mp3_set_volume(current_volume);
}

void df_mini_player_next(void) {
    mp3_next();
}

void df_mini_player_prev(void) {
    mp3_prev();
}

void df_mini_player_cycle_check(void) {
    if (is_free) {
        Serial.print("Detect the end of song, play next");
        mp3_next();
        is_free = false;
    }
}

