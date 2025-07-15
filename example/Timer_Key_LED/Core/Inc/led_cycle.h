//
// Created by L3AAF on 25-7-15.
//

#ifndef LED_CYCLE_H
#define LED_CYCLE_H

#include "led.h"
#include <stdbool.h>

typedef struct {
    LED_Type type;
    uint8_t currentIndex;
} LED_Control;

bool modeSequence_Setter(LED_Mode *newSequence, size_t size);
void led_cycle_init(LED_Type led);
void led_cycle_next(LED_Type led);
void led_cycle_all_init(void);

#endif //LED_CYCLE_H
