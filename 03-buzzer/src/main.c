#include "stm32f1xx_hal.h"
#include "buzzer.h"
#include "light_sensor.h"

int main(void) {
    HAL_Init();
    Buzzer_Init();
    Light_Sensor_Init();

    while (1) {
       if (Light_Sensor_Read() == GPIO_PIN_RESET) {
        Buzzer_On();
       } else {
        Buzzer_Off();
       }
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
}