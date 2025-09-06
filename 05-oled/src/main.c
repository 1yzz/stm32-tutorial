#include "stm32f1xx_hal.h"
#include "oled.h"

int main(void)
{
    // 一键初始化所有系统组件
    OLED_System_Init();
    
    // 显示测试内容 - 测试完整字体库
    OLED_Clear();
    OLED_ShowString(0, 0, "Hello World!", 16);
    OLED_ShowString(0, 16, "STM32 + OLED", 16);
    OLED_ShowString(0, 32, "ASCII: !\"#$%&'()", 16);
    OLED_ShowString(0, 48, "0123456789", 16);
    
    while (1)
    {
        // 主循环
        HAL_Delay(1000);
        
        // 这里可以添加其他应用逻辑
        // 例如：更新显示内容、处理用户输入等
    }
}