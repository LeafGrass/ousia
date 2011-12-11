/*
 * Copyright (c) 2011-2012 LeafGrass <leafgrass.g@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */

/*
 * @file    driver/char/i2c/i2c_soft.c
 * @brief   gpio operations as software implemented i2c bus
 * @log     2011.12 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <stm32/libmaple/gpio.h>
#include <stm32/libmaple/delay.h>

#include <char/i2c/i2c_soft.h>


typedef gpio_dev i2c_soft_io_port;

struct i2c_soft_port_t {
	i2c_soft_io_port *dev;
	uint8 pin;
};

struct i2c_soft_io_t {
	i2c_soft_io_port *dev;
	uint8 pin;
};

static void i2c_soft_start(void);

static struct i2c_soft_io_t _scl =
{
	.dev = &gpiob,
	.pin = 6
};

static struct i2c_soft_io_t _sda =
{
	.dev = &gpiob,
	.pin = 7
};

#define sda_set(val) \
do { \
	gpio_write_bit(_sda.dev, _sda.pin, val); \
} while (0)

#define scl_set(val) \
do { \
	gpio_write_bit(_scl.dev, _scl.pin, val); \
} while (0)

#define i2c_soft_io_out(dev, pin) \
do { \
	gpio_set_mode(dev, pin, GPIO_OUTPUT_PP); \
} while (0)

#define i2c_soft_io_in(dev, pin) \
do { \
	gpio_set_mode(dev, pin, GPIO_INPUT_PU); \
} while (0)


/*
 * @brief   i2c start condition
 * @param   a -i- an integer
 *          b -i- another integer
 * @return  none
 */
void i2c_soft_init(void)
{
	i2c_soft_io_out(_scl.dev, _scl.pin);
	i2c_soft_io_out(_sda.dev, _sda.pin);
	i2c_soft_start();
}

/*
 * @brief   i2c start condition
 * @param   a -i- an integer
 *          b -i- another integer
 * @return  none
 */
static void i2c_soft_start(void)
{
	sda_set(1);
	delay_us(1000);
	scl_set(1);
	delay_us(1000);
	sda_set(0);
	delay_us(1000);
	scl_set(0);
}
