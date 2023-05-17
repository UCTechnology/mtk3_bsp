/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
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
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 *----------------------------------------------------------------------
 *    Modifications: Porting to micro T-Kernel 2.0
 *    Modified by UC Technology at 2013/3/25.
 *
 *    UCT micro T-Kernel DevKit tuned for Kinetis Version 2.00.02
 *    Copyright (c) 2011-2014 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */
#ifndef __CC_H__
#define __CC_H__

/* For wchar_t conflict */
#define _WCHART

#include <tm/tmonitor.h>
#include <tk/tkernel.h>
#include <tk/syslib.h>
#include <dev_ser.h>

typedef		UB	u8_t;
typedef		UB	uint8_t;
typedef		B	s8_t;
typedef		UH	u16_t;
typedef		H 	s16_t;
typedef		UW	u32_t;
typedef		W	s32_t;

#define LWIP_NO_STDDEF_H	1
#define LWIP_NO_STDINT_H	1
#define LWIP_NO_INTTYPES_H	1
#define LWIP_NO_CTYPE_H		1

typedef		u32_t		mem_ptr_t;

#define NETIF_LINK_SPEED	10000000		/* 10MBPS */

#define PPPOS_DEVNM	"sera"

#ifndef __sio_fd_t_defined
#define __sio_fd_t_defined
typedef struct _sio_fd_t{
	ID devid;
	FastLock lock;
} *sio_fd_t;
#endif // __sio_fd_t_defined

#define LWIP_RAND() ((u32_t)rand())


// For LWIP_NO_INTTYPES_H = 1
#ifndef X8_F
#define X8_F  "x"
#endif
#ifndef U16_F
#define U16_F "hu"
#endif
#ifndef S16_F
#define S16_F "hd"
#endif
#ifndef X16_F
#define X16_F "hx"
#endif
#ifndef U32_F
#define U32_F "lu"
#endif
#ifndef S32_F
#define S32_F "ld"
#endif
#ifndef X32_F
#define X32_F "lx"
#endif
#ifndef SZT_F
#define SZT_F "lu"
#endif


#if BIGENDIAN
#define BYTE_ORDER   BIG_ENDIAN
#else
#define BYTE_ORDER   LITTLE_ENDIAN
#endif

#if defined(__GNUC__)
	#define PACK_STRUCT_FIELD(x)	x
	#define PACK_STRUCT_STRUCT		__attribute__((__packed__))
	#define PACK_STRUCT_BEGIN
	#define PACK_STRUCT_END
#elif defined(__IASMARM__) || defined(__ICCARM__)
	#define PACK_STRUCT_FIELD(x)	x
	#define PACK_STRUCT_STRUCT
	#define PACK_STRUCT_BEGIN		__packed
	#define PACK_STRUCT_END
#elif defined(__CC_ARM)
	#define PACK_STRUCT_FIELD(x)	x
	#define PACK_STRUCT_STRUCT      __attribute__((packed))
	#define PACK_STRUCT_BEGIN
	#define PACK_STRUCT_END
#elif defined(__RENESAS__)
	#define PACK_STRUCT_FIELD(x)	x
	#define PACK_STRUCT_STRUCT      
	#define PACK_STRUCT_BEGIN	_Pragma("pack")
	#define PACK_STRUCT_END		_Pragma("unpack")
#endif

#ifndef LWIP_PLATFORM_DIAG
#define NO_WARNING_PRINTF(fmt, ...)	tm_printf((UB *)fmt, ##__VA_ARGS__)
#define LWIP_PLATFORM_DIAG(x)	do {NO_WARNING_PRINTF x;} while(0)
#endif

#ifndef LWIP_PLATFORM_ASSERT
#define LWIP_PLATFORM_ASSERT(x) do {tm_printf((UB *) "Assertion \"%s\" failed at line %d in %s\n", x, __LINE__, __FILE__); while(1);} while(0)
#endif

#if defined(__GNUC__) || defined(__RENESAS__)
#define  snprintf(str, size, format, ...)		tm_sprintf(str, format, ##__VA_ARGS__)
#define  sprintf(str, format, ...)				tm_sprintf(str, format, ##__VA_ARGS__)
#endif // __GNUC__

#define LWIP_CONST_CAST(target_type, val) ((target_type)(val))

int rand(void);

#endif /* __CC_H__ */

