#ifndef LCD_H_
#define LCD_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// I2C 주소 정의
#define LCD_I2C_ADDR 0x27 

// 함수 프로토타입 선언
void I2C_init(void);
void LCD_init(void);
void LCD_command(uint8_t command);
void LCD_data(uint8_t data);
void LCD_string(uint8_t row, uint8_t col, char *string);
void LCD_clear(void);

#endif