/* *****************************************************************************
 * @file    platform/stm32/stm32utils/utils.h
 *
 * @brief   stm32 library libmaple utilities
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

#ifndef __STM32UTILS_H__
#define __STM32UTILS_H__

/*
 * @brief   sample function
 * @param   a -i- an integer
 *          b -i- another integer
 *          pResult -o- result
 * @return  an integer
 * @note    just use it and take it easy
 */
void stm32utils_system_init(void);

/* for printf */
void stm32utils_io_putc(void *p, char ch);


#endif /* __STM32UTILS_H__ */

