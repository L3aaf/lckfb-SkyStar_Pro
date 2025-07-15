//
// 按键驱动模块
// Created by L3AAF on 25-7-14.
// 修改说明：修正了电平检测逻辑，现为高电平有效（按下=SET）
//

#include "key.h"

// 按键硬件配置数组，每个元素存储对应按键的GPIO端口和引脚
// 示例：KEY1使用KEY_GPIO_Port和KEY_Pin
static Key_Config key_config[KEY_COUNT] = {
    [KEY1] = {KEY_GPIO_Port, KEY_Pin}, // KEY1的硬件配置
    // 如需添加更多按键，格式为：[KEY2] = {GPIOx, GPIO_PIN_x}
};

// 存储最近触发的按键编号，初始化为无效值（KEY_COUNT）
// 注意：此变量会在按键释放时被更新，通过KeyGetNum()读取后自动清除
static KEY_Type key_num = KEY_COUNT;

/**
 * @brief 检测当前被按下的按键
 * @return 按下的按键编号（KEY1等），若无按键按下返回KEY_COUNT
 * @note 高电平有效（GPIO_PIN_SET表示按下）
 */
static KEY_Type Key_GetState(void)
{
    // 遍历所有已配置的按键
    for (KEY_Type i = KEY1; i < KEY_COUNT; i++)
    {
        // 检测按键引脚是否为高电平（按下状态）
        if (HAL_GPIO_ReadPin(key_config[i].port, key_config[i].pin) == GPIO_PIN_SET)
        {
            return i; // 返回按下的按键编号
        }
    }
    return KEY_COUNT; // 无按键按下
}

/**
 * @brief 获取被触发的按键编号（单次触发）
 * @return 触发按键的编号，若无触发返回KEY_COUNT
 * @note 调用后会清除内部存储的键值，避免重复触发
 */
KEY_Type KeyGetNum(void)
{
    // 检查是否有有效按键触发
    if (key_num < KEY_COUNT)
    {
        KEY_Type temp = key_num; // 临时保存键值
        key_num = KEY_COUNT;     // 清除键值（单次触发）
        return temp;            // 返回按键编号
    }
    return KEY_COUNT; // 无按键触发
}

/**
 * @brief 按键状态检测函数（需定时器周期性调用）
 * @note 实现功能：
 *       1. 按键消抖（通过KEY_DEBOUNCE_TICKS控制）
 *       2. 释放事件检测（从按下到释放的跳变）
 */
void Key_Tick(void)
{
    static uint8_t counter;              // 消抖计数器
    static KEY_Type CurrKeyState = KEY_COUNT; // 当前按键状态
    static KEY_Type PrevKeyState = KEY_COUNT; // 上一次按键状态

    counter++; // 计数器递增（每次调用+1）

    // 达到消抖时间阈值时检测按键状态
    if (counter >= KEY_DEBOUNCE_TICKS)
    {
        counter = 0; // 重置计数器

        // 更新状态记录
        PrevKeyState = CurrKeyState;     // 保存旧状态
        CurrKeyState = Key_GetState();   // 获取新状态

        // 检测按键释放事件（之前按下，现在释放）
        if (CurrKeyState == KEY_COUNT && PrevKeyState != KEY_COUNT)
        {
            key_num = PrevKeyState; // 记录被释放的按键编号
        }
    }
}