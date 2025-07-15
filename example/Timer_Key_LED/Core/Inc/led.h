//
// Created by L3AAF on 25-7-14.
//

#ifndef LED_H
#define LED_H

#include "main.h"
#include "led.h"
#include "gpio.h"

// 枚举：LED编号
typedef enum {
    LED1 = 0,
    LED_COUNT  // 总LED数量，用于数组
} LED_Type;

// 枚举：LED状态/模式
typedef enum {
    LED_OFF = 0,        // 熄灭
    LED_ON,             // 常亮
    LED_BLINK_SLOW,     // 慢闪（1秒周期）
    LED_BLINK_FAST,     // 快闪（0.1秒周期）
    LED_BLINK_SHORT,    // 短闪（亮0.1秒）
    BLINK_COUNT
} LED_Mode;

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
    LED_Mode mode;
    uint32_t counter;
}LED_Config;

typedef struct
{
    uint32_t period;
    uint32_t threshold;
}Mode_Config;


void LED_SetMode(LED_Type led, LED_Mode mode);
void LED_Tick(void);

#endif //LED_H
