/**
 * baby_mobile.ino: Baby mobile main file
 */

#include "baby_mobile.h"

void setup(void) {
    /* Start serial port */
    Serial.begin(SERIAL_SPEED);

    /* Setup DFMiniPlayer */
    df_mini_player_setup();

    /* Setup IR remote */
    ir_remote_setup();

    /* Setup Motor */
    motor_setup();

    return;
}

void loop(void) {
    decode_results* ir_remote_command = NULL;

    ir_remote_command = ir_remote_recv();
    if (ir_remote_command != NULL) {
        ir_remote_dump(&Serial, ir_remote_command);
    }

    switch (ir_remote_command->value) {
        case IR_REMOTE_PLAY_PAUSE:
            df_mini_player_play_pause();
            break;

        case IR_REMOTE_VOL_PLUS:
            df_mini_player_increase_volume();
            break;

        case IR_REMOTE_VOL_MINUS:
            df_mini_player_decrease_volume();
            break;

        case IR_REMOTE_NEXT:
            df_mini_player_next();
            break;

        case IR_REMOTE_PREV:
            df_mini_player_prev();
            break;

        case IR_REMOTE_UP:
            motor_increment_speed();
            break;

        case IR_REMOTE_DOWN:
            motor_decrement_speed();
            break;

        case IR_REMOTE_LEFT:
            motor_change_direction(MOTOR_DIRECTION_ANTICLOCKWISE);
            break;

        case IR_REMOTE_RIGHT:
            motor_change_direction(MOTOR_DIRECTION_CLOCKWISE);
            break;

        default:
            break;
    }

    return;
}

