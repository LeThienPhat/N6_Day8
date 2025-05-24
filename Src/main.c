#include <stdint.h>
#include <stdbool.h>
#include "stm32f401re_gpio.h"
#include "stm32f401re_rcc.h"

#define BTN_PRESS               0
#define GPIO_PIN_SET            1
#define GPIO_PIN_RESET          0

// Buzzer
#define BUZZER_PORT             GPIOC
#define BUZZER_PIN              GPIO_Pin_9
#define BUZZER_CLK              RCC_AHB1Periph_GPIOC

// Buttons
#define BUTTON_B2_PORT          GPIOB
#define BUTTON_B2_PIN           GPIO_Pin_3
#define B2_CLK                  RCC_AHB1Periph_GPIOB

#define BUTTON_B3_PORT          GPIOA
#define BUTTON_B3_PIN           GPIO_Pin_4
#define B3_CLK                  RCC_AHB1Periph_GPIOA

// LEDs
#define LED_ONBOARD_GREEN_PORT  GPIOA
#define LED_ONBOARD_GREEN_PIN   GPIO_Pin_5

#define LED_RGB_GREEN_PORT      GPIOA
#define LED_RGB_GREEN_PIN       GPIO_Pin_0

#define LED_BLUE_1_PORT         GPIOA
#define LED_BLUE_1_PIN          GPIO_Pin_3

#define LED_BLUE_2_PORT         GPIOA
#define LED_BLUE_2_PIN          GPIO_Pin_10

static void Buzzer_Init(void);
static void Button_Init(void);
static void LEDs_Init(void);
static void SetPin(GPIO_TypeDef* port, uint16_t pin, uint8_t state);
static uint8_t ReadPin(GPIO_TypeDef* port, uint16_t pin);
static void Delay(volatile uint32_t count);

int main(void) {
    Buzzer_Init();
    Button_Init();
    LEDs_Init();

    // Nháy LED onboard khi khởi động
    SetPin(LED_ONBOARD_GREEN_PORT, LED_ONBOARD_GREEN_PIN, GPIO_PIN_SET);
    Delay(500000);
    SetPin(LED_ONBOARD_GREEN_PORT, LED_ONBOARD_GREEN_PIN, GPIO_PIN_RESET);
    Delay(200000);

    bool blue1_on = false;
    bool b2_pressed_flag = false;
    bool b3_pressed_flag = false;

    while (1) {
        // B3: Nháy LED_RGB_GREEN + LED_BLUE_2 + buzzer
        if (ReadPin(BUTTON_B3_PORT, BUTTON_B3_PIN) == BTN_PRESS) {
            if (!b3_pressed_flag) {
                b3_pressed_flag = true;

                SetPin(LED_RGB_GREEN_PORT, LED_RGB_GREEN_PIN, GPIO_PIN_SET);
                SetPin(LED_BLUE_2_PORT, LED_BLUE_2_PIN, GPIO_PIN_SET);
                SetPin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);

                Delay(500000);

                SetPin(LED_RGB_GREEN_PORT, LED_RGB_GREEN_PIN, GPIO_PIN_RESET);
                SetPin(LED_BLUE_2_PORT, LED_BLUE_2_PIN, GPIO_PIN_RESET);
                SetPin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
            }
        } else {
            b3_pressed_flag = false;
        }

        // B2: bật/tắt LED_BLUE_1
        if (ReadPin(BUTTON_B2_PORT, BUTTON_B2_PIN) == BTN_PRESS) {
            if (!b2_pressed_flag) {
                b2_pressed_flag = true;
                blue1_on = !blue1_on;
                SetPin(LED_BLUE_1_PORT, LED_BLUE_1_PIN, blue1_on ? GPIO_PIN_SET : GPIO_PIN_RESET);
            }
        } else {
            b2_pressed_flag = false;
        }

        Delay(10000); // Chống dội nút
    }
}

static void Buzzer_Init(void) {
    GPIO_InitTypeDef gpio;
    RCC_AHB1PeriphClockCmd(BUZZER_CLK, ENABLE);

    gpio.GPIO_Pin = BUZZER_PIN;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(BUZZER_PORT, &gpio);
}

static void Button_Init(void) {
    GPIO_InitTypeDef gpio;

    // B2
    RCC_AHB1PeriphClockCmd(B2_CLK, ENABLE);
    gpio.GPIO_Pin = BUTTON_B2_PIN;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(BUTTON_B2_PORT, &gpio);

    // B3
    RCC_AHB1PeriphClockCmd(B3_CLK, ENABLE);
    gpio.GPIO_Pin = BUTTON_B3_PIN;
    gpio.GPIO_Mode = GPIO_Mode_IN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(BUTTON_B3_PORT, &gpio);
}

static void LEDs_Init(void) {
    GPIO_InitTypeDef gpio;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    gpio.GPIO_Pin = LED_ONBOARD_GREEN_PIN | LED_RGB_GREEN_PIN | LED_BLUE_1_PIN | LED_BLUE_2_PIN;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio);
}

static void SetPin(GPIO_TypeDef* port, uint16_t pin, uint8_t state) {
    if (state == GPIO_PIN_SET) port->BSRRL = pin;
    else                       port->BSRRH = pin;
}

static uint8_t ReadPin(GPIO_TypeDef* port, uint16_t pin) {
    return ((port->IDR & pin) == 0) ? BTN_PRESS : 1;
}

static void Delay(volatile uint32_t count) {
    while (count--) __NOP();
}
