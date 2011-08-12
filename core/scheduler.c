/* *****************************************************************************
 * @file    core/scheduler.c
 *
 * @brief   Implementation of ousia scheduler
 *
 * @log     2011-07-28 Initial revision
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

#include <ousia/scheduler.h>

/*
 * @brief   sample function
 * @param   a -i- an integer
 *          b -i- another integer
 *          pResult -o- result
 * @return  an integer
 * @note    just use it and take it easy
 */
int sample_function(int a, int b, int *pResult)
{
    *pResult = a + b;
    return 0;
}

