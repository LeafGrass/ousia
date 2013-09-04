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
 * @file    platform/stm32/utils/utils.c
 * @brief   stm32 library utilities
 * @log     2011.8 initial revision
 */

#include "libmaple/libmaple.h"
#include "libmaple/libmaple_types.h"
#include "libmaple/flash.h"
#include "libmaple/rcc.h"
#include "series/rcc.h"
#include "libmaple/nvic.h"
#include "libmaple/systick.h"
#include "libmaple/gpio.h"
#include "libmaple/adc.h"
#include "libmaple/timer.h"
#include "libmaple/usart.h"
#include "libmaple/util.h"
#include "libmaple/usb.h"
#include "libmaple/usb_cdcacm.h"

#include "utils/utils.h"


#define USB_TIMEOUT 2

extern int32 (*non_busy_wait)(uint32);

/*
 * @brief   stm32 usb putstr routine
 * @param   buf -i/o- data buffer to be transmited
 *          len -i- length of data to be transmited
 * @return  none
 * @note    FIXME This will eventually affect performance because
 *                it has a USB_TIMEOUT long busy wait if we don't
 *                open tty cdc/acm device on host side.
 */
static void usb_putstr(const void *buf, uint32 len)
{
	if (!(usb_is_connected(USBLIB) && usb_is_configured(USBLIB)) || !buf)
		return;

	uint32 txed = 0;
	uint32 old_txed = 0;
	uint32 start = systick_uptime();

	uint32 sent = 0;

	while (txed < len && (systick_uptime() - start < USB_TIMEOUT)) {
		sent = usb_cdcacm_tx((const uint8 *)buf + txed, len - txed);
		txed += sent;
		if (old_txed != txed)
			start = systick_uptime();
		old_txed = txed;
	}

	if (sent == USB_CDCACM_TX_EPSIZE) {
		while (usb_cdcacm_is_transmitting() != 0) {
		}
		/* flush out to avoid having the pc wait for more data */
		usb_cdcacm_tx(NULL, 0);
	}
}

enum reset_state_t {
	DTR_UNSET,
	DTR_HIGH,
	DTR_NEGEDGE,
	DTR_LOW
};

static enum reset_state_t reset_state = DTR_UNSET;

static void usb_iface_setup_hook(unsigned hook, void *requestvp)
{
	uint8 request = *(uint8*)requestvp;

	/* Ignore requests we're not interested in. */
	if (request != USB_CDCACM_SET_CONTROL_LINE_STATE)
		return;

	/*
	 * We need to see a negative edge on DTR before we start looking
	 * for the in-band magic reset byte sequence.
	 */
	uint8 dtr = usb_cdcacm_get_dtr();
	switch (reset_state) {
	case DTR_UNSET:
		reset_state = dtr ? DTR_HIGH : DTR_LOW;
		break;
	case DTR_HIGH:
		reset_state = dtr ? DTR_HIGH : DTR_NEGEDGE;
		break;
	case DTR_NEGEDGE:
		reset_state = dtr ? DTR_HIGH : DTR_LOW;
		break;
	case DTR_LOW:
		reset_state = dtr ? DTR_HIGH : DTR_LOW;
		break;
	}
}

static void wait_reset(void)
{
	delay_us(RESET_DELAY);
	nvic_sys_reset();
}

#define STACK_TOP 0x20000800
#define EXC_RETURN 0xFFFFFFF9
#define DEFAULT_CPSR 0x61000000
static void usb_rx_hook(unsigned hook, void *ignored)
{
	uint32 i, target;
	static const uint8 magic[4] = {'1', 'E', 'A', 'F'};

	/*
	 * FIXME this is mad buggy; we need a new reset sequence. E.g. NAK
	 * after each RX means you can't reset if any bytes are waiting.
	 */
	if (reset_state == DTR_NEGEDGE) {
		reset_state = DTR_LOW;

		if (usb_cdcacm_data_available() >= 4) {
			/* The magic reset sequence is "1EAF". */
			uint8 chkBuf[4];

			/*
			 * Peek at the waiting bytes, looking for reset sequence,
			 * bailing on mismatch.
			 */
			usb_cdcacm_peek(chkBuf, 4);
			for (i = 0; i < sizeof(magic); i++)
				if (chkBuf[i] != magic[i])
					return;

			/* Got the magic sequence -> reset, presumably into the bootloader. */
			/* Return address is wait_reset, but we must set the thumb bit. */
			target = (uint32)wait_reset | 0x1;
			asm volatile("mov r0, %[stack_top]      \n\t" // Reset stack
					"mov sp, r0                \n\t"
					"mov r0, #1                \n\t"
					"mov r1, %[target_addr]    \n\t"
					"mov r2, %[cpsr]           \n\t"
					"push {r2}                 \n\t" // Fake xPSR
					"push {r1}                 \n\t" // PC target addr
					"push {r0}                 \n\t" // Fake LR
					"push {r0}                 \n\t" // Fake R12
					"push {r0}                 \n\t" // Fake R3
					"push {r0}                 \n\t" // Fake R2
					"push {r0}                 \n\t" // Fake R1
					"push {r0}                 \n\t" // Fake R0
					"mov lr, %[exc_return]     \n\t"
					"bx lr"
					:
					: [stack_top] "r" (STACK_TOP),
					[target_addr] "r" (target),
					[exc_return] "r" (EXC_RETURN),
					[cpsr] "r" (DEFAULT_CPSR)
					: "r0", "r1", "r2");
			/* Can't happen. */
			ASSERT_FAULT(0);
		}
	}
}

/*
 * @brief   enable USB (present us to USB)
 * @param   disc_dev io port for usb disc
 *          disc_bit bit of this io port for usb disc
 * @return  none
 * @note    none
 */
static void usb_enable(gpio_dev *disc_dev, uint8 disc_bit)
{
	usb_cdcacm_enable(disc_dev, disc_bit);
	usb_cdcacm_set_hooks(USB_CDCACM_HOOK_RX, usb_rx_hook);
	usb_cdcacm_set_hooks(USB_CDCACM_HOOK_IFACE_SETUP, usb_iface_setup_hook);
}

/*
 * @brief   enable USB (present us to USB)
 * @param   disc_dev io port for usb disc
 *          disc_bit bit of this io port for usb disc
 * @return  none
 * @note    none
 */
static void usb_disable(gpio_dev *disc_dev, uint8 disc_bit)
{
	usb_cdcacm_disable(disc_dev, disc_bit);
}

/*
 * @brief   usb getchar routine
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          buf -i/o- pointer to buffer to store received data
 * @return  0 read data from usb receiving buffer ok
 *          -1 no data in usb receiving buffer
 * @note    none
 */
static int32 usb_getc(void *p, char *buf)
{
	uint32 len = 0;
	if (!buf)
		return -1;
	len = usb_cdcacm_rx((uint8 *)buf, 1);
	if (len == 0)
		return -1;
	return 0;
}

/*
 * @brief   initialize flash on chip
 * @param   none
 * @return  none
 * @note    none
 */
static void flash_setup(void)
{
	/*
	 * Turn on as many Flash "go faster" features as
	 * possible. flash_enable_features() just ignores any flags it
	 * can't support.
	 */
	flash_enable_features(FLASH_PREFETCH | FLASH_ICACHE | FLASH_DCACHE);

	/*
	 * Configure the wait states, assuming we're operating at "close
	 * enough" to 3.3V.
	 */
	flash_set_latency(FLASH_SAFE_WAIT_STATES);
}

/*
 * @brief   initialize clock on chip
 * @param   none
 * @return  none
 * @note    FIXME These APIs are deprecated in libmaple
 *          since they only support stm32f1, we are still using it
 */
static void clocks_setup(void)
{

/*
 * FIXME stm32f1 only!!!
 * Allow boards to provide a PLL multiplier. This is useful for
 * e.g. STM32F100 value line MCUs, which use slower multipliers.
 * (We're leaving the default to RCC_PLLMUL_9 for now, since that
 * works for F103 performance line MCUs, which is all that LeafLabs
 * currently officially supports).
 */
#ifndef BOARD_RCC_PLLMUL
#define BOARD_RCC_PLLMUL RCC_PLLMUL_9
#endif

	static stm32f1_rcc_pll_data pll_data = {BOARD_RCC_PLLMUL};
	rcc_pll_cfg w_board_pll_cfg = {RCC_PLLSRC_HSE, &pll_data};

	/*
	 * Turn on HSI. We'll switch to and run off of this while we're
	 * setting up the main PLL.
	 */
	rcc_turn_on_clk(RCC_CLK_HSI);

	/*
	 * Turn off and reset the clock subsystems we'll be using, as well
	 * as the clock security subsystem (CSS). Note that resetting CFGR
	 * to its default value of 0 implies a switch to HSI for SYSCLK.
	 */
	RCC_BASE->CFGR = 0x00000000;
	rcc_disable_css();
	rcc_turn_off_clk(RCC_CLK_PLL);
	rcc_turn_off_clk(RCC_CLK_HSE);

	/* FIXME need to distinguish series, stm32f1 or other */
	/* board_reset_pll(); */

	/*
	 * Clear clock readiness interrupt flags and turn off clock
	 * readiness interrupts.
	 */
	RCC_BASE->CIR = 0x00000000;

	/* Enable HSE, and wait until it's ready. */
	rcc_turn_on_clk(RCC_CLK_HSE);
	while (!rcc_is_clk_ready(RCC_CLK_HSE))
		;

	/* Configure AHBx, APBx, etc. prescalers and the main PLL. */

	/* FIXME need to distinguish series, stm32f1 or other */
	rcc_set_prescaler(RCC_PRESCALER_AHB, RCC_AHB_SYSCLK_DIV_1);
	rcc_set_prescaler(RCC_PRESCALER_APB1, RCC_APB1_HCLK_DIV_2);
	rcc_set_prescaler(RCC_PRESCALER_APB2, RCC_APB2_HCLK_DIV_1);

	rcc_configure_pll(&w_board_pll_cfg);

	/* Enable the PLL, and wait until it's ready. */
	rcc_turn_on_clk(RCC_CLK_PLL);
	while(!rcc_is_clk_ready(RCC_CLK_PLL))
		;

	/* Finally, switch to the now-ready PLL as the main clock source. */
	rcc_switch_sysclk(RCC_CLKSRC_PLL);
}

/*
 * @brief   initialize nvic on chip
 * @param   none
 * @return  none
 * @note    none
 */
static void nvic_setup(void)
{
#define USER_ADDR_ROM 0x08005000
#define USER_ADDR_RAM 0x20000C00

#ifdef VECT_TAB_FLASH
	nvic_init(USER_ADDR_ROM, 0);
#elif defined VECT_TAB_RAM
	nvic_init(USER_ADDR_RAM, 0);
#elif defined VECT_TAB_BASE
	nvic_init((uint32)0x08000000, 0);
#else
#error "You must select a base address for the vector table."
#endif
}

/*
 * @brief   initialize adc on chip
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          ch -i- data to transmit
 * @return  none
 * @note    none
 */
static void adc_defconfig(const adc_dev* dev)
{
	adc_init(dev);
	adc_set_extsel(dev, ADC_SWSTART);
	adc_set_exttrig(dev, 1);
	adc_enable(dev);
	adc_calibrate(dev);
	adc_set_sample_rate(dev, ADC_SMPR_55_5);
}

/*
 * @brief   initialize each adc module on chip
 * @param   none
 * @return  none
 * @note    none
 */
static void adc_setup(void)
{
	rcc_set_prescaler(RCC_PRESCALER_ADC, RCC_ADCPRE_PCLK_DIV_6);
	adc_foreach(adc_defconfig);
}

/*
 * @brief   initialize timer on chip
 * @param   dev -i- timer device pointer
 * @return  none
 * @note    none
 */
static void timer_defconfig(timer_dev *dev) {
	timer_adv_reg_map *regs = (dev->regs).adv;
	const uint16 full_overflow = 0xFFFF;
	const uint16 half_duty = 0x8FFF;
	int channel;

	timer_init(dev);
	timer_pause(dev);

	regs->CR1 = TIMER_CR1_ARPE;
	regs->PSC = 1;
	regs->SR = 0;
	regs->DIER = 0;
	regs->EGR = TIMER_EGR_UG;

	switch (dev->type) {
	case TIMER_ADVANCED:
		regs->BDTR = TIMER_BDTR_MOE | TIMER_BDTR_LOCK_OFF;
		/* fall-through */
	case TIMER_GENERAL:
		timer_set_reload(dev, full_overflow);

		for (channel = 1; channel <= 4; channel++) {
			timer_set_compare(dev, channel, half_duty);
			timer_oc_set_mode(dev, channel, TIMER_OC_MODE_PWM_1, TIMER_OC_PE);
		}
		/* fall-through */
	case TIMER_BASIC:
		break;
	}

	timer_resume(dev);
}

/*
 * @brief   initialize each timer module on chip
 * @param   none
 * @return  none
 * @note    none
 */
static void timers_setup(void)
{
	timer_foreach(timer_defconfig);
}

/*
 * @brief   initialize usart on chip
 * @param   dev -i- usart device pointer
 *          baud -i- baudrate
 * @return  none
 * @note    none
 */
static void usart_setup(usart_dev *dev, uint32 baud)
{
	uint32 i = USART_RX_BUF_SIZE;

	gpio_set_mode(USART_CONSOLE_PORT, USART_CONSOLE_TX, GPIO_AF_OUTPUT_PP);
	gpio_set_mode(USART_CONSOLE_PORT, USART_CONSOLE_RX, GPIO_INPUT_FLOATING);

	usart_init(dev);
	usart_set_baud_rate(dev, 72000000UL, baud);
	usart_disable(dev);
	usart_enable(dev);

	/* flush buffer */
	while (i--) {
		usart_putc(dev, '\r');
	}
}

/*
 * @brief   stm32 board specific init
 * @param   none
 * @return  none
 * @note    none
 */
void utils_board_init(void)
{
	flash_setup();
	clocks_setup();
	nvic_setup();
#if 0
	/*
	 * systick should only be enabled after specific
	 * init steps has been finished while os bootup
	 */
	systick_init(SYSTICK_RELOAD_VAL);
#endif
	gpio_init_all();
	afio_init();
	adc_setup();
	timers_setup();
	usart_setup(USART_CONSOLE_BANK, SERIAL_BAUDRATE);
	usb_disable(USB_DISC_DEV, USB_DISC_BIT);
	usb_enable(USB_DISC_DEV, USB_DISC_BIT);

	usb_putstr("", 0);
	usart_putc(USART_CONSOLE_BANK, 0x0c);
	gpio_set_mode(ERROR_LED_PORT, ERROR_LED_PIN, GPIO_OUTPUT_PP);
	gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 0);
}

/*
 * @brief   stm32 io putchar routine
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          ch -i- data to be transmited
 * @return  none
 * @note    device is USART_CONSOLE_BANK by default
 */
void utils_io_putc(void *p, char ch)
{
	usart_putc(USART_CONSOLE_BANK, ch);
}

/*
 * @brief   stm32 io getchar routine, block here if no data available
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 * @return  char
 */
char utils_io_getc(void *p)
{
	char ch;
	do {
		ch = usart_getc(p);
		if (non_busy_wait)
			non_busy_wait(10);
		else
			continue;
	} while (ch < 0);
	return ch;
}

/*
 * @brief   stm32 usb putchar routine
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          ch -i- data to be transmited
 * @return  none
 * @note    FIXME here use putstr to support putc, bad logic
 */
void utils_usb_putc(void *p, char ch)
{
	const uint8 buf[] = {ch};
	usb_putstr(buf, 1);
}

/*
 * @brief   stm32 usb getchar routine, block here if no data available
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 * @return  char
 * @note    none
 */
char utils_usb_getc(void *p)
{
	char ch;
	while (usb_getc(p, &ch) != 0) {
		if (non_busy_wait)
			non_busy_wait(10);
		else
			continue;
	}
	return ch;
}
