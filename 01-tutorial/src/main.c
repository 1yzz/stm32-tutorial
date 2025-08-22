#include "stm32f1xx_hal.h"

int main(void) {
    HAL_Init(); // Initialize the HAL Library

    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_13; // Pin 13    
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Set as Push-Pull Output
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Low speed

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); // Initialize GPIOC Pin 13  

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // Set Pin 13 low

    while (1)
    {
        /* code */
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Set Pin 13 low
        HAL_Delay(500); // Delay 200 ms 
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // Set Pin 13 high
        HAL_Delay(500); // Delay 200 ms

    }
    
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1) {}
}


void MemManage_Handler(void)
{
  while (1) {}
}

void BusFault_Handler(void)
{
  while (1) {}
}

void UsageFault_Handler(void)
{
  while (1) {}
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}
