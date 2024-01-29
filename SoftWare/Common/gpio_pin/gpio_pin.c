
/*
 */

/* includes ----------------------------------------------------------------- */
#include <string.h>
#include "gpio_pin.h"
#include "stm32g4xx_hal.h"


/* private variables -------------------------------------------------------- */
static const GPIO_TypeDef *gpio_table[] =
{
    GPIOA, GPIOB, GPIOC, GPIOD,
};

/* private function prototype ----------------------------------------------- */
static bool _check_pin_name_valid(const char *name);
static void _translate_pin_name(const char *name, gpio_pin_data_t *data);

/* public functions --------------------------------------------------------- */

void gpio_set_pinmode(gpio_pin_t * const me, const char *name, enum pin_mode mode)
{
    _translate_pin_name(name, &me->data);
    me->mode = mode;

    /* Configure GPIO pin. */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (mode == PIN_MODE_INPUT)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    else if (mode == PIN_MODE_OUTPUT)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }

    GPIO_InitStruct.Pin = me->data.pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(me->data.gpio_x, &GPIO_InitStruct);
}

/**
  * @brief  EIO pin initialization.
  * @param  me      this pointer
  * @param  name    pin's name, just like "A.01" or "B.14" and so on.
  * @param  mode    pin's mode.
  * @retval None
  */
void gpio_pininit(gpio_pin_t* const me, const char *name, enum pin_mode mode)
{
    bool valid = _check_pin_name_valid(name);

    _translate_pin_name(name, &me->data);
    me->mode = mode;

    /* Configure GPIO pin. */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (mode == PIN_MODE_INPUT)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    else if (mode == PIN_MODE_OUTPUT)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
    }

    GPIO_InitStruct.Pin = me->data.pin;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(me->data.gpio_x, &GPIO_InitStruct);

    gpio_get_pinstatus(me);
}

/**
  * @brief  EIO pin's status getting function.
  * @param  me      this pointer
  * @retval The pin's status.
  */
bool gpio_get_pinstatus(gpio_pin_t * const me)
{
    GPIO_PinState status = HAL_GPIO_ReadPin(me->data.gpio_x, me->data.pin);
    me->status = (status == GPIO_PIN_SET) ? true : false;
    return me->status;
}

/**
  * @brief  EIO pin's status turning on function.
  * @param  me      this pointer
  * @param  status  the input pin status.
  * @retval None.
  */
void gpio_set_pinstatus(gpio_pin_t * const me, bool status)
{
    HAL_GPIO_WritePin(me->data.gpio_x,
                        me->data.pin,
                        status ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* private functions -------------------------------------------------------- */
/**
  * @brief  Check the given pin name is valid or not.
  * @param  name    The given pin name.
  * @retval None.
  */
static bool _check_pin_name_valid(const char *name)
{
    bool ret = true;
    uint8_t pin_number;

    if (!(strlen(name) == 4 && (name[1] == '.')))
    {
        ret = false;
        goto exit;
    }
    
    if (!((name[0] < 'A' && name[0] > 'D') &&
            (name[2] < '0' && name[2] > '1') &&
            (name[3] < '0' && name[3] > '9')))
    {
        ret = false;
        goto exit;
    }

    pin_number = (name[2] - '0') * 10 + (name[3] - '0');
    if (pin_number >= 16)
    {
        ret = false;
        goto exit;
    }

exit:
    return ret;
}

/**
  * @brief  Translate the pin name to MCU relate pin data structure.
  * @param  name    The given pin name.
  * @param  data    The pin data output.
  * @retval None.
  */
static void _translate_pin_name(const char *name, gpio_pin_data_t *data)
{
    data->gpio_x = (GPIO_TypeDef *)gpio_table[name[0] - 'A'];
    data->pin = (1 << ((uint8_t)((name[2] - '0') * 10 + (name[3] - '0'))));

    if (name[0] == 'A')
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (name[0] == 'B')
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (name[0] == 'C')
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if (name[0] == 'D')
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
}

/* ----------------------------- end of file -------------------------------- */
