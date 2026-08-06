#include "led.h"

uint8_t mode = 0;
uint8_t led_state = 0; // 전역 변수 선언

void init_LED_System(void) {
    MCUCSR |= (1 << JTD); MCUCSR |= (1 << JTD);
    DDRB |= (1 << PB4) | (1 << PB5) | (1 << PB6);
    TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS02);

    OCR0 = 0;   
    OCR1A = 0;  
    OCR1B = 0;  

    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_ADC(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void set_RGB_Color(uint8_t r, uint8_t g, uint8_t b) {
    OCR0 = r;    
    OCR1A = g;   
    OCR1B = b;   

    // 밝기 값이 0이면 꺼짐(0), 1 이상이면 켜짐(1) 처리
    if (r == 0 && g == 0 && b == 0) {
        led_state = 0;
    } else {
        led_state = 1;
    }
}

void update_LED_Brightness(uint16_t raw_light, uint8_t current_mode) {
    uint8_t val = raw_light / 4; 

    if (val < 3) val = 3; 

    switch (current_mode) {
        case 0: 
            set_RGB_Color(val, val, val); 
            break;

        case 1: 
            set_RGB_Color(val, val, 0);
            break;

        case 2: 
            set_RGB_Color(0, 0, 0); 
            break;
            
        default:
            set_RGB_Color(0, 0, 0);
            break;
    }
}