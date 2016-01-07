#ifndef __IR_REMOTE_H__
#define __IR_REMOTE_H__

#include <IRremote.h>

#define IR_REMOTE_POWER      0x00FFB04F
#define IR_REMOTE_BGM        0x00FF708F
#define IR_REMOTE_MUTE       0x00FFF00F
#define IR_REMOTE_MENU       0x00FF906F
#define IR_REMOTE_UP         0x00FF50AF
#define IR_REMOTE_RETURN     0x00FFD02F
#define IR_REMOTE_LEFT       0x00FF8877
#define IR_REMOTE_OK         0x00FF48B7
#define IR_REMOTE_RIGHT      0x00FFC837
#define IR_REMOTE_SETUP      0x00FFA857
#define IR_REMOTE_DOWN       0x00FF6897
#define IR_REMOTE_PLAY_PAUSE 0x00FFE817
#define IR_REMOTE_VOL_MINUS  0x00FF9867
#define IR_REMOTE_VOL_PLUS   0x00FF58A7
#define IR_REMOTE_ZOOM       0x00FFD827
#define IR_REMOTE_PREV       0x00FFB847
#define IR_REMOTE_NEXT       0x00FF7887
#define IR_REMOTE_ROTATE     0x00FFF807
#define IR_REMOTE_REPEAT     0xFFFFFFFF

void ir_remote_dump(HardwareSerial* serial, decode_results* command);

#endif /* !__IR_REMOTE_H__ */

