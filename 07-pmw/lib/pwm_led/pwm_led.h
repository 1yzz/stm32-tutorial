#ifndef PWM_LED_H
#define PWM_LED_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

// PWM LED configuration for PA0
#define PWM_LED_PIN GPIO_PIN_0
#define PWM_LED_GPIO_PORT GPIOA
#define PWM_LED_TIMER TIM2
#define PWM_LED_CHANNEL TIM_CHANNEL_1

// PWM frequency and resolution
#define PWM_FREQUENCY 1000  // 1kHz PWM frequency (smooth for LED)
#define PWM_RESOLUTION 1000  // 0-1000% duty cycle resolution (higher resolution)

// LED configuration for 高电平触发 (active high)
#define LED_ACTIVE_HIGH 1  // LED turns on when pin is HIGH

// Function declarations
void PWM_LED_Init(void);
void PWM_LED_SetDutyCycle(uint8_t duty_cycle);
void PWM_LED_On(void);
void PWM_LED_Off(void);
void PWM_LED_Toggle(void);
void PWM_LED_FadeIn(void);
void PWM_LED_FadeOut(void);
void PWM_LED_SetBrightness(uint8_t brightness); // 设置亮度 (0-100%)
uint8_t PWM_LED_GetCurrentDutyCycle(void); // 获取当前占空比
void PWM_LED_Breathing(void); // 呼吸灯效果 (非阻塞)
void PWM_LED_BreathingBlocking(uint16_t cycle_time_ms); // 呼吸灯效果 (阻塞)

#endif // PWM_LED_H
