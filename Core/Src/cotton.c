#include "cotton.h"

uint8_t cotton_state = 0;       // 0: 닫힘, 1: 열림
static uint16_t current_angle = 160;   // 서보모터의 '현재' 각도
static uint16_t target_angle = 160;    // 서보모터가 '가야 할' 목표 각도
static uint16_t move_tick = 0;         // 스무스 이동 속도 조절용

void init_Cotton_System(void) {
    DDRB |= (1 << PB7); 
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1 = 4999;
    
    OCR1C = current_angle; // 시작할 때 현재 각도로 초기화
    cotton_state = 0; 
}

void update_Cotton(uint16_t current_light) {
    
    // ====================================================
    // 1. 센서 값 판별: 모터를 직접 돌리지 않고 "목표치"만 설정합니다.
    // ====================================================
    if (current_light > 300 && cotton_state == 0) {
        target_angle = 375;  // 목표: 열기 (90도)
        cotton_state = 1; 
    }
    else if (current_light < 200 && cotton_state == 1) {
        target_angle = 150;  // 목표: 닫기 (0도 부근)
        cotton_state = 0; 
    }

    // ====================================================
    // 2. 스무스(Smooth) 이동 로직: 관성 제거의 핵심!
    // ====================================================
    // main문이 엄청 빨리 돌아도, 여기서는 10번 루프에 1번씩(약 10ms)만 각도를 바꿉니다.
    if (move_tick % 5 == 0) { 
        
        // 현재 각도가 목표치보다 작으면 1씩 살금살금 올림
        if (current_angle < target_angle) {
            current_angle++;
            OCR1C = current_angle;
        }
        // 현재 각도가 목표치보다 크면 1씩 살금살금 내림
        else if (current_angle > target_angle) {
            current_angle--;
            OCR1C = current_angle;
        }
    }
    
    
    move_tick++;
    if (move_tick >= 1000) move_tick = 0;
}

void set_Cotton_Manual(uint8_t state) {
    if (state == 1) {
        target_angle = 375; // 열기
        cotton_state = 1;
    } else {
        target_angle = 150; // 닫기
        cotton_state = 0;
    }
}