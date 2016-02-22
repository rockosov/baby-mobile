#include <Arduino.h>

#include "motor.h"
#include "baby_mobile_pins.h"

static uint8_t current_speed = MOTOR_DEFAULT_SPEED;
static bool is_switch_on = false;

void motor_setup(void) {
    pinMode(MOTOR_DIRECTION_PIN, OUTPUT);
    pinMode(MOTOR_SPEED_PIN, OUTPUT);

    return;
}

void motor_increment_speed(void) {
    if (is_switch_on) {
        Serial.print("speed ");
        Serial.print(current_speed);

        current_speed += (current_speed != MOTOR_MAX_SPEED) ? 1 : 0;

        Serial.print(" => speed ");
        Serial.println(current_speed);

        analogWrite(MOTOR_SPEED_PIN, current_speed);
    }

    return;
}

void motor_decrement_speed(void) {
    if (is_switch_on) {
        Serial.print("speed ");
        Serial.print(current_speed);

        current_speed -= (current_speed != MOTOR_MIN_SPEED) ? 1 : 0;

        Serial.print(" => speed ");
        Serial.println(current_speed);

        analogWrite(MOTOR_SPEED_PIN, current_speed);
    }

    return;
}

void motor_change_direction(uint8_t direction) {
    uint8_t current_direction = 0;

    if (is_switch_on) {
        current_direction = digitalRead(MOTOR_DIRECTION_PIN);

        if (current_direction != direction) {
            digitalWrite(MOTOR_DIRECTION_PIN, direction);
        }
    }

    return;
}

void motor_switch_on_off(void) {
    current_speed = is_switch_on ? MOTOR_DEFAULT_SPEED : MOTOR_MIN_SPEED;
    analogWrite(MOTOR_SPEED_PIN, current_speed);
    is_switch_on ^= true;

    return;
}

