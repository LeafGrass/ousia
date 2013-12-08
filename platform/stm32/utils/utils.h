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
 * @file    platform/stm32/utils/utils.h
 * @brief   stm32 library utilities
 * @log     2011.8 initial revision
 */

#ifndef __STM32_UTILS_H__
#define __STM32_UTILS_H__

#define SYSTICK_RELOAD_VAL	71999	/* takes a cycle to reload */
#define SERIAL_BAUDRATE		9600

#if (USART_CONSOLE_BANK == USART1)
#define USART_CONSOLE_PORT	GPIOA
#define USART_CONSOLE_TX	9
#define USART_CONSOLE_RX	10
#elif (USART_CONSOLE_BANK == USART2)
#define USART_CONSOLE_PORT	GPIOA
#define USART_CONSOLE_TX	2
#define USART_CONSOLE_RX	3
#elif (USART_CONSOLE_BANK == USART3)
#define USART_CONSOLE_PORT	GPIOB
#define USART_CONSOLE_TX	10
#define USART_CONSOLE_RX	11
#else
#error "You must specify a USART_CONSOLE_BANK."
#endif

#define VCOM_ID_VENDOR		0x1EAF
#define RESET_DELAY		(100000)
#define USB_CONFIG_MAX_POWER	(100 >> 1)

#define GPIO_MEMLCD_EXTMODE	15
#define GPIO_MEMLCD_DISP	14
#define GPIO_MEMLCD_EXTCOMIN	13
#define GPIO_MEMLCD_CS		15

void utils_board_init(void);
void utils_io_putc(void *p, char ch);
char utils_io_getc(void *p);
void utils_usb_putc(void *p, char ch);
char utils_usb_getc(void *p);

#endif /* __STM32_UTILS_H__ */
