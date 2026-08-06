#include "fan.h"
#include <stdint.h>

#define DHT11_PIN PA1

uint8_t fan_mode = FAN_MANUAL;
uint8_t fan_state = FAN_OFF;
uint8_t spin_state = SPIN_OFF;

extern uint8_t current_temp = 0;
extern uint8_t current_humi = 0;

void init_Fan_System(void) {
    // 1. DC 팬 릴레이 (PA0) — 참조 펌웨어와 동일: Active-Low (LOW = 접점 ON)
    DDRA |= (1 << PA0);
    PORTA |= (1 << PA0); // 초기: 릴레이 OFF (팬 정지)

    // 2. 선풍기 회전 서보 모터 설정 (🌟 PE3 핀 사용 - Timer 3)
    DDRE |= (1 << PE3);
    
    // Timer 3 Fast PWM Mode 14, 분주비 64
    TCCR3A = (1 << COM3A1) | (1 << WGM31);
    TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31) | (1 << CS30);
    ICR3 = 4999;  // 20ms 주기 (16MHz 기준)
    OCR3A = 375;  // 서보 모터 중앙 정렬
    
    // 3. DHT11 (PA1): 입력 + 내부 풀업 (버스 idle HIGH)
    DDRA &= ~(1 << DHT11_PIN);
    PORTA |= (1 << DHT11_PIN);
}

void set_Fan_Relay(uint8_t state) {
    fan_state = state;
    if (state == FAN_ON) {
        PORTA &= ~(1 << PA0);
    } else {
        PORTA |= (1 << PA0);
    }
}

// 비동기 서보 모터 제어 (딜레이 없이 메인 루프에서 실행됨)
void process_Fan_Servo(void) {
    static uint16_t timeCount = 0;
    static uint16_t currentAngle = 375;
    static int16_t direction = 2;  // 부드러운 미세 이동

    if (spin_state == SPIN_ON) {
        timeCount++;
        if (timeCount >= 5) {
            timeCount = 0;
            currentAngle += direction;

            if (currentAngle >= 650) {
                currentAngle = 650;
                direction = -2;
            } else if (currentAngle <= 100) {
                currentAngle = 100;
                direction = 2;
            }

            OCR3A = currentAngle;
        }
    } else {
        timeCount = 0;
    }
}

uint8_t check_dht11_condition(void) {
    uint8_t data[5] = {0, 0, 0, 0, 0};
    uint8_t i, j;
    uint16_t timeout;

    // 🌟 수정 1: 마이크로초 타이밍이 꼬이지 않도록 모든 인터럽트 일시 중지
    cli(); 

    // MCU의 Start Signal 전송
    DDRA |= (1 << DHT11_PIN);    // 출력 모드로 설정
    PORTA &= ~(1 << DHT11_PIN);  // LOW 신호 전송
    _delay_ms(18);               // 18ms 유지

    // 🌟 수정 2: 버스 충돌 방지를 위해 즉시 입력 모드로 전환하여 센서에게 양보
    DDRA &= ~(1 << DHT11_PIN);   // 입력 모드로 전환 (MCU 점유 해제)
    PORTA |= (1 << DHT11_PIN);   // 내부 풀업 저항 활성화 (안정성 확보)

    // 안전장치 0: 센서가 응답(LOW)할 때까지 대기 (약 20~40us 소요)
    timeout = 10000;
    while (PINA & (1 << DHT11_PIN)) {
        if (--timeout == 0) { sei(); return 0; } // 타임아웃 시 인터럽트 켜고 탈출
    }

    // 🌟 안전장치 1: HIGH 기다리기
    timeout = 10000;
    while (!(PINA & (1 << DHT11_PIN))) {
        if (--timeout == 0) { 
            // UART1_print("Error: 1\n"); // 폰으로 에러 위치 전송
            sei(); return 0; 
        }
    }
    
    // 🌟 안전장치 2: LOW 기다리기
    timeout = 10000;
    while (PINA & (1 << DHT11_PIN)) {
        if (--timeout == 0) { 
            // UART1_print("Error: 2\n"); // 폰으로 에러 위치 전송
            sei(); return 0; 
        }
    }

    // 데이터 40비트(5바이트) 읽기 시작
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            // 비트 전송 시작(LOW 50us)이 끝날 때까지 대기
            timeout = 10000;
            while (!(PINA & (1 << DHT11_PIN))) {
                if (--timeout == 0) { sei(); return 0; }
            }

            // 🌟 수정 3: '0'(28us)과 '1'(70us)을 명확히 구분하기 위해 40us 대기
            _delay_us(40);

            // 40us가 지났는데도 핀이 HIGH라면 이것은 '1'을 의미함
            if (PINA & (1 << DHT11_PIN)) {
                data[i] |= (1 << (7 - j));
            }

            // 해당 비트의 HIGH 구간이 완전히 끝날 때까지 대기
            timeout = 10000;
            while (PINA & (1 << DHT11_PIN)) {
                if (--timeout == 0) { sei(); return 0; }
            }
        }
    }

    // 🌟 데이터 수신이 끝났으므로 인터럽트 다시 활성화
    sei();

    // Checksum 검증: 읽어온 데이터가 정상인지 확인
    if (data[4] == (uint8_t)(data[0] + data[1] + data[2] + data[3])) {
        current_humi = data[0];
        current_temp = data[2];

        // 팬 ON 조건: 온도 29℃ 이상 또는 습도 61% 이상
        if (current_temp >= 29 || current_humi >= 61)
            return 1;
        // 팬 OFF 조건: 온도 23℃ 이하 이고 습도 40% 이하
        if (current_temp <= 23 && current_humi <= 40)
            return 2;

        // 히스테리시스 (상태 유지)
        return 3;
    }

    return 0; // Checksum 실패 시 에러 처리
}

void fan_apply_auto_from_dht(void) {
    uint8_t condition = check_dht11_condition();

    if (condition == 0) {
        set_Fan_Relay(FAN_OFF);
        spin_state = SPIN_OFF;
    } else if (condition == 1) {
        set_Fan_Relay(FAN_ON);
        spin_state = SPIN_ON;
    } else if (condition == 2) {
        set_Fan_Relay(FAN_OFF);
        spin_state = SPIN_OFF;
    }
}

