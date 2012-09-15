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
 * @file    core/sys_debug.c
 * @brief   debuging services
 * @log     2011.8 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_port.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/print.h>
#include <sys/utils.h>
#include <sys/debug.h>

/*
 * @brief   os assert failing routine
 * @param   file -i- __FILE__
 *          line -i- __LINE__
 *          exp -i- assert expression
 * @return  none
 * @note    may call porting assert service or implemented solely
 */
void _os_assert_fail(const char *p_file, int line, const char *p_exp)
{
	os_printk(LOG_ERROR, "line %d in %s, %s fail!\n", line, p_file, p_exp);

	/* throb alarm signal or something */
	port_assert_fail();
}
