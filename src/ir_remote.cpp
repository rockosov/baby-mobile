/**
 * ir_remote.cpp: Routines for IR Remote handling
 */

#include "ir_remote.h"
#include "baby_mobile_pins.h"

decode_results ir_remote_cur_cmd;
decode_results ir_remote_lst_cmd;
IRrecv         ir_remote(IR_REMOTE_PIN);

void ir_remote_dump(HardwareSerial* serial, decode_results* command) {
    serial->print("IR_REMOTE: ");
    switch (command->value) {
        case IR_REMOTE_POWER:
            serial->println("IR_REMOTE_POWER");
            break;

        case IR_REMOTE_BGM:
            serial->println("IR_REMOTE_BGM");
            break;

        case IR_REMOTE_MUTE:
            serial->println("IR_REMOTE_MUTE");
            break;

        case IR_REMOTE_MENU:
            serial->println("IR_REMOTE_MENU");
            break;

        case IR_REMOTE_UP:
            serial->println("IR_REMOTE_UP");
            break;

        case IR_REMOTE_RETURN:
            serial->println("IR_REMOTE_RETURN");
            break;

        case IR_REMOTE_LEFT:
            serial->println("IR_REMOTE_LEFT");
            break;

        case IR_REMOTE_OK:
            serial->println("IR_REMOTE_OK");
            break;

        case IR_REMOTE_RIGHT:
            serial->println("IR_REMOTE_RIGHT");
            break;

        case IR_REMOTE_SETUP:
            serial->println("IR_REMOTE_SETUP");
            break;

        case IR_REMOTE_DOWN:
            serial->println("IR_REMOTE_DOWN");
            break;

        case IR_REMOTE_PLAY_PAUSE:
            serial->println("IR_REMOTE_PLAY_PAUSE");
            break;

        case IR_REMOTE_VOL_MINUS:
            serial->println("IR_REMOTE_VOL_MINUS");
            break;

        case IR_REMOTE_VOL_PLUS:
            serial->println("IR_REMOTE_VOL_PLUS");
            break;

        case IR_REMOTE_ZOOM:
            serial->println("IR_REMOTE_ZOOM");
            break;

        case IR_REMOTE_PREV:
            serial->println("IR_REMOTE_PREV");
            break;

        case IR_REMOTE_NEXT:
            serial->println("IR_REMOTE_NEXT");
            break;

        case IR_REMOTE_ROTATE:
            serial->println("IR_REMOTE_ROTATE");
            break;

        default:
            serial->print("UNKNOWN(");
            serial->print(command->value, HEX);
            serial->println(")");
            break;
    }
}

void ir_remote_setup(void) {
    /* Start IR receiver */
    ir_remote.enableIRIn();

    return;
}

decode_results* ir_remote_recv(void) {
    if (ir_remote.decode(&ir_remote_cur_cmd)) {
        if (ir_remote_cur_cmd.value == IR_REMOTE_REPEAT) {
            ir_remote_cur_cmd = ir_remote_lst_cmd;
        }
        ir_remote_lst_cmd = ir_remote_cur_cmd;
        ir_remote.resume();

        return &ir_remote_cur_cmd;
    }

    return NULL;
}

