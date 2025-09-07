#ifndef __FONT8X16_H
#define __FONT8X16_H

#include <stdint.h>

// 8x16 ASCII字体库 (0x20-0x7E)
// 每个字符16字节，按行存储
extern const uint8_t font8x16[][16];

// 字体库大小定义
#define FONT_WIDTH  8
#define FONT_HEIGHT 16
#define FONT_START_CHAR 0x20  // 空格
#define FONT_END_CHAR   0x7E  // ~
#define FONT_COUNT      (FONT_END_CHAR - FONT_START_CHAR + 1)

// 获取字符字体数据的宏
#define GET_FONT_DATA(chr) font8x16[(chr) - FONT_START_CHAR]

#endif
