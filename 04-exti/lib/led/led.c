#include "stm32f1xx_hal.h"
#include "led.h"

void LED_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
    
    // Ensure LED is off after initialization
    LED_Off();
}

void LED_On(void) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_PIN, GPIO_PIN_RESET);
}

void LED_Off(void) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_PIN, GPIO_PIN_SET);
}

void LED_Toggle(void) {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_PIN);
}