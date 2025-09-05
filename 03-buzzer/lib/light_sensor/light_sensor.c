#include "stm32f1xx_hal.h"
#include "light_sensor.h"

void Light_Sensor_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = Light_Sensor_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(Light_Sensor_GPIO_Port, &GPIO_InitStruct);
}

uint8_t Light_Sensor_Read(void) {
    return HAL_GPIO_ReadPin(Light_Sensor_GPIO_Port, Light_Sensor_Pin);
}
