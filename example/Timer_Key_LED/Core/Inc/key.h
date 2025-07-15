//
// Created by L3AAF on 25-7-14.
//

#ifndef KEY_H
#define KEY_H
#define KEY_DEBOUNCE_TICKS 20 // 允许用户修改

#include "key.h"
#include "main.h"
#include "gpio.h"

// 枚举：KEY编号
typedef enum
{
    KEY1 = 0,
    // 这里添加你的key编号，示例：KEY3....
    KEY_COUNT // 总KEY数量，用于数组
} KEY_Type;

// 按键配置结构体
typedef struct
{
    GPIO_TypeDef *port; // GPIO端口（GPIOx）
    uint16_t pin;       // 引脚号（如GPIO_PIN_x）
} Key_Config;

KEY_Type KeyGetNum(void);
void Key_Tick(void);

#endif //KEY_H
