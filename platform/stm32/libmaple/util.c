/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Modification:
 *     @date 2011.08
 *     @reviser Librae
 *     @brief Modify for specific board.
 *****************************************************************************/

/**
 * @file libmaple/util.c
 * @brief Utility procedures for debugging
 */

#include <libmaple/libmaple.h>
#include <libmaple/usart.h>
#include <libmaple/gpio.h>
#include <libmaple/nvic.h>

uint32 __exc_num = 0;

/* (Undocumented) hooks used by Wirish to direct our behavior here */
extern __weak void __lm_error(void);
extern __weak usart_dev* __lm_enable_error_usart(void);

/* If you define ERROR_LED_PORT and ERROR_LED_PIN, then a failed
 * ASSERT() will also throb() an LED connected to that port and pin.
 */
#if defined(ERROR_LED_PORT) && defined(ERROR_LED_PIN)
#define HAVE_ERROR_LED
#endif

static void (*__def_exc_hook)(uint32 psp, uint32 exc_num);

static inline uint32 __get_psp(void)
{
    uint32 r;
    __asm volatile ("mrs %0, psp\n" : "=r"(r));
    return r;
}

static inline uint32 __get_msp(void)
{
    uint32 r;
    __asm volatile ("mrs %0, msp\n" : "=r"(r));
    return r;
}

/* (Called from exc.S with global interrupts disabled.) */
__attribute__((noreturn)) void __error(void)
{
    if (__lm_error)
        __lm_error();

    /* Reenable global interrupts */
    nvic_globalirq_enable();

    /* Go to ousia default exception handler. */
    __def_exc_hook(__get_psp(), __exc_num);

    throb();
}

/*
 * Print an error message on a UART upon a failed assertion (if one is
 * available), and punt to __error().
 *
 * @param file Source file of failed assertion
 * @param line Source line of failed assertion
 * @param exp String representation of failed assertion
 * @sideeffect Turns of all peripheral interrupts except USB.
 */
void _fail(const char* file, int line, const char* exp)
{
    if (__lm_enable_error_usart) {
        /* Initialize the error USART */
        usart_dev *err_usart = __lm_enable_error_usart();

        /* Print failed assert message */
        usart_putstr(err_usart, "ERROR: FAILED ASSERT(");
        usart_putstr(err_usart, exp);
        usart_putstr(err_usart, "): ");
        usart_putstr(err_usart, file);
        usart_putstr(err_usart, ": ");
        usart_putudec(err_usart, line);
        usart_putc(err_usart, '\n');
        usart_putc(err_usart, '\r');
    }
    /* Shutdown and error fade */
    __error();
}

/*
 * Provide an __assert_func handler to libc so that calls to assert()
 * get redirected to _fail.
 */
void __assert_func(const char* file, int line, const char* method,
                   const char* expression)
{
    _fail(file, line, expression);
}

/*
 * Provide an abort() implementation that aborts execution and punts
 * to __error().
 */
void abort()
{
    if (__lm_enable_error_usart) {
        /* Initialize the error USART */
        usart_dev *err_usart = __lm_enable_error_usart();
        /* Print abort message. */
        usart_putstr(err_usart, "ERROR: PROGRAM ABORTED VIA abort()\r\n");
    }

    /* Shutdown and error fade */
    __error();
}

/* This was public as of v0.0.12, so we've got to keep it public. */
/**
 * @brief Fades the error LED on and off
 * @sideeffect Sets output push-pull on ERROR_LED_PIN.
 */
__attribute__((noreturn)) void throb(void)
{
#ifdef HAVE_ERROR_LED
    int32  slope   = 1;
    uint32 CC      = 0x0000;
    uint32 TOP_CNT = 0x0400;
    uint32 i       = 0;

    gpio_set_mode(ERROR_LED_PORT, ERROR_LED_PIN, GPIO_MODE_OUTPUT);
    /* Error fade. */
    while (1) {
        if (CC == TOP_CNT)  {
            slope = -1;
        } else if (CC == 0) {
            slope = 1;
        }

        if (i == TOP_CNT)  {
            CC += slope;
            i = 0;
        }

        if (i < CC) {
            gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 1);
        } else {
            gpio_write_bit(ERROR_LED_PORT, ERROR_LED_PIN, 0);
        }
        i++;
    }
#else
    /* No error LED is defined; do nothing. */
    while (1)
        ;
#endif
}

void attach_exc_hook(void (*fn)(uint32 psp, uint32 exc_num))
{
    __def_exc_hook = fn;
}
