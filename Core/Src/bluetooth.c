#include "bluetooth.h"
#include "fan.h"
#include "cotton.h"
#include "led.h"

volatile char rx_cmd = 0;
volatile uint8_t system_mode = 1;

void UART1_init(void) {
    UBRR1H = 0;
    UBRR1L = 103; 
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void UART1_transmit(char data) {
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = data;
}

void UART1_print(char* str) {
    while (*str) UART1_transmit(*str++);
}

// 🌟 인터럽트를 1번 채널로 변경
ISR(USART1_RX_vect) {
    rx_cmd = UDR1; 
}

void process_bluetooth_command(void) {
    if (rx_cmd == 0) return;

    char cmd = rx_cmd;
    rx_cmd = 0;

    if (cmd == 'A') {
        system_mode = 1;
        fan_mode = FAN_AUTO;
    } 
    else if (cmd == 'M') {
        system_mode = 0;
        fan_mode = FAN_MANUAL;
    }
    
    if (system_mode == 0) {
        switch (cmd) {
            case 'f': set_Fan_Relay(FAN_OFF); break;
            case 'F': set_Fan_Relay(FAN_ON); break;
            case 'c': set_Cotton_Manual(0); break;
            case 'C': set_Cotton_Manual(1); break;
            case 'l': set_LED_Manual(0); break;
            case 'L': set_LED_Manual(1); break;
        }
    }
}

void send_bluetooth_data(uint8_t temp, uint8_t humi, uint16_t light, uint8_t mode, uint8_t f_state, uint8_t c_state, uint8_t l_state) {
    char bt_buffer[80];
    system_mode = mode;
    if (system_mode == 1) {
        sprintf(bt_buffer, "T:%d H:%d L:%d M:Auto F:%d C:%d D:%d\n", 
                temp, humi, light, f_state, c_state, l_state);
    } else {
        sprintf(bt_buffer, "T:%d H:%d L:%d M:Manual F:%d C:%d D:%d\n", 
                temp, humi, light, f_state, c_state, l_state);
    }
    UART1_print(bt_buffer); // 🌟 UART1으로 전송
}