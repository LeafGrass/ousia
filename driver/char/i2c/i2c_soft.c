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

#define I2C_SOFT_EEPROM_ADDR	0xA0
#define I2C_SOFT_EEPROM_CMD_W	0x00
#define I2C_SOFT_EEPROM_CMD_R	0x01

typedef gpio_dev i2c_soft_io_port;

struct i2c_soft_port_t {
	i2c_soft_io_port *dev;
	uint8 pin;
};

struct i2c_soft_io_t {
	i2c_soft_io_port *dev;
	uint8 pin;
};

static struct i2c_soft_io_t __scl = {
	.dev = &gpiob,
	.pin = 6
};

static struct i2c_soft_io_t __sda = {
	.dev = &gpiob,
	.pin = 7
};

#define __sda_set(val) \
do { \
	gpio_write_bit(__sda.dev, __sda.pin, val); \
} while (0)

#define __scl_set(val) \
do { \
	gpio_write_bit(__scl.dev, __scl.pin, val); \
} while (0)

#define __io_out(dev, pin) \
do { \
	gpio_set_mode(dev, pin, GPIO_OUTPUT_PP); \
} while (0)

#define __io_in(dev, pin) \
do { \
	gpio_set_mode(dev, pin, GPIO_INPUT_PU); \
} while (0)

uint8 __soft_slave_ack(void);
void __soft_write_byte(uint8 data);
uint8 __soft_read_byte(void);
static void __soft_start(void);
static void __soft_stop(void);

/*
 * @brief   i2c start condition
 * @param   a -i- an integer
 *          b -i- another integer
 * @return  none
 */
void i2c_soft_init(void)
{
	__io_out(__scl.dev, __scl.pin);
	__io_out(__sda.dev, __sda.pin);
}

int8 i2c_soft_eeprom_write(uint8 addr, uint8 * pbuf, uint32 nb)
{
	uint32 i = 0;

	if (pbuf == NULL)
		return I2C_SOFT_RET_ERR;

	__soft_start();
	__soft_write_byte(I2C_SOFT_EEPROM_ADDR | I2C_SOFT_EEPROM_CMD_W);
	if (__soft_slave_ack() == 1)
		return I2C_SOFT_RET_ENACK;
	__soft_write_byte(addr);
	if (__soft_slave_ack() == 1)
		return I2C_SOFT_RET_ENACK;
	for (i = 0; i < nb; i++) {
		__soft_write_byte(*pbuf++);
		if (__soft_slave_ack() == 1)
			return I2C_SOFT_RET_ENACK;
	}
	__soft_stop();

	return I2C_SOFT_RET_OK;
}

int8 i2c_soft_eeprom_read(uint8 addr, uint8 * pbuf, uint32 nb)
{
	uint32 i = 0;

	if (pbuf == NULL)
		return I2C_SOFT_RET_ERR;

	__soft_start();
	__soft_write_byte(I2C_SOFT_EEPROM_ADDR | I2C_SOFT_EEPROM_CMD_W);
	if (__soft_slave_ack() == 1)
		return I2C_SOFT_RET_ENACK;
	__soft_write_byte(addr);
	if (__soft_slave_ack() == 1)
		return I2C_SOFT_RET_ENACK;
	__soft_start();
	__soft_write_byte(I2C_SOFT_EEPROM_ADDR | I2C_SOFT_EEPROM_CMD_R);
	if (__soft_slave_ack() == 1)
		return I2C_SOFT_RET_ENACK;
	for (i = 0; i < nb; i++)
		*pbuf++ = __soft_read_byte();
	__soft_stop();

	return I2C_SOFT_RET_OK;
}

uint8 __soft_slave_ack(void)
{
	uint32 timeout = 0;
	uint8 ret = 0;

	__io_in(__sda.dev, __sda.pin);

	__scl_set(1);
	timeout = 10000;
	while (timeout-- > 0) {
		if (gpio_read_bit(__sda.dev, __sda.pin) == 1) {
			ret = 1;
			break;
		} else
			ret = 0;
	}
	__scl_set(0);

	__io_out(__sda.dev, __sda.pin);
	return ret;
}

void __soft_write_byte(uint8 data)
{
	uint8 i = 0;

	for (i = 0; i < 8; i++) {
		if (data & 0x80)
			__sda_set(1);
		else
			__sda_set(0);
		data <<= 1;
		delay_us(1000);
		__scl_set(1);
		delay_us(1000);
		__scl_set(0);
		delay_us(1000);
	}
}

uint8 __soft_read_byte(void)
{
	uint8 i = 0;
	uint8 rdval = 0;
	uint8 bit = 0;

	__io_in(__sda.dev, __sda.pin);

	for (i = 0; i < 8; i++) {
		__scl_set(1);
		delay_us(1000);
		if (gpio_read_bit(__sda.dev, __sda.pin) == 1)
			bit = 0x01;
		else
			bit = 0x00;
		rdval = (rdval << 1) | bit;
		__scl_set(0);
		delay_us(1000);
	}

	__io_out(__sda.dev, __sda.pin);
	return rdval;
}

/*
 * @brief   i2c start condition
 * @param   a -i- an integer
 *          b -i- another integer
 * @return  none
 */
static void __soft_start(void)
{
	__sda_set(1);
	delay_us(1000);
	__scl_set(1);
	delay_us(1000);
	__sda_set(0);
	delay_us(1000);
	__scl_set(0);
}

static void __soft_stop(void)
{
	__sda_set(0);
	delay_us(1000);
	__scl_set(1);
	delay_us(1000);
	__sda_set(1);
	delay_us(1000);
	__scl_set(0);
}
