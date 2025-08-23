#include "stm32f1xx_hal.h"
#include "button.h"

void Button_Init(void) {
    // Enable GPIOA clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = Button_Pin; // Assuming the button is connected to PA0
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Set as input mode
    GPIO_InitStruct.Pull = GPIO_PULLUP; // No pull-up or pull-down resistor
    HAL_GPIO_Init(Button_GPIO_Port, &GPIO_InitStruct);   
}

uint8_t Button_Pressed(void) {
    uint8_t button_state = 0;
    if (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_RESET) {
        HAL_Delay(50);
        while (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_RESET);
        HAL_Delay(50);
        button_state = 1;
    }
    return button_state;
}