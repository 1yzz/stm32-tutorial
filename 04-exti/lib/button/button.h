#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f1xx_hal.h"

#define BUTTON_PIN GPIO_PIN_0
#define BUTTON_GPIO_Port GPIOA
#define BUTTON_EXTI_IRQn EXTI0_IRQn

// 防抖设置
#define BUTTON_DEBOUNCE_TIME_MS 20  // 防抖时间50ms

void Button_Init(void);
void Button_EXTI_Init(void);
uint8_t Button_Pressed(void);
uint8_t Button_Debounce_Check(void);

#endif // BUTTON_H