#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "led.h"
#include "button.h"
#include "cotton.h" 
#include "fan.h"    
#include "lcd.h"    // LCD 헤더 추가

#define BTN_AUTO_MANUAL  3 
#define BTN_FAN_ONOFF    4 
#define BTN_SPIN_ONOFF   5 

// int main(void) {
//     // 1. LCD 초기화 (내부적으로 I2C 통신도 함께 초기화됨)
//     LCD_init();

//     // 2. 1행(첫 번째 줄)에 고정 메시지 출력
//     LCD_string(0, 0, "LCD Test Mode!!");

//     uint16_t counter = 0;
//     char buffer[17]; // 16글자 + 널 문자

//     while (1) {
//         // 3. 2행(두 번째 줄)에 1초마다 올라가는 카운터 출력
//         // 보드가 다운되지 않고 잘 살아있는지 확인하는 용도입니다.
//         sprintf(buffer, "Counter: %04d   ", counter);
//         LCD_string(1, 0, buffer);

//         counter++;
        
//         // 1초 대기
//         _delay_ms(1000); 
//     }

//     return 0;
// }



// int main(void) {
//     // 1. 모든 시스템 초기화
//     LCD_init();           // LCD 초기화 추가
//     init_LED_System();    
//     init_Cotton_System(); 
//     init_Fan_System();    

//     BUTTON btnMode, btnAutoToggle, btnFanToggle, btnSpinToggle;
    
//     ButtonInit(&btnMode, &DDRG, &PING, MY_BUTTON_PIN); 
//     PORTG |= (1 << MY_BUTTON_PIN); 

//     ButtonInit(&btnAutoToggle, &DDRA, &PINA, BTN_AUTO_MANUAL);
//     ButtonInit(&btnFanToggle, &DDRA, &PINA, BTN_FAN_ONOFF);
//     ButtonInit(&btnSpinToggle, &DDRA, &PINA, BTN_SPIN_ONOFF);
//     PORTA |= (1 << BTN_AUTO_MANUAL) | (1 << BTN_FAN_ONOFF) | (1 << BTN_SPIN_ONOFF);

//     uint16_t timer_tick = 0; 
//     uint16_t dht_timer = 0;  
//     uint16_t lcd_timer = 0;  // LCD 갱신용 타이머 변수 추가

//     while (1) {
//         // [1단계] 센서 데이터 측정
//         uint32_t sum = 0;
//         for(int i = 0; i < 16; i++) sum += read_ADC(0);
//         uint16_t current_light = sum / 16;

//         // [2단계] 사용자 입력 처리
//         if (ButtongGetState(&btnMode) == ACT_PUSH) {
//             mode = (mode + 1) % 3; 
//         }

//         if (ButtongGetState(&btnAutoToggle) == ACT_PUSH) {
//             fan_mode = (fan_mode == FAN_MANUAL) ? FAN_AUTO : FAN_MANUAL;
//             if (fan_mode == FAN_AUTO) {
//                 dht_timer = 0;
//                 fan_apply_auto_from_dht();
//             } else {
//                 set_Fan_Relay(FAN_OFF);
//                 spin_state = SPIN_OFF;
//             }
//         }

//         if (fan_mode == FAN_MANUAL) {
//             if (ButtongGetState(&btnSpinToggle) == ACT_PUSH) {
//                 spin_state = (spin_state == SPIN_ON) ? SPIN_OFF : SPIN_ON;
//             }
//         }

//         if (ButtongGetState(&btnFanToggle) == ACT_PUSH) {
//             if (fan_mode == FAN_AUTO) fan_mode = FAN_MANUAL;
//             set_Fan_Relay((fan_state == FAN_ON) ? FAN_OFF : FAN_ON);
//         }

//         // [3단계] 각 장치 동작 업데이트
//         update_LED_Brightness(current_light, mode);
//         update_Cotton(current_light);

//         if (fan_mode == FAN_AUTO) {
//             dht_timer++;
//             if (dht_timer >= 2000) { 
//                 dht_timer = 0;
//                 fan_apply_auto_from_dht();
//             }
//         }

//         process_Fan_Servo();

//         // [FSM 4단계] LCD 화면 데이터 갱신 (1초 주기)
//         lcd_timer++;
//         if (lcd_timer >= 1000) {
//             lcd_timer = 0;
            
//             // 🌟 배열 크기를 30으로 넉넉히 늘리고, {0, }으로 모든 칸을 깨끗하게 청소(초기화)
//             char line1[30] = {0, }; 
//             char line2[30] = {0, };

//             // 1행 포맷팅 (영어/숫자만 사용)
//             sprintf(line1, "t:%02d h:%02d l:%04d", current_temp, current_humi, current_light);
            
//             // 2행 포맷팅
//             char c_fan = (fan_state == 1) ? 'o' : 'x'; // 변수 대신 상태 함수로 체크
//             char c_led = (led_state == 1) ? 'o' : 'x';
//             char c_ct  = (cotton_state == 1) ? 'o' : 'x';

//             sprintf(line2, "fan:%c led:%c ct:%c", c_fan, c_led, c_ct);

//             // LCD에 출력
//             LCD_string(0, 0, line1);
//             LCD_string(1, 0, line2);
//         }

//         // 타이밍 유지
//         timer_tick++;



int main(void) {
    // 1. 모든 시스템 초기화
    LCD_init();           // LCD 초기화 추가
    init_LED_System();    
    init_Cotton_System(); 
    init_Fan_System();    

    BUTTON btnMode, btnAutoToggle, btnFanToggle, btnSpinToggle;
    
    ButtonInit(&btnMode, &DDRG, &PING, MY_BUTTON_PIN); 
    PORTG |= (1 << MY_BUTTON_PIN); 

    ButtonInit(&btnAutoToggle, &DDRA, &PINA, BTN_AUTO_MANUAL);
    ButtonInit(&btnFanToggle, &DDRA, &PINA, BTN_FAN_ONOFF);
    ButtonInit(&btnSpinToggle, &DDRA, &PINA, BTN_SPIN_ONOFF);
    PORTA |= (1 << BTN_AUTO_MANUAL) | (1 << BTN_FAN_ONOFF) | (1 << BTN_SPIN_ONOFF);

    uint16_t timer_tick = 0; 
    uint16_t dht_timer = 0;  
    uint16_t lcd_timer = 0;  // LCD 갱신용 타이머 변수 추가

    while (1) {
        // [1단계] 센서 데이터 측정
        uint32_t sum = 0;
        for(int i = 0; i < 16; i++) sum += read_ADC(0);
        uint16_t current_light = sum / 16;

        // [2단계] 사용자 입력 처리
        if (ButtongGetState(&btnMode) == ACT_PUSH) {
            mode = (mode + 1) % 3; 
        }

        if (ButtongGetState(&btnAutoToggle) == ACT_PUSH) {
            fan_mode = (fan_mode == FAN_MANUAL) ? FAN_AUTO : FAN_MANUAL;
            if (fan_mode == FAN_AUTO) {
                dht_timer = 0;
                fan_apply_auto_from_dht();
            } else {
                set_Fan_Relay(FAN_OFF);
                spin_state = SPIN_OFF;
            }
        }

        if (fan_mode == FAN_MANUAL) {
            if (ButtongGetState(&btnSpinToggle) == ACT_PUSH) {
                spin_state = (spin_state == SPIN_ON) ? SPIN_OFF : SPIN_ON;
            }
        }

        if (ButtongGetState(&btnFanToggle) == ACT_PUSH) {
            if (fan_mode == FAN_AUTO) fan_mode = FAN_MANUAL;
            set_Fan_Relay((fan_state == FAN_ON) ? FAN_OFF : FAN_ON);
        }
        update_LED_Brightness(current_light, mode);
        update_Cotton(current_light);

        // 🌟 수정: 모드와 상관없이 2초마다 온습도를 읽어오도록 변경
        dht_timer++;
        if (dht_timer >= 2000) { 
            dht_timer = 0;
            
            // DHT11 센서로부터 온도, 습도 값을 읽어와서 current_temp, current_humi 갱신
            // 내부적으로 condition 값을 반환하지만, 여기서는 변수 갱신이 목적
            uint8_t condition = check_dht11_condition(); 

            // 자동 모드일 때만 읽어온 값을 바탕으로 팬을 제어
            if (fan_mode == FAN_AUTO) {
                if (condition == 1) {        // ON 조건
                    set_Fan_Relay(FAN_ON);
                    spin_state = SPIN_ON;
                } else if (condition == 2 || condition == 0) { // OFF 또는 에러
                    set_Fan_Relay(FAN_OFF);
                    spin_state = SPIN_OFF;
                }
            }
        }

        process_Fan_Servo();

        // [FSM 4단계] LCD 화면 데이터 갱신 (1초 주기)
        lcd_timer++;
        if (lcd_timer >= 1000) {
            lcd_timer = 0;
            
            // 🌟 배열 크기를 30으로 넉넉히 늘리고, {0, }으로 모든 칸을 깨끗하게 청소(초기화)
            char line1[30] = {0, }; 
            char line2[30] = {0, };

            // 1행 포맷팅 (영어/숫자만 사용)
            sprintf(line1, "t:%02d h:%02d l:%04d", current_temp, current_humi, current_light);
            
            // 2행 포맷팅
            char c_fan = ((PORTA & (1 << PA0)) == 0) ? 'x' : 'o'; // 변수 대신 상태 함수로 체크
            char c_led = (led_state == 1) ? 'o' : 'x';
            char c_ct  = (cotton_state == 1) ? 'o' : 'x';

            sprintf(line2, "fan:%c led:%c ct:%c", c_fan, c_led, c_ct);


            // LCD에 출력
            LCD_string(0, 0, line1);
            LCD_string(1, 0, line2);
        }

        // 타이밍 유지
        timer_tick++;
        if (timer_tick >= 1000) timer_tick = 0;

        _delay_ms(1); 
    }
    
    return 0;
}