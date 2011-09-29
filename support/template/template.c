/* *****************************************************************************
 * @file    support/template/template.c
 *
 * @brief   File template.
 *
 * @log     2011-07-28 Initial revision
 *          2011-07-29 Second revision
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

#include <template.h>

/*
 * @brief   sample function
 * @param   a -i- an integer
 *          b -i- another integer
 *          result -io- result
 * @return  an integer
 * @note    just use it and take it easy
 */
int sample_function(int a, int b, int *result)
{
    *result = a + b;
    return 0;
}

