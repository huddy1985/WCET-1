/*
 * Declaration of functions and data types used for MD5 sum computing
 * library functions.  Copyright (C) 1995, 1996 Free Software
 * Foundation, Inc.  NOTE: The canonical source of this file is
 * maintained with the GNU C Library.  Bugs can be reported to
 * bug-glibc@prep.ai.mit.edu.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * $Id: md5.h,v 1.4 2000/03/09 04:06:41 gray Exp $
 */

/*
 * NOTE: during quick performance tests on a sun ultra and an alpha
 * 255 300, the md5 libraries performed upwards of 3mb/sec.  That
 * included disk i/o on bobcat and panther.
 */

#ifndef __MD5_H__
#define __MD5_H__

/*
 * Size of a standard MD5 signature in bytes.  This definition is for
 * external programs only.  The MD5 routines themselves reference the
 * signature as 4 unsigned 32-bit integers.
 */
#define MD5_SIZE	16

/*
 * NOTE: the following is assumed to generate a 32-bit unsigned data
 * type.
 */
typedef unsigned int		md5_uint32;
#define MAX_MD5_UINT32		((md5_uint32)4294967295U)

/*
 * The MD5 algorithm works on blocks of characters of 64 bytes.  This
 * is an internal value only and is not necessary for external use.
 */
#define MD5_BLOCK_SIZE		64


/*
 * md5_buffer
 *
 * DESCRIPTION:
 *
 * This function is used to calculate a MD5 signature for a buffer of
 * bytes of maximum length of 56 bytes. 
 *
 * WCET: 17778 cycles
 *
 * RETURNS:
 *
 * None.
 *
 * ARGUMENTS:
 *
 * buffer - A buffer of bytes whose MD5 signature we are calculating.
 *
 * buf_len - The length of the buffer. Max Length 56 Bytes
 *
 * signature - A 16 byte buffer that will contain the MD5 signature.
 */
extern void md5_buffer_56(const char *buffer, const unsigned int buf_len, void *signature);


#endif /* ! __MD5_H__ */
