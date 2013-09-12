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
 * @file    framework/console/console.h
 * @brief   console header
 * @log     2011.11 first revision
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/*#define DEBUG_CONSOLE*/
#define CMD_CHAR_BUF_SIZE	32

struct cmd_char_stack {
	int32 nc;
	int32 nc_tot;
	struct list_head s;
};

struct cmd_char {
	char c;
	struct list_head list;
};

struct console_cmd {
	const struct cmd_handle *hcmd_arr;
	struct cmd_char_stack ccs;
	struct cmd_char cbuf[CMD_CHAR_BUF_SIZE];
};

void ps_console(void *args);

#endif /* __CONSOLE_H__ */
