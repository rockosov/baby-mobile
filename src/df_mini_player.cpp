#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>

#include "df_mini_player.h"
#include "baby_mobile_pins.h"

static SoftwareSerial df_mini_player_serial(DF_MINI_PLAYER_RX_PIN,
                                            DF_MINI_PLAYER_TX_PIN);

static uint16_t current_volume = DF_MINI_PLAYER_DEFAULT_VOLUME;

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

    return;
}

void df_mini_player_play_pause(void) {
    mp3_play(1);

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

