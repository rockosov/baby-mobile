#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_MIN_SPEED               0
#define MOTOR_MAX_SPEED               255
#define MOTOR_DIRECTION_CLOCKWISE     HIGH
#define MOTOR_DIRECTION_ANTICLOCKWISE LOW
#define MOTOR_DEFAULT_SPEED           0

void motor_setup(void);
void motor_increment_speed(void);
void motor_decrement_speed(void);
void motor_change_direction(uint8_t direction);

#endif /* !__MOTOR_H__ */

