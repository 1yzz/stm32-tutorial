#include "timer.h"

// Timer handle
TIM_HandleTypeDef htim2;

void TIMER_Init(void) {
    // Enable TIM2 clock
    __HAL_RCC_TIM2_CLK_ENABLE();
    
    // Configure TIM2
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = TIMER_PRESCALER;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = TIMER_PERIOD - 1;  // ARR value
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    // Initialize TIM2
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
        // Error handling - could add error LED or other indication
    }
    
    // Configure TIM2 interrupt priority and enable
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);  // Highest priority
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    
    // Enable timer update interrupt
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
}

void TIMER_Start(void) {
    HAL_TIM_Base_Start_IT(&htim2);
}

void TIMER_Stop(void) {
    HAL_TIM_Base_Stop_IT(&htim2);
}

void TIMER_SetPeriod(uint32_t period_ms) {
    // Stop timer before changing period
    TIMER_Stop();
    
    // Update the period (ARR register)
    __HAL_TIM_SET_AUTORELOAD(&htim2, period_ms - 1);
    
    // Restart timer
    TIMER_Start();
}

// TIM2 interrupt handler
void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}
