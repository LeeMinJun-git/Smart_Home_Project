#ifndef FAN_H_
#define FAN_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// 선풍기 상태 정의
enum { FAN_MANUAL, FAN_AUTO };
enum { FAN_OFF, FAN_ON };
enum { SPIN_OFF, SPIN_ON };

extern uint8_t fan_mode;
extern uint8_t fan_state;
extern uint8_t spin_state;
extern uint8_t current_temp;
extern uint8_t current_humi;

// 함수 프로토타입
void init_Fan_System(void);
void set_Fan_Relay(uint8_t state);
void process_Fan_Servo(void);

/* DHT11(PD1): 0=에러, 1=ON조건, 2=OFF조건, 3=유지(히스테리시스) */
uint8_t check_dht11_condition(void);
void fan_apply_auto_from_dht(void);

#endif