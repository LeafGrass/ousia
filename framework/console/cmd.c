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
 * @file    framework/console/cmd.c
 * @brief   various definitions and implementations of commands
 * @log     2012.9 first revision
 */

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <ulib/stdlib.h>
#include <sys/time.h>
#include <sys/sched.h>
#include <sys/print.h>
#include <sys/mm.h>
#include <console/console.h>
#include <console/cmd.h>


static int32 cmd_help(void *args);
static int32 cmd_debug(void *args);
static int32 cmd_reboot(void *args);
static int32 cmd_free(void *args);

const struct cmd_handle hcmd_arr[] = {
	{
		.cmd_word = "help",
		.cmd_fn = cmd_help,
	},
	{
		.cmd_word = "debug",
		.cmd_fn = cmd_debug,
	},
	{
		.cmd_word = "reboot",
		.cmd_fn = cmd_reboot,
	},
	{
		.cmd_word = "free",
		.cmd_fn = cmd_free,
	},
	{
		.cmd_word = NULL,
		.cmd_fn = NULL,
	}
};

void commands_register(struct console_cmd *conc)
{
	conc->hcmd_arr = hcmd_arr;
}

static int32 cmd_help(void *args)
{
	const struct cmd_handle *c;
	os_printf("available commands:\n");
	for (c = &hcmd_arr[0]; c->cmd_word != NULL; c++)
		os_printf("\t%s\n", c->cmd_word);
	return 0;
}

static int32 cmd_debug(void *args)
{
	_sched_dump_pq();
	return 0;
}

static int32 cmd_reboot(void *args)
{
#define DO_SOMETHING()	os_process_sleep(1000)
	/* TODO Prepare things before reboot */
	os_printf("Rebooting...\n");
	DO_SOMETHING();
	os_printf("Ousia warm down.\n\n");
	/* TODO Do real hardware reset here */
	return 0;
}

static int32 cmd_free(void *args)
{
	struct mallinfo data;
	mallinfo(&data);
	os_printf("              total       used       free    largest\n");
	os_printf("Mem:    %11d%11d%11d%11d\n",
			   data.arena, data.uordblks,
			   data.fordblks, data.mxordblk);
	return 0;
}
