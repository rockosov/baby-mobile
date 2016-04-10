#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>

#include "df_mini_player.h"
#include "baby_mobile_pins.h"

static SoftwareSerial df_mini_player_serial(DF_MINI_PLAYER_RX_PIN,
                                            DF_MINI_PLAYER_TX_PIN);

static uint16_t current_volume = DF_MINI_PLAYER_DEFAULT_VOLUME;
static volatile bool is_free = false;
static uint8_t current_folder = 0x0;

extern void mp3_send_cmd (uint8_t cmd, uint16_t arg);
extern void mp3_send_cmd (uint8_t cmd);

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

extern uint8_t send_buf[10];

void df_mini_player_print_hex (uint8_t *buf, uint8_t len) {
    int i = 0;

    for(i = 0; i < len; ++i) {
        Serial.print(buf[i], HEX);
        Serial.print(":");
    }

    Serial.println ();
}

uint8_t df_mini_player_serial_read(SoftwareSerial serial, uint8_t *buf,
                                   uint8_t len, uint8_t timeout) {
    uint32_t start_time = 0;
    uint8_t  recv_bytes = 0;

    if (serial.available()) {
        for (recv_bytes = 0; recv_bytes < len; ++recv_bytes) {
            start_time = millis ();

            while (!serial.available()) {
                if (millis() - start_time > timeout) {
                    return recv_bytes;
                }
            }

            buf[recv_bytes] = serial.read();
        }
    }
    return recv_bytes;
}

void df_mini_player_print_info(void) {
    uint8_t recv_bytes = 0;

    Serial.println ("You sent a request:");
    df_mini_player_print_hex(send_buf, 10);
    delay(DF_MINI_PLAYER_SENT_CMD_DELAY);
    recv_bytes = df_mini_player_serial_read(df_mini_player_serial, recv_buf, 10, 3);
    if (recv_bytes != 0) {
        Serial.println ("You got a response:");
        df_mini_player_print_hex(recv_buf, recv_bytes);
    }
}

void df_mini_player_next_folder(void) {
    /* Valid folder numbers are in range [1:DF_MINI_PLAYER_FOLDER_NUM] */
    current_folder = (current_folder % DF_MINI_PLAYER_FOLDER_NUM) + 1;

    Serial.print("Specify folder number ");
    Serial.println(current_folder);

    /* Play 01.mp3 in the current folder */

    /* Detach BUSY interrupt to do not trigger handler */
    detachInterrupt(DF_MINI_PLAYER_BUSY_INT);
    /* Send the command with folder number */
    mp3_send_cmd(0x0F, current_folder << 8);
    df_mini_player_print_info();
    /* Wait DF_MINI_PLAYER_SET_FOLDER_DELAY */
    delay(DF_MINI_PLAYER_SET_FOLDER_DELAY);
    /* Clear all interrupts from BUSY pin and attach it again */
    EIFR |= 1 << DF_MINI_PLAYER_BUSY_INT;
    attachInterrupt(DF_MINI_PLAYER_BUSY_INT,
                    df_mini_player_busy_rising_handler,
                    RISING);
}

