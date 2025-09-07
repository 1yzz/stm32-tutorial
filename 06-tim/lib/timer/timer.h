#ifndef TIMER_H
#define TIMER_H

#include "stm32f1xx_hal.h"

// Timer configuration
#define TIMER_INSTANCE TIM2
#define TIMER_PRESCALER 71999 // For 1ms tick at 72MHz: (72MHz / 1000) - 1
#define TIMER_PERIOD 1000     // 1000ms = 1 second

// Function declarations
void TIMER_Init(void);
void TIMER_Start(void);
void TIMER_Stop(void);
void TIMER_SetPeriod(uint32_t period_ms);

// Interrupt callback function (to be implemented in main.c)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif // TIMER_H
