//
// LED控制模块
// Created by L3AAF on 25-7-14.
// 功能：支持LED开关及多种闪烁模式
//

#include "led.h"

// LED硬件配置数组，每个元素对应一个LED的硬件参数和状态
// 结构：{GPIO端口, 引脚, 当前模式, 计数器}
static LED_Config leds[LED_COUNT] = {
    [LED1] = {LED_GPIO_Port, LED_Pin, LED_OFF, 0}, // LED1默认关闭，计数器归零
    // 可扩展更多LED，例如：
    // [LED2] = {GPIOB, GPIO_PIN_1, LED_OFF, 0},
};

// 闪烁模式配置表（单位：Tick数，具体时间取决于LED_Tick调用频率）
// 结构：{周期总Tick数, 高电平维持Tick数}
static const Mode_Config modes[BLINK_COUNT] = {
    [LED_BLINK_SLOW]  = {1000, 500}, // 慢闪：1000Tick周期，500Tick亮
    [LED_BLINK_SHORT] = {1000, 100}, // 短闪：1000Tick周期，100Tick亮
    [LED_BLINK_FAST]  = {100,  50},  // 快闪：100Tick周期，50Tick亮
};

/**
 * @brief 控制LED引脚电平（底层硬件操作）
 * @param led LED编号（LED1, LED2等）
 * @param state 目标状态（GPIO_PIN_SET=高电平亮，GPIO_PIN_RESET=低电平灭）
 */
static void LED_Write(LED_Type led, GPIO_PinState state)
{
    HAL_GPIO_WritePin(leds[led].port, leds[led].pin, state);
}

/**
 * @brief 开启指定LED
 * @param led LED编号
 */
static void led_on(LED_Type led)
{
    LED_Write(led, GPIO_PIN_SET); // 设置高电平
}

/**
 * @brief 关闭指定LED
 * @param led LED编号
 */
static void led_off(LED_Type led)
{
    LED_Write(led, GPIO_PIN_RESET); // 设置低电平
}

/**
 * @brief 设置LED工作模式
 * @param led LED编号
 * @param mode 模式枚举值（常亮/关闭/闪烁等）
 */
void LED_SetMode(LED_Type led, LED_Mode mode)
{
    if (led < LED_COUNT) // 有效性检查
    {
        leds[led].mode = mode;    // 更新模式
        leds[led].counter = 0;    // 重置计数器（用于闪烁模式同步）
    }
}

/**
 * @brief LED状态机（需周期性调用，如每1ms调用一次）
 * @note 实现功能：
 *       1. 处理常亮/关闭模式
 *       2. 管理多种闪烁模式的定时切换
 */
void LED_Tick(void)
{
    for (int i = 0; i < LED_COUNT; ++i) // 遍历所有LED
    {
        LED_Mode mode = leds[i].mode; // 获取当前模式

        switch (mode)
        {
            case LED_ON:  // 常亮模式
                led_on(i);
                break;
            case LED_OFF: // 关闭模式
                led_off(i);
                break;

            // 闪烁模式处理
            case LED_BLINK_SLOW:
            case LED_BLINK_FAST:
            case LED_BLINK_SHORT:
            {
                Mode_Config cfg = modes[mode]; // 获取当前模式的配置参数
                leds[i].counter = (leds[i].counter + 1) % cfg.period; // 循环计数

                // 根据计数值切换亮灭状态
                if (leds[i].counter < cfg.threshold)
                    led_on(i);  // 在阈值时间内保持亮
                else
                    led_off(i); // 超过阈值后灭
                break;
            }
            default: // 未知模式默认关闭
                led_off(i);
                break;
        }
    }
}