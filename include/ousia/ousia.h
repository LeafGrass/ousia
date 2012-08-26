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
 * @file    include/ousia/ousia.h
 * @brief   ousia header
 * @log     2011.8 initial revision
 *          2012.3 add version code
 */

#ifndef __OUSIA_H__
#define __OUSIA_H__


#define VERCODE		(VERSION*100 + SUBLEVEL*10 + DEVNUM)
#define VERSTR(str)	#str

#define BOOT_LOGO() \
{ \
	os_putchar(0x0C); \
	os_printf("                       _\n"); \
	os_printf("     _                / /\n"); \
	os_printf("    | |    ___  __ _ _| |_ __ _ _  __  __ _   _   _\n"); \
	os_printf("    | |   / _ \\/ _` |_   _/ _` | \\/ _)/ _` | / / / /\n"); \
	os_printf("    | |_ _  __( (_| | | |  (_| | | | ( (_| | \\ \\ \\ \\\n"); \
	os_printf("    |_ _ _\\___|\\__,_| | | \\__, / | |  \\__,_| /_/ /_/\n"); \
	os_printf("                      /_/ \\_ _/\n" ); \
	os_printf("\n"); \
	os_printf("Ousia v%d.%d.%d %s\n", VERSION, SUBLEVEL, DEVNUM, VERNAME); \
	os_printf("\tby LeafGrass - leafgrass.g@gmail.com"); \
	os_printf("\n\n"); \
	os_log("# Hello Ousia :)\n\n"); \
}


#endif /* __OUSIA_H__ */
