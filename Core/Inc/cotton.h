#ifndef COTTON_H_
#define COTTON_H_

#include <avr/io.h>

extern uint8_t cotton_state; // 외부 참조 허용

void init_Cotton_System(void);
void set_Cotton_Manual(uint8_t state);
void update_Cotton(uint16_t current_light);

#endif