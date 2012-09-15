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
 * @file    core/sys_sync.c
 * @brief   core system synchronization implementation
 * @log     2012.9 initial revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <port/ousia_port.h>
#include <sys/sync.h>

static unsigned int critical_nest = 0;

/*
 * @brief   os_enter_critical
 * @param   none
 * @return  none
 */
void os_enter_critical(void)
{
	OS_DISABLE_INTERRUPTS();
	critical_nest++;
}

/*
 * @brief   os_exit_critical
 * @param   none
 * @return  none
 */
void os_exit_critical(void)
{
	critical_nest--;
	if (critical_nest == 0)
		OS_ENABLE_INTERRUPTS();
}
