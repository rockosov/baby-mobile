#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#define RGB_LED_DEFAULT_HUE        0
#define RGB_LED_DEFAULT_SATURATION 100
#define RGB_LED_DEFAULT_BRIGHTNESS 100
#define RGB_LED_NO_COLOR           {0, 0, 0}
#define RGB_LED_DEFAULT_TICK       100

typedef enum {
    RGB_LED_OFF = 0,
    RGB_LED_COLOR_STEPPER = 1,
} rgb_led_mode_t;

void rgb_led_setup(void);
void rgb_led_set_mode(rgb_led_mode_t rgb_led_mode);
rgb_led_mode_t rgb_led_get_mode(void);
void rgb_led_update(void);

#endif /* !__RGB_LED_H__ */

