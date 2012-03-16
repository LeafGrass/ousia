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
 * @file    core/sys_utils.c
 * @brief   ousia system utilities
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
 * @brief   the very first init of ousia
 * @param   none
 * @return  os status code
 * @note    this function should be called before all other syscalls
 *          interrupts should better no be enabled before os init finished
 */
os_status os_init(void)
{
	os_status ret = OS_OK;

	_os_port_init();
	_init_printf();
	BOOT_LOGO();
//	os_logk(LOG_INFO, "%s - init 1\n", __FUNCTION__);
//	OS_DISABLE_INTERRUPTS();
//	os_logk(LOG_INFO, "%s - init 2\n", __FUNCTION__);
	_sys_timetick_init();
	ret = _sys_sched_init();
	ret = _sys_sched_process_init();

	return ret;
}

/*
 * @brief   now it's time to run
 * @param   none
 * @return  none
 * @note    will never return, everything will be handled by ousia then
 */
void os_run(void)
{
	_sys_sched_startup();
}
