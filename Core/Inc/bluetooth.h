#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

extern volatile char rx_cmd;
extern volatile uint8_t system_mode; // 1: Auto, 0: Manual

// 🌟 함수 이름 UART1으로 변경
void UART1_init(void);
void UART1_transmit(char data);
void UART1_print(char* str);

void process_bluetooth_command(void);
void send_bluetooth_data(uint8_t temp, uint8_t humi, uint16_t light, uint8_t mode, uint8_t f_state, uint8_t c_state, uint8_t l_state);

#endif