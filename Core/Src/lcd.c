#include "lcd.h"

void I2C_init(void) {
    TWBR = 72; 
    TWSR = 0x00; 
}

void I2C_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void I2C_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void I2C_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void LCD_send(uint8_t value, uint8_t rs_mode) {
    uint8_t high_nib = value & 0xF0;
    uint8_t low_nib = (value << 4) & 0xF0;
    uint8_t flag = rs_mode | 0x08; 

    I2C_start();
    I2C_write(LCD_I2C_ADDR << 1); 
    
    I2C_write(high_nib | flag | 0x04); 
    I2C_write(high_nib | (flag & ~0x04)); // 괄호 추가
    
    I2C_write(low_nib | flag | 0x04); 
    I2C_write(low_nib | (flag & ~0x04)); // 괄호 추가

    I2C_stop();
    _delay_us(50); 
}

void LCD_command(uint8_t command) {
    LCD_send(command, 0); 
}

void LCD_data(uint8_t data) {
    LCD_send(data, 1); 
}

void LCD_init(void) {
    I2C_init();
    _delay_ms(50); 
    
    LCD_send(0x30, 0); _delay_ms(5);
    LCD_send(0x30, 0); _delay_us(150);
    LCD_send(0x30, 0); _delay_us(150);
    LCD_send(0x20, 0); _delay_ms(5); 

    LCD_command(0x28); 
    LCD_command(0x08); 
    LCD_command(0x01); 
    _delay_ms(2);
    LCD_command(0x06); 
    LCD_command(0x0C); 
}

void LCD_string(uint8_t row, uint8_t col, char *string) {
    uint8_t address = (row == 0) ? 0x80 : 0xC0; 
    address += col;
    
    LCD_command(address);
    
    while (*string) {
        LCD_data(*string++);
    }
}

void LCD_clear(void) {
    LCD_command(0x01);
    _delay_ms(2);
}