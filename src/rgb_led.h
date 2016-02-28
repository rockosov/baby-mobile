#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#define RGB_LED_DEFAULT_HUE        0
#define RGB_LED_DEFAULT_SATURATION 100
#define RGB_LED_DEFAULT_BRIGHTNESS 100
#define RGB_LED_MIN_BRIGHTNESS     0
#define RGB_LED_MAX_BRIGHTNESS     100
#define RGB_LED_NO_COLOR           {0, 0, 0}
#define RGB_LED_DEFAULT_TICK       0
#define RGB_LED_MIN_TICK           0
#define RGB_LED_MAX_TICK           200
#define RGB_LED_STEP_TICK          20

typedef enum {
    RGB_LED_OFF          = 0,
    RGB_LED_COLOR_STEP   = 1,
    RGB_LED_COLOR_FREEZE = 2,
} rgb_led_mode_t;

void rgb_led_setup(void);
void rgb_led_switch_on_off(void);
rgb_led_mode_t rgb_led_get_mode(void);
void rgb_led_update(void);
void rgb_led_color_freeze_or_step(void);
void rgb_led_increment_brightness(void);
void rgb_led_decrement_brightness(void);
void rgb_led_increment_step_tick(void);
void rgb_led_decrement_step_tick(void);

#endif /* !__RGB_LED_H__ */

