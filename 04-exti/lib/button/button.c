#include "stm32f1xx_hal.h"
#include "button.h"

// 防抖变量
static uint32_t last_interrupt_time = 0;

void Button_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);
}

void Button_EXTI_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

    // Configure EXTI line
    HAL_NVIC_SetPriority(BUTTON_EXTI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(BUTTON_EXTI_IRQn);
}

uint8_t Button_Pressed(void) {
    if (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_PIN) == GPIO_PIN_RESET) {
        HAL_Delay(50);
        while (HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_PIN) == GPIO_PIN_RESET);
        HAL_Delay(50);
        return 1;
    }
    return 0;
}

// 防抖检查函数 - 在中断中调用
uint8_t Button_Debounce_Check(void) {
    uint32_t current_time = HAL_GetTick();
    
    // 检查是否超过防抖时间
    if ((current_time - last_interrupt_time) >= BUTTON_DEBOUNCE_TIME_MS) {
        last_interrupt_time = current_time;
        return 1; // 允许处理这次中断
    }
    return 0; // 忽略这次中断（在防抖时间内）
}