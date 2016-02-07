#include <Arduino.h>

#include "motor.h"
#include "baby_mobile_pins.h"

static uint8_t current_speed = MOTOR_DEFAULT_SPEED;

void motor_setup(void) {
    pinMode(MOTOR_DIRECTION_PIN, OUTPUT);
    pinMode(MOTOR_SPEED_PIN, OUTPUT);

    return;
}

void motor_increment_speed(void) {
    Serial.print("speed ");
    Serial.print(current_speed);

    current_speed += (current_speed != MOTOR_MAX_SPEED) ? 1 : 0;

    Serial.print(" => speed ");
    Serial.println(current_speed);

    analogWrite(MOTOR_SPEED_PIN, current_speed);

    return;
}

void motor_decrement_speed(void) {
    Serial.print("speed ");
    Serial.print(current_speed);

    current_speed -= (current_speed != MOTOR_MIN_SPEED) ? 1 : 0;

    Serial.print(" => speed ");
    Serial.println(current_speed);

    analogWrite(MOTOR_SPEED_PIN, current_speed);

    return;
}

void motor_change_direction(uint8_t direction) {
    uint8_t current_direction = 0;

    current_direction = digitalRead(MOTOR_DIRECTION_PIN);

    if (current_direction != direction) {
        digitalWrite(MOTOR_DIRECTION_PIN, direction);
    }

    return;
}

