#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include <zephyr/types.h>

int rgb_led_init();
void rgb_led_set(uint8_t r, uint8_t g, uint8_t b);

#endif /* __RGB_LED_H_ */
