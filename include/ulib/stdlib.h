/*
 * Copyright (c) 2011-2013 LeafGrass <leafgrass.g@gmail.com>
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
 * @file    include/ulib/stdlib.h
 * @brief   micro implementation of stdlib.h of libc
 * @log     2013.5 initial revision
 */

#ifndef __ULIB_STDLIB_H__
#define __ULIB_STDLIB_H__

struct mallinfo
{
	int arena;    /* This is the total size of memory allocated
		       * for use by malloc in bytes. */
	int ordblks;  /* This is the number of free (not in use) chunks */
	int mxordblk; /* Size of the largest free (not in use) chunk */
	int uordblks; /* This is the total size of memory occupied by
		       * chunks handed out by malloc. */
	int fordblks; /* This is the total size of memory occupied
		       * by free (not in use) chunks.*/
};

#endif /* __ULIB_STDLIB_H__ */
