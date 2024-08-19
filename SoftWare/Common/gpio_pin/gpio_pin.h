/*
 * eLesson Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef GPIO_PIN_H
#define GPIO_PIN_H

/* includes ----------------------------------------------------------------- */
#include <stdbool.h>
#include "stm32g4xx_hal.h"

/* public define ------------------------------------------------------------ */
enum pin_mode
{
    PIN_MODE_INPUT = 0,
    PIN_MODE_INPUT_PULLUP,
    PIN_MODE_INPUT_PULLDOWN,
    PIN_MODE_OUTPUT,
    PIN_MODE_OUTPUT_OD,
    PIN_MODE_MAX
};

/* public typedef ----------------------------------------------------------- */
typedef struct gpio_pin_data
{
    GPIO_TypeDef *gpio_x;
    uint16_t pin;
} gpio_pin_data_t;

typedef struct gpio_pin
{
    /* private */
    gpio_pin_data_t data;
    enum pin_mode mode;
    bool status;
} gpio_pin_t;

/* public functions --------------------------------------------------------- */
void gpio_pininit(gpio_pin_t * const me, const char *name, enum pin_mode mode);
bool gpio_get_pinstatus(gpio_pin_t * const me);
void gpio_set_pinstatus(gpio_pin_t * const me, bool status);
void gpio_set_pinmode(gpio_pin_t * const me, const char *name, enum pin_mode mode);

#endif

/* ----------------------------- end of file -------------------------------- */
