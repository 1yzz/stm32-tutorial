#include "stm32f1xx_hal.h"
#include "light_sensor.h"

// 防抖变量
static uint32_t light_last_interrupt_time = 0;

void Light_Sensor_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = Light_Sensor_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(Light_Sensor_GPIO_Port, &GPIO_InitStruct);
}

void Light_Sensor_EXTI_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = Light_Sensor_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(Light_Sensor_GPIO_Port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(Light_Sensor_EXTI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(Light_Sensor_EXTI_IRQn);
}

uint8_t Light_Sensor_Read(void) {
    return HAL_GPIO_ReadPin(Light_Sensor_GPIO_Port, Light_Sensor_Pin);
}