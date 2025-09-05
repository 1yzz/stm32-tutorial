#include "stm32f1xx_hal.h"

void OLED_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

void OLED_Write(uint8_t data) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, data);
}