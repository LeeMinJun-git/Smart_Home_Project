# 🌙 ATmega128A 기반 실외 환경 동기화 지능형 수면 제어 시스템
> **실시간 조도 및 온습도 피드백을 통한 최적의 수면 환경 자동화 및 하이브리드 제어 솔루션**

![C](https://img.shields.io/badge/Language-C-00599C?style=flat-square&logo=c)
![MCU](https://img.shields.io/badge/MCU-ATmega128A-03234B?style=flat-square&logo=microchip)
![Environment](https://img.shields.io/badge/Environment-Linux-FCC624?style=flat-square&logo=linux)

## 📌 프로젝트 소개
조도 센서와 온습도 센서를 통해 실내외 환경을 실시간으로 감지하고, 커튼, LED 무드등, 선풍기 등의 액추에이터를 유기적으로 제어하는 지능형 수면 제어 시스템입니다. 
사용자의 수면 본능에 최적화된 환경 밸런스를 자율적으로 유지(Auto Mode)하면서도, 필요시 버튼이나 블루투스(스마트폰)를 통해 즉각적인 개입이 가능한 하이브리드 제어(Manual Mode) 환경을 구축했습니다.

## 🛠️ 기술 스택 및 개발 환경
*   **MCU** : ATmega128A
*   **Language** : C (Bare-metal)
*   **OS Environment** : Linux (Ubuntu)
*   **Toolchain** : Visual Studio Code, AVR-GCC, Make

## 💡 핵심 기능 및 설계 특징 (Key Features)

### 1. 실외 환경 능동 동기화 (Real-time Synchronization)
*   **지능형 커튼 및 조명:** 조도 센서(CDS)의 ADC 데이터를 16회 측정 후 평균값을 산출하여 노이즈를 필터링하고, 빛 유입량에 맞춰 커튼 모터(Fast PWM)의 각도와 RGB LED의 디밍(Dimming)을 부드럽게 자동 제어.
*   **쾌적 수면 온도 유지:** 1-Wire 프로토콜로 구현한 DHT11 센서 제어를 통해 온습도를 측정, 설정된 이상적 수면 조건(온도 24~29도, 습도 40~60%)을 벗어날 경우 릴레이를 통해 선풍기를 자동 구동[cite: 4, 5].

### 2. 다중 모듈 병렬 제어 알고리즘
*   비동기적 타이머 인터럽트를 활용하여 센서 데이터 스캐닝, 블루투스 UART 송수신, 모터 PWM 제어가 메인 루프의 지연 없이 동시에 구동되도록 설계[cite: 4].
*   각 기기의 동작을 독립적인 상태 머신(FSM) 모듈로 분리하여 코드의 재사용성 확보[cite: 4, 5].

### 3. 실시간 모니터링 및 시각적 피드백
*   I2C 통신 방식의 1602 LCD를 연동하여 현재 측정된 온/습도/조도 수치와 각 장치(Fan, LED, Curtain)의 동작 상태, 제어 모드(Auto/Manual)를 1초 주기로 갱신하여 출력[cite: 4, 5].

## ⚙️ 하드웨어 구성 및 핀맵 (Hardware PinMap)

| 포트/핀 | 컴포넌트 | 제어 방식 및 기능 |
| :--- | :--- | :--- |
| **PF0** | CDS 조도 센서 | ADC0 - 아날로그 입력 (0~1023 조도 값 측정) |
| **PA1** | DHT11 온습도 센서 | 디지털 입출력 (1-Wire 데이터 통신) |
| **PB4, 5, 6** | RGB LED 모듈 | Timer 0, 1 기반 PWM 출력 (Red, Green, Blue 밝기 제어) |
| **PA0** | DC 선풍기 모터 | Relay 모듈을 통한 메인 전원 디지털 ON/OFF 제어 |
| **PE3** | 선풍기 회전 서보 모터 | Timer 3 (OC3A) - Fast PWM (20ms 주기) 좌우 회전 제어 |
| **PB7** | 스마트 커튼 서보 모터 | Timer 1 (OC1C) - Fast PWM (20ms 주기) 차광 각도 제어 |
| **PD0, PD1** | I2C 1602 LCD | I2C (SCL, SDA) 통신을 통한 실시간 시스템 상태 출력 |
| **UART1** | HC-05 블루투스 | 스마트폰 앱 기반 무선 원격 제어 및 데이터 로깅 |
| **PA3, 4, 5, PG2** | 물리 스위치 | Pull-up 저항 활용 상태 제어 (모드 변경, 회전, 전원 등) |

## 📁 디렉토리 구조 (Directory Structure)
각 하드웨어 모듈별로 역할을 명확히 분리하여 `src`와 `inc` 폴더로 모듈화했습니다.
```text
📦 Smart-Sleep-Control-System
 ┣ 📂 src
 ┃ ┣ 📜 main.c        # 메인 루프, 시스템 초기화 및 비동기 틱(Tick) 타이머 스케줄링[cite: 4]
 ┃ ┣ 📜 button.c      # 물리 버튼 GPIO 설정 및 디바운싱(Debouncing) 상태 판별 로직[cite: 4]
 ┃ ┣ 📜 cotton.c      # Timer 1 (OC1C) 기반 PWM 제어를 통한 스마트 커튼 서보모터 구동[cite: 4]
 ┃ ┣ 📜 fan.c         # DHT11 1-Wire 데이터 파싱 및 릴레이(DC 모터), 회전 서보 제어[cite: 4]
 ┃ ┣ 📜 lcd.c         # I2C(TWI) 레지스터 직접 제어 및 1602 LCD 화면 출력 프로토콜[cite: 4]
 ┃ ┣ 📜 led.c         # 조도 센서 ADC 변환 및 RGB LED PWM 디밍(Dimming) 제어[cite: 4]
 ┃ ┗ 📜 bluetooth.c   # UART1 인터럽트(RX) 기반 블루투스 명령어 파싱 및 모바일 상태 전송[cite: 4]
 ┣ 📂 inc
 ┃ ┣ 📜 button.h      # 버튼 구조체(BUTTON) 및 액션 열거형 선언[cite: 4]
 ┃ ┣ 📜 cotton.h      # 커튼 구동 외부 전역 변수 및 함수 선언[cite: 4]
 ┃ ┣ 📜 fan.h         # DHT11 통신 타이밍 매크로 및 팬 상태 선언[cite: 4]
 ┃ ┣ 📜 lcd.h         # I2C 주소(0x27) 매크로 및 LCD 초기화 함수 선언[cite: 4]
 ┃ ┣ 📜 led.h         # LED 색상/밝기 매핑 및 ADC 채널 설정 선언[cite: 4]
 ┃ ┗ 📜 bluetooth.h   # UART 통신 속도(Baudrate) 및 버퍼 제어 선언[cite: 4]
 ┗ 📜 README.md
