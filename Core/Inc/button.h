#ifndef BUTTON_H_
#define BUTTON_H_

#include <avr/io.h>
#include <util/delay.h>

#define MY_BUTTON_PIN   2 

enum {PUSHED, RELEASED};
enum {
    NO_ACT,
    ACT_PUSH,
    ACT_RELEASE
};

typedef struct {
    volatile uint8_t *ddr; 
    volatile uint8_t *pin; 
    uint8_t btnPin;
    uint8_t prevState; 
} BUTTON;

void ButtonInit(BUTTON *button, volatile uint8_t *ddr, volatile uint8_t *pin, uint8_t pinNum);
uint8_t ButtongGetState(BUTTON *button);

#endif