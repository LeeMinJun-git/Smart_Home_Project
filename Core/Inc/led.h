#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include <util/delay.h>

extern uint8_t mode;
extern uint8_t led_state; // 추가됨

void init_LED_System(void);
void update_LED_Brightness(uint16_t raw_light, uint8_t current_mode); 
uint16_t read_ADC(uint8_t channel);
void set_RGB_Color(uint8_t r, uint8_t g, uint8_t b);
void set_LED_Manual(uint8_t state);

#endif