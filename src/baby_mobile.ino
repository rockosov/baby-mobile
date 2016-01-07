/**
 * baby_mobile.ino: Baby mobile main file
 */

#include "baby_mobile.h"

static decode_results ir_remote_cur_cmd;
static decode_results ir_remote_lst_cmd;
static IRrecv         ir_remote(IR_REMOTE_PIN);

void setup(void) {
    /* Start serial port */
    Serial.begin(SERIAL_SPEED);

    /* Start IR receiver */
    ir_remote.enableIRIn();
}

void loop(void) {
    if (ir_remote.decode(&ir_remote_cur_cmd)) {
        if (ir_remote_cur_cmd.value == IR_REMOTE_REPEAT) {
            ir_remote_cur_cmd = ir_remote_lst_cmd;
        }
        ir_remote_lst_cmd = ir_remote_cur_cmd;
        ir_remote_dump(&Serial, &ir_remote_cur_cmd);
        ir_remote.resume();
    }
}

