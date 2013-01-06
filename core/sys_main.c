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
 * @file    core/sys_main.c
 * @brief   implementation of ousia scheduler
 * @log     2012.8 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_cfg.h>
#include <port/ousia_port.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/debug.h>
#include <sys/utils.h>

/*
 * @brief   start entry after asmlinkage
 * @param   none
 * @return  none
 */
void __os_main(void)
{
	int32 ret;

	port_bsp_init();
	ret = os_init();
	os_assert(ret == OS_OK);
	os_kick_off();
}
