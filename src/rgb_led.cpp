#include "rgb_led.h"
#include "baby_mobile_pins.h"

#include <Arduino.h>
#include <Timer.h>

static rgb_led_mode_t current_mode = RGB_LED_OFF;
static Timer step_timer;
static int8_t step_event_id = 0;
static uint32_t step_timer_tick = RGB_LED_DEFAULT_TICK;
static uint32_t current_hue = RGB_LED_DEFAULT_HUE;
static uint32_t current_saturation = RGB_LED_DEFAULT_SATURATION;
static uint32_t current_brightness = RGB_LED_DEFAULT_BRIGHTNESS;
static bool is_color_changed_in_freeze = false;

/**
 * hue ∈ [0°, 360°)
 * saturation ∈ [0, 100]
 * brightness ∈ [0, 100]
 */
static void rgb_led_hsb_to_rgb(uint32_t hue,
                               uint32_t saturation,
                               uint32_t brightness,
                               uint8_t rgb[]) {
    double red = 0;
    double green = 0;
    double blue = 0;
    double h = 0;
    double s = 0;
    double b = 0;
    double f = 0;
    double p = 0;
    double q = 0;
    double t = 0;

    hue = hue % 360;
    saturation = ((saturation > 100) ? 100 : saturation);
    brightness = (brightness > 100) ? 100 : brightness;

    h = hue / 60.0;
    s = saturation / 100.0;
    b = brightness / 100.0;

    f = h - (uint32_t)h;
    p = b * (1 - s);
    q = b * (1 - f * s);
    t = b * (1 - (1 - f) * s);

    switch ((uint32_t)h) {
        case 0:
            red = b;
            green = t;
            blue = p;
            break;

        case 1:
            red = q;
            green = b;
            blue = p;
            break;

        case 2:
            red = p;
            green = b;
            blue = t;
            break;

        case 3:
            red = p;
            green = q;
            blue = b;
            break;

        case 4:
            red = t;
            green = p;
            blue = b;
            break;

        case 5:
            red = b;
            green = p;
            blue = q;
            break;
    }

    rgb[0] = round(red * 255);
    rgb[1] = round(green * 255);
    rgb[2] = round(blue * 255);

    return;
}

static void rgb_led_update_current_color(uint8_t rgb[]) {
    uint8_t  red_color = rgb[0];
    uint8_t  green_color = rgb[1];
    uint8_t  blue_color = rgb[2];

    analogWrite(RGB_LED_RED_PIN, red_color);
    analogWrite(RGB_LED_GREEN_PIN, green_color);
    analogWrite(RGB_LED_BLUE_PIN, blue_color);

    Serial.print("red_color = ");
    Serial.print(red_color);
    Serial.print("; green_color = ");
    Serial.print(green_color);
    Serial.print("; blue_color = ");
    Serial.println(blue_color);

    return;
}

static void rgb_led_color_step(void) {
    uint32_t hue = 0;
    uint8_t  rgb[3] = RGB_LED_NO_COLOR;

    /**
     * Only for step mode we need change the color.
     * But for freeze mode if color was changed in the another routine,
     * we need to update rgb.
     */
    if (current_mode == RGB_LED_COLOR_STEP ||
        (current_mode == RGB_LED_COLOR_FREEZE && is_color_changed_in_freeze)) {
        if (current_mode == RGB_LED_COLOR_STEP) {
            current_hue = (current_hue + 1) % (2 * 360);
        }

        is_color_changed_in_freeze = false;

        hue = (current_hue > 360) ? (2 * 360 - current_hue) : current_hue;
        Serial.print("hue = ");
        Serial.println(hue);
        rgb_led_hsb_to_rgb(hue, current_saturation, current_brightness, rgb);
        rgb_led_update_current_color(rgb);
    }

    return;
}

void rgb_led_setup(void) {
    step_event_id = step_timer.every(step_timer_tick, rgb_led_color_step);

    return;
}

static void rgb_led_set_mode(rgb_led_mode_t rgb_led_mode) {
    uint8_t no_color[3] = RGB_LED_NO_COLOR;

    current_mode = rgb_led_mode;

    if (current_mode == RGB_LED_OFF) {
        rgb_led_update_current_color(no_color);
    }

    return;
}

rgb_led_mode_t rgb_led_get_mode(void) {
    return current_mode;
}

void rgb_led_switch_on_off(void) {
    if (current_mode != RGB_LED_OFF) {
        rgb_led_set_mode(RGB_LED_OFF);
    } else {
        rgb_led_set_mode(RGB_LED_COLOR_STEP);
    }
}

void rgb_led_update(void) {
    step_timer.update();
}

void rgb_led_color_freeze_or_step(void) {
    switch (current_mode) {
        case RGB_LED_COLOR_STEP:
            /* Freeze */
            rgb_led_set_mode(RGB_LED_COLOR_FREEZE);
            break;

        case RGB_LED_COLOR_FREEZE:
            /* Step */
            rgb_led_set_mode(RGB_LED_COLOR_STEP);
            break;

        default:
            /* RGB_LED_OFF */
            break;
    }

    return;
}

void rgb_led_increment_brightness(void) {
    uint8_t increment = 0;

    if (current_mode == RGB_LED_OFF) {
        return;
    }

    Serial.print("brightness ");
    Serial.print(current_brightness);

    increment = (current_brightness != RGB_LED_MAX_BRIGHTNESS) ? 1 : 0;
    current_brightness += increment;

    Serial.print(" => brightness ");
    Serial.println(current_brightness);

    if (current_mode == RGB_LED_COLOR_FREEZE && increment != 0) {
        is_color_changed_in_freeze = true;
    }

    return;
}

void rgb_led_decrement_brightness(void) {
    uint8_t decrement = 0;

    if (current_mode == RGB_LED_OFF) {
        return;
    }

    Serial.print("brightness ");
    Serial.print(current_brightness);

    decrement = (current_brightness != RGB_LED_MIN_BRIGHTNESS) ? 1 : 0;
    current_brightness -= decrement;

    Serial.print(" => brightness ");
    Serial.println(current_brightness);

    if (current_mode == RGB_LED_COLOR_FREEZE && decrement != 0) {
        is_color_changed_in_freeze = true;
    }

    return;
}

void rgb_led_increment_step_tick(void) {
    if (current_mode != RGB_LED_COLOR_STEP) {
        return;
    }

    Serial.print("step_timer_tick ");
    Serial.print(step_timer_tick);

    step_timer_tick += (step_timer_tick != RGB_LED_MAX_TICK) ? RGB_LED_STEP_TICK : 0;

    step_timer.stop(step_event_id);
    step_event_id = step_timer.every(step_timer_tick, rgb_led_color_step);

    Serial.print(" => step_timer_tick ");
    Serial.println(step_timer_tick);

    return;
}

void rgb_led_decrement_step_tick(void) {
    if (current_mode != RGB_LED_COLOR_STEP) {
        return;
    }

    Serial.print("step_timer_tick ");
    Serial.print(step_timer_tick);

    step_timer_tick -= (step_timer_tick != RGB_LED_MIN_TICK) ? RGB_LED_STEP_TICK : 0;

    step_timer.stop(step_event_id);
    step_event_id = step_timer.every(step_timer_tick, rgb_led_color_step);

    Serial.print(" => step_timer_tick ");
    Serial.println(step_timer_tick);

    return;
}

