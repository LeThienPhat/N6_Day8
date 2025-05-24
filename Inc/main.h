/*
 * main.h
 *
 *  Created on: May 20, 2025
 *      Author: PHAT
 */

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

// GPIO pin mapping
#define LED_BOARD_GREEN_PIN      GPIO_PIN_5
#define LED_BOARD_GREEN_PORT     GPIOA

#define LED_RGB_GREEN_PIN        GPIO_PIN_1
#define LED_RGB_BLUE_PIN         GPIO_PIN_2
#define LED_RGB_PORT             GPIOB

#define BUZZER_PIN               GPIO_PIN_10
#define BUZZER_PORT              GPIOB

#define LED1_BLUE_PIN            GPIO_PIN_3
#define LED1_BLUE_PORT           GPIOB

#define BUTTON_B3_PIN            GPIO_PIN_13
#define BUTTON_B3_PORT           GPIOC

#define BUTTON_B2_PIN            GPIO_PIN_0
#define BUTTON_B2_PORT           GPIOB

#endif
