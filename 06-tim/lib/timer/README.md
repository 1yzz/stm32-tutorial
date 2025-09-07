# Timer Library

This library provides basic timer functionality using STM32F103 internal clock and interrupt events.

## Features

- Timer initialization and configuration
- Interrupt-based timer events
- Configurable timer period
- Start/stop timer functionality

## Configuration

- **Timer Instance**: TIM2
- **Default Period**: 1000ms (1 second)
- **Clock Source**: Internal system clock (72MHz)
- **Prescaler**: 7199 (for 1ms resolution)

## Usage

```c
#include "timer.h"

int main(void) {
    HAL_Init();
    TIMER_Init();
    TIMER_Start();
    
    while (1) {
        // Timer interrupt will call HAL_TIM_PeriodElapsedCallback
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // Handle timer interrupt here
    }
}
```

## Functions

- `TIMER_Init()`: Initialize timer with default settings
- `TIMER_Start()`: Start timer with interrupt
- `TIMER_Stop()`: Stop timer
- `TIMER_SetPeriod(uint32_t period_ms)`: Change timer period
