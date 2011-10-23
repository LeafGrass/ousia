/* *****************************************************************************
 * @file    platform/stm32/stm32utils/stm32utils.c
 *
 * @brief   stm32 library utilities
 *
 * @log     2011.8 initial revision
 *
 * *****************************************************************************
 * COPYRIGHT (C) LEAFGRASS - LeafGrass (leafgrass.g@gmail.com)
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
#include <usb_cdcacm.h>
#include <usart.h>
#include <util.h>

#include <stm32/stm32utils/stm32utils.h>


#define USB_TIMEOUT 50


static void setupFlash(void);
static void setupClocks(void);
static void setupNVIC(void);
static void setupADC(void);
static void setupTimers(void);
static void setupUSART(usart_dev *dev, uint32 baud);
static void enableUSB(gpio_dev *disc_dev, uint8 disc_bit);
static void disableUSB(gpio_dev *disc_dev, uint8 disc_bit);
static void stm32utils_usb_putstr(const void *buf, uint32 len);


/*
 * @brief   stm32 board specific init
 * @param   none
 * @return  none
 * @note    none
 */
void stm32utils_board_init(void)
{
        setupFlash();
        setupClocks();
        setupNVIC();
        systick_init(SYSTICK_RELOAD_VAL);
        gpio_init_all();
        afio_init();
        setupADC();
        setupTimers();
        setupUSART(USART_CONSOLE_BANK, SERIAL_BAUDRATE);
        disableUSB(USB_DISC_DEV, USB_DISC_BIT);
        enableUSB(USB_DISC_DEV, USB_DISC_BIT);
        stm32utils_usb_putstr("", 0);

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
void stm32utils_io_putc(void *p, char ch)
{
        usart_putc(USART_CONSOLE_BANK, ch);
}

/*
 * @brief   stm32 io getchar routine
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          ch -i/o- pointer to variable to store received data
 * @return  return status
 * @note    TODO needs to be completed
 */
int32 stm32utils_io_getc(void *p, char *ch)
{
        *ch = usart_getc(p);
        return 0;
}

/*
 * @brief   stm32 usb putchar routine
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          ch -i- data to be transmited
 * @return  none
 * @note    FIXME here use putstr to support putc, bad logic
 */
void stm32utils_usb_putc(void *p, char ch)
{
        const uint8 buf[] = {ch};
        stm32utils_usb_putstr(buf, 1);
}

/*
 * @brief   stm32 usb getchar routine
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          buf -i/o- pointer to buffer to store received data
 * @return  0 read data from usb receiving buffer ok
 *          -1 no data in usb receiving buffer
 * @note    none
 */
int32 stm32utils_usb_getc(void *p, char *buf)
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
 * @brief   enable USB (present us to USB)
 * @param   disc_dev io port for usb disc
 *          disc_bit bit of this io port for usb disc
 * @return  none
 * @note    none
 */
void enableUSB(gpio_dev *disc_dev, uint8 disc_bit)
{
    usb_cdcacm_enable(disc_dev, disc_bit);
}

/*
 * @brief   enable USB (present us to USB)
 * @param   disc_dev io port for usb disc
 *          disc_bit bit of this io port for usb disc
 * @return  none
 * @note    none
 */
void disableUSB(gpio_dev *disc_dev, uint8 disc_bit)
{
    usb_cdcacm_disable(disc_dev, disc_bit);
}

/*
 * @brief   stm32 usb putstr routine
 * @param   buf -i/o- data buffer to be transmited
 *          len -i- length of data to be transmited
 * @return  none
 * @note    none
 */
static void stm32utils_usb_putstr(const void *buf, uint32 len)
{
        if (!(usbIsConnected() && usbIsConfigured()) || !buf)
                return;

        uint32 txed = 0;
        uint32 old_txed = 0;
        uint32 start = systick_uptime();

        while (txed < len && (systick_uptime() - start < USB_TIMEOUT)) {
                txed += usb_cdcacm_tx((const uint8 *)buf + txed, len - txed);
                if (old_txed != txed)
                        start = systick_uptime();
                old_txed = txed;
        }
}

/*
 * @brief   initialize flash on chip
 * @param   none
 * @return  none
 * @note    none
 */
static void setupFlash(void)
{
        flash_enable_prefetch();
        flash_set_latency(FLASH_WAIT_STATE_2);
}

/*
 * @brief   initialize clock on chip
 * @param   none
 * @return  none
 * @note    none
 */
static void setupClocks(void)
{
        rcc_clk_init(RCC_CLKSRC_PLL, RCC_PLLSRC_HSE, RCC_PLLMUL_9);
        rcc_set_prescaler(RCC_PRESCALER_AHB, RCC_AHB_SYSCLK_DIV_1);
        rcc_set_prescaler(RCC_PRESCALER_APB1, RCC_APB1_HCLK_DIV_2);
        rcc_set_prescaler(RCC_PRESCALER_APB2, RCC_APB2_HCLK_DIV_1);
}

/*
 * @brief   initialize nvic on chip
 * @param   none
 * @return  none
 * @note    none
 */
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

/*
 * @brief   initialize adc on chip
 * @param   p -i- device pointer (could be NULL in tfp_printf)
 *          ch -i- data to transmit
 * @return  none
 * @note    none
 */
static void adcDefaultConfig(const adc_dev* dev)
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
static void setupADC(void)
{
        rcc_set_prescaler(RCC_PRESCALER_ADC, RCC_ADCPRE_PCLK_DIV_6);
        adc_foreach(adcDefaultConfig);
}

/*
 * @brief   initialize timer on chip
 * @param   dev -i- timer device pointer
 * @return  none
 * @note    none
 */
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

/*
 * @brief   initialize each timer module on chip
 * @param   none
 * @return  none
 * @note    none
 */
static void setupTimers(void)
{
        timer_foreach(timerDefaultConfig);
}

/*
 * @brief   initialize usart on chip
 * @param   dev -i- usart device pointer
 *          baud -i- baudrate
 * @return  none
 * @note    none
 */
static void setupUSART(usart_dev *dev, uint32 baud)
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

