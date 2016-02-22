#ifndef __DF_MINI_PLAYER_H__
#define __DF_MINI_PLAYER_H__

#define DF_MINI_PLAYER_SERIAL_SPEED      9600
#define DF_MINI_PLAYER_INIT_DELAY        5000
#define DF_MINI_PLAYER_SET_VOLUME_DELAY  1000
#define DF_MINI_PLAYER_DEFAULT_VOLUME    10
#define DF_MINI_PLAYER_MIN_VOLUME        0
#define DF_MINI_PLAYER_MAX_VOLUME        30
#define DF_MINI_PLAYER_CMD_PLAYBACK_MODE 0x08
#define DF_MINI_PLAYER_PARAM_FLDR_REPEAT 0x01

void df_mini_player_setup(void);
void df_mini_player_play_pause(void);
void df_mini_player_increase_volume(void);
void df_mini_player_decrease_volume(void);
void df_mini_player_next(void);
void df_mini_player_prev(void);
void df_mini_player_cycle_check(void);

#endif /* !__DF_MINI_PLAYER_H__ */

