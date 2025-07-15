//
// Created by L3AAF on 25-7-15.
//

#include "led_cycle.h"

// 计算数组长度的宏
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// 默认的LED模式序列（从关到多种闪烁模式）
static LED_Mode modeSequence[] = {
    LED_OFF,          // 0: 关闭
    LED_ON,           // 1: 常亮
    LED_BLINK_SLOW,   // 2: 慢闪
    LED_BLINK_FAST,   // 3: 快闪
    LED_BLINK_SHORT   // 4: 短促闪烁
};

// LED控制结构体数组（初始化LED1，索引为0）
static LED_Control led_control[] = {
    [LED1] = {LED1, 0}, // 初始化LED1，默认索引0（对应LED_OFF）
};

// 全局指针和大小，指向当前模式序列
static LED_Mode *modeSequencePtr = modeSequence; // 默认指向初始序列
static size_t modeSequenceSize = ARRAY_SIZE(modeSequence); // 默认序列长度

/**
 * @brief 设置新的LED模式序列
 * @param newSequence 新的模式数组指针
 * @param size 新数组的长度
 * @return 成功返回true，失败（参数无效）返回false
 */
bool modeSequence_Setter(LED_Mode *newSequence, size_t size) {
    if (newSequence == NULL || size == 0) return false;

    modeSequencePtr = newSequence;   // 更新模式序列指针
    modeSequenceSize = size;        // 更新序列长度
    return true;
}

/**
 * @brief 初始化单个LED的循环模式
 * @param led LED类型（如LED1）
 */
void led_cycle_init(LED_Type led) {
    // 检查LED类型是否超出数组范围
    if(led >= ARRAY_SIZE(led_control)) return;

    // 初始化LED的控制结构体
    led_control[led].type = led;               // 设置LED类型
    led_control[led].currentIndex = 0;         // 重置索引为0（从第一个模式开始）
    LED_SetMode(led, modeSequencePtr[0]);       // 设置LED为序列中的第一个模式（如LED_OFF）
}

/**
 * @brief 初始化所有LED的循环模式
 */
void led_cycle_all_init(void) {
    for (size_t i = 0; i < ARRAY_SIZE(led_control); ++i) {
        led_cycle_init(led_control[i].type);   // 逐个初始化LED
    }
}

/**
 * @brief 切换到LED的下一个模式（循环）
 * @param led LED类型（如LED1）
 */
void led_cycle_next(LED_Type led) {
    // 检查LED类型是否有效
    if(led >= ARRAY_SIZE(led_control)) return;

    // 计算下一个索引（循环递增）
    led_control[led].currentIndex =
        (led_control[led].currentIndex + 1) % modeSequenceSize;

    // 设置LED为新的模式
    LED_SetMode(led, modeSequencePtr[led_control[led].currentIndex]);
}