/* *****************************************************************************
 * @file    platform/stm32/stm32utils/stm32utils.c
 *
 * @brief   stm32 library utilities
 * @date    2011-08-03
 *
 * @history 2011-08-03 Initial revision
 *
 * *****************************************************************************
 * COPYRIGHT (C) LEAFGRASS - Librae (librae8226@gmail.com)
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 * ****************************************************************************/

#include <libmaple.h>
#include <libmaple_types.h>
#include <flash.h>
#include <rcc.h>
#include <nvic.h>
#include <systick.h>
#include <gpio.h>
#include <adc.h>
#include <timer.h>
#include <usb.h>
#include <usart.h>

#include <stm32/stm32utils/stm32utils.h>

#define SYSTICK_RELOAD_VAL  71999 /* takes a cycle to reload */
#define USARTx  USART1
#define SERIAL_BAUDRATE 9600

static void setupFlash(void);
static void setupClocks(void);
static void setupNVIC(void);
static void setupADC(void);
static void setupTimers(void);
static void setupUSART(usart_dev *dev, uint32 baud);


/**
 * @brief   stm32 chip intialize
 * @param   none
 * @return  none
 * @note    FIXME should not be called this directly
 */
void stm32utils_system_init(void)
{
    setupFlash();
    setupClocks();
    setupNVIC();
    systick_init(SYSTICK_RELOAD_VAL);
    gpio_init_all();
    afio_init();
    setupADC();
    setupTimers();
    setupUSART(USARTx, SERIAL_BAUDRATE);

    gpio_set_mode(GPIOA, 0, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOA, 0, 0);
    gpio_set_mode(GPIOA, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOA, 1, 0);
    gpio_set_mode(GPIOA, 12, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOA, 12, 0);
}

/**
 * @brief   stm32 chip intialize
 * @param   none
 * @return  none
 * @note    FIXME should not be called this directly
 */
void stm32utils_io_putc(void *p, char ch)
{
    usart_putc(USARTx, ch);
    return;
}

static void setupFlash(void)
{
    flash_enable_prefetch();
    flash_set_latency(FLASH_WAIT_STATE_2);
}

static void setupClocks(void)
{
    rcc_clk_init(RCC_CLKSRC_PLL, RCC_PLLSRC_HSE, RCC_PLLMUL_9);
    rcc_set_prescaler(RCC_PRESCALER_AHB, RCC_AHB_SYSCLK_DIV_1);
    rcc_set_prescaler(RCC_PRESCALER_APB1, RCC_APB1_HCLK_DIV_2);
    rcc_set_prescaler(RCC_PRESCALER_APB2, RCC_APB2_HCLK_DIV_1);
}

static void setupNVIC(void)
{
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

static void adcDefaultConfig(const adc_dev* dev)
{
    adc_init(dev);
    adc_set_extsel(dev, ADC_SWSTART);
    adc_set_exttrig(dev, 1);
    adc_enable(dev);
    adc_calibrate(dev);
    adc_set_sample_rate(dev, ADC_SMPR_55_5);
}
static void setupADC(void)
{
    rcc_set_prescaler(RCC_PRESCALER_ADC, RCC_ADCPRE_PCLK_DIV_6);
    adc_foreach(adcDefaultConfig);
}

static void timerDefaultConfig(timer_dev *dev) {
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
static void setupTimers(void)
{
    timer_foreach(timerDefaultConfig);
}

static void setupUSART(usart_dev *dev, uint32 baud)
{
    uint32 i = USART_RX_BUF_SIZE;
    /* FIXME: need some preprocess here, according to specific board */
    if (dev == USART1) {
        gpio_set_mode(GPIOA, 9, GPIO_AF_OUTPUT_PP);
        gpio_set_mode(GPIOA, 10, GPIO_INPUT_FLOATING);
    }
    usart_init(dev);
    usart_set_baud_rate(dev, 72000000UL, baud);
    usart_disable(dev);
    usart_enable(dev);
    /* flush buffer */
    while (i--) {
        usart_putc(dev, '\r');
    }
}

