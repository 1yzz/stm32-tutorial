#include "led.h"

void LED_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable clock for GPIOB

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED_PIN; // Pin 10
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Set as Push-Pull Output
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // Low speed

    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct); // Initialize GPIOB Pin 10
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET); // Turn off LED initially
}

void LED_On(void) {
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_RESET); // Set Pin 10 low to turn on LED
}

void LED_Off(void) {
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_PIN, GPIO_PIN_SET); // Set Pin 10 high to turn off LED
}

void LED_Toggle(void) {
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN); // Toggle Pin 10
}