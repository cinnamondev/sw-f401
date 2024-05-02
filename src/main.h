//
// Created by cinnamon on 27/04/2024.
//

#ifndef SW_F401_MAIN_H
#define SW_F401_MAIN_H

#include "mbed.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart2;

void Error_Handler(void);

EventQueue* get_event_queue(void);


#endif // SW_F401_MAIN_H
