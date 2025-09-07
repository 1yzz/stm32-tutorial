#include "oled.h"
#include "font8x16.h"
#include <string.h>

// I2C handle definition
I2C_HandleTypeDef hi2c1;

// Static function prototypes
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void Error_Handler(void);

// 字体库现在在 font8x16.h 中定义

// Compute a checkerboard mask for dithering based on x parity
static inline uint8_t oled_dither_mask(uint8_t xParity)
{
    // 0x55 = 01010101b, 0xAA = 10101010b
    return xParity ? 0x55u : 0xAAu;
}

// Apply simple edge-based dithering to soften jagged edges for 1bpp fonts
static void oled_apply_edge_dither(const uint8_t src[8], uint8_t dst[8], uint8_t xStart)
{
    for (uint8_t i = 0; i < 8; i++) {
        const uint8_t left  = (i > 0)     ? src[i - 1] : 0u;
        const uint8_t col   = src[i];
        const uint8_t right = (i < 7)     ? src[i + 1] : 0u;

        // Horizontal edges: where current pixel is 0 but neighbors have 1s
        uint8_t horizEdges = (uint8_t)(~col) & (uint8_t)(left | right);

        // Vertical edges within this column: detect isolated gaps between set bits
        uint8_t verticalNeighbors = (uint8_t)((col << 1) | (col >> 1));
        uint8_t vertEdges = (uint8_t)(~col) & verticalNeighbors;

        // Combine and apply checkerboard dithering to avoid thickening too much
        uint8_t pattern = oled_dither_mask((uint8_t)((xStart + i) & 0x01u));
        uint8_t add = (uint8_t)((horizEdges | vertEdges) & pattern);

        dst[i] = (uint8_t)(col | add);
    }
}

void OLED_Write_Command(uint8_t cmd)
{
    uint8_t data[2];
    data[0] = 0x00; // Co = 0, D/C# = 0
    data[1] = cmd;
    HAL_I2C_Master_Transmit(&hi2c1, OLED_I2C_ADDR, data, 2, HAL_MAX_DELAY);
}

void OLED_Write_Data(uint8_t data_byte)
{
    uint8_t data[2];
    data[0] = 0x40; // Co = 0, D/C# = 1
    data[1] = data_byte;
    HAL_I2C_Master_Transmit(&hi2c1, OLED_I2C_ADDR, data, 2, HAL_MAX_DELAY);
}

// 批量数据写入函数 - 一次发送多个数据字节
static void OLED_Write_DataStream(const uint8_t *bytes, uint16_t length)
{
    if (bytes == NULL || length == 0) {
        return;
    }
    if (length > OLED_WIDTH) {
        length = OLED_WIDTH;
    }

    uint8_t txBuffer[1 + OLED_WIDTH];
    txBuffer[0] = 0x40; // 数据流控制字节
    memcpy(&txBuffer[1], bytes, length);
    HAL_I2C_Master_Transmit(&hi2c1, OLED_I2C_ADDR, txBuffer, (uint16_t)(length + 1), HAL_MAX_DELAY);
}

void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    // SSD1315 列地址设置
    OLED_Write_Command(0xB0 + y);                    // 设置页地址
    OLED_Write_Command(((x & 0xF0) >> 4) | 0x10);    // 设置列地址高4位
    OLED_Write_Command((x & 0x0F) | 0x01);           // 设置列地址低4位
}

void OLED_Display_On(void)
{
    OLED_Write_Command(0x8D); // Charge Pump Setting
    OLED_Write_Command(0x14); // Enable Charge Pump
    OLED_Write_Command(0xAF); // Display ON
}

void OLED_Display_Off(void)
{
    OLED_Write_Command(0x8D); // Charge Pump Setting
    OLED_Write_Command(0x10); // Disable Charge Pump
    OLED_Write_Command(0xAE); // Display OFF
}

void OLED_Clear(void)
{
    uint8_t i, j;
    for (i = 0; i < 8; i++)
    {
        OLED_Write_Command(0xB0 + i); // Set page address
        OLED_Write_Command(0x00);     // Set lower column address
        OLED_Write_Command(0x10);     // Set higher column address
        for (j = 0; j < OLED_WIDTH; j++)
        {
            OLED_Write_Data(0x00);
        }
    }
}

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t page, bit;
    page = y / 8;
    bit = y % 8;
    OLED_Set_Pos(x, page);
    if (t)
        OLED_Write_Data(1 << bit);
    else
        OLED_Write_Data(0x00);
}

void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t size)
{
    uint8_t i;
    
    // 检查字符是否在支持的范围内 (0x20-0x7E)
    if (chr < FONT_START_CHAR || chr > FONT_END_CHAR) {
        chr = ' '; // 超出范围显示为空格
    }
    
    if (size == 16) {
        // 使用新的字体库直接索引
        const uint8_t *font_data = GET_FONT_DATA(chr);
        
        // Draw 8x16 character using two batched transfers (top and bottom 8 bytes)
        uint8_t top[8];
        uint8_t bottom[8];
        for (i = 0; i < 8; i++) {
            top[i] = font_data[i];
            bottom[i] = font_data[i + 8];
        }
        OLED_Set_Pos(x, y / 8);
        OLED_Write_DataStream(top, 8);
        OLED_Set_Pos(x, y / 8 + 1);
        OLED_Write_DataStream(bottom, 8);
    }
}

// Anti-aliased version using edge dithering to visually smooth 8x16 glyphs on 1bpp OLED
void OLED_ShowChar_AA(uint8_t x, uint8_t y, char chr, uint8_t size)
{
    if (size != 16) {
        // Fallback to normal rendering for unsupported sizes
        OLED_ShowChar(x, y, chr, size);
        return;
    }

    // 检查字符是否在支持的范围内 (0x20-0x7E)
    if (chr < FONT_START_CHAR || chr > FONT_END_CHAR) {
        chr = ' '; // 超出范围显示为空格
    }
    
    // 使用新的字体库直接索引
    const uint8_t *font_data = GET_FONT_DATA(chr);

    uint8_t top[8];
    uint8_t bottom[8];
    for (uint8_t i = 0; i < 8; i++) {
        top[i] = font_data[i];
        bottom[i] = font_data[i + 8];
    }

    uint8_t topAA[8];
    uint8_t bottomAA[8];
    oled_apply_edge_dither(top, topAA, x);
    oled_apply_edge_dither(bottom, bottomAA, x);

    OLED_Set_Pos(x, y / 8);
    OLED_Write_DataStream(topAA, 8);
    OLED_Set_Pos(x, y / 8 + 1);
    OLED_Write_DataStream(bottomAA, 8);
}

void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size)
{
    while (*str)
    {
        OLED_ShowChar(x, y, *str, size);
        uint8_t advance = (size == 16) ? 8 : (uint8_t)(size / 2);
        x += advance;
        if (x > (OLED_WIDTH - advance))
        {
            x = 0;
            y += size;
        }
        str++;
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    char buf[11];
    int i;
    for (i = len - 1; i >= 0; i--)
    {
        buf[i] = (num % 10) + '0';
        num /= 10;
    }
    buf[len] = '\0';
    OLED_ShowString(x, y, buf, size);
}

void OLED_Init(void)
{
    HAL_Delay(100);

    // SSD1315 初始化序列
    OLED_Write_Command(0xAE); // Display off
    OLED_Write_Command(0x20); // Set Memory Addressing Mode
    OLED_Write_Command(0x10); // Page Addressing Mode
    OLED_Write_Command(0xB0); // Set Page Start Address for Page Addressing Mode,0-7
    OLED_Write_Command(0xC8); // Set COM Output Scan Direction
    OLED_Write_Command(0x00); // Set lower column address
    OLED_Write_Command(0x10); // Set higher column address
    OLED_Write_Command(0x40); // Set start line address
    OLED_Write_Command(0x81); // Set contrast control register
    OLED_Write_Command(0x7F); // Set contrast value
    OLED_Write_Command(0xA1); // Set segment re-map 0 to 127
    OLED_Write_Command(0xA6); // Set normal display
    OLED_Write_Command(0xA8); // Set multiplex ratio(1 to 64)
    OLED_Write_Command(0x3F); // 1/64 duty
    OLED_Write_Command(0xA4); // Output follows RAM content
    OLED_Write_Command(0xD3); // Set display offset
    OLED_Write_Command(0x00); // Not offset
    OLED_Write_Command(0xD5); // Set display clock divide ratio/oscillator frequency
    OLED_Write_Command(0xF0); // Set divide ratio
    OLED_Write_Command(0xD9); // Set pre-charge period
    OLED_Write_Command(0x22); // Set pre-charge period
    OLED_Write_Command(0xDA); // Set com pins hardware configuration
    OLED_Write_Command(0x12); // Alternative COM pin configuration
    OLED_Write_Command(0xDB); // Set vcomh
    OLED_Write_Command(0x20); // 0.77*VCC
    OLED_Write_Command(0x8D); // Set DC-DC enable
    OLED_Write_Command(0x14); // Enable charge pump
    OLED_Write_Command(0xAF); // Turn on OLED panel

    OLED_Clear();
}

// ==================== 封装的系统初始化函数 ====================

/**
 * @brief OLED系统初始化函数（不包含HAL_Init）
 * @note 包含时钟配置、GPIO和I2C初始化、OLED初始化
 */
void OLED_System_Init(void)
{
    // 1. 配置系统时钟
    SystemClock_Config();
    
    // 2. 初始化GPIO
    MX_GPIO_Init();
    
    // 3. 初始化I2C
    MX_I2C1_Init();
    
    // 4. 初始化OLED显示屏
    OLED_Init();
}

/**
 * @brief 系统时钟配置
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** 初始化CPU、AHB和APB总线时钟 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    
    /** 初始化CPU、AHB和APB总线时钟 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief I2C1初始化函数
 */
static void MX_I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 400000;        // 400kHz
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief GPIO初始化函数
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO端口时钟使能 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

/**
 * @brief 错误处理函数
 */
static void Error_Handler(void)
{
    /* 错误处理：可以在这里添加LED指示或其他错误处理 */
    while(1) 
    {
        // 无限循环，等待复位
    }
}

/**
 * @brief I2C MSP初始化回调函数
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hi2c->Instance==I2C1)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO配置 - 标准硬件引脚
        PB6     ------> I2C1_SCL (时钟线)
        PB7     ------> I2C1_SDA (数据线)
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* 外设时钟使能 */
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}