#include "stm32f1xx_hal.h"
#include "buzzer.h"

void Buzzer_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = Buzzer_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStruct);
}

void Buzzer_On(void) {
    HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
}

void Buzzer_Off(void) {
    HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_SET);
}

void Buzzer_Toggle(void) {
    HAL_GPIO_TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
}

void Buzzer_Beep(void) {
    Buzzer_On();
}

void Buzzer_Beep_Short(void) {
    Buzzer_On();
    HAL_Delay(100);
    Buzzer_Off();
}

void Buzzer_Beep_Long(void) {
    Buzzer_On();
    HAL_Delay(1000);
    Buzzer_Off();
}   

void Buzzer_Beep_Short_Long(void) {
    Buzzer_On();
    HAL_Delay(100);
    Buzzer_Off();
    HAL_Delay(1000);
}

void Buzzer_Beep_Long_Short(void) {
    Buzzer_On();
    HAL_Delay(1000);
    Buzzer_Off();
}