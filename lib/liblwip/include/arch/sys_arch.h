/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
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
 
#ifndef __SYS_UTK_H__
#define __SYS_UTK_H__

#include <tk/tkernel.h>

#define LWIP_COMPAT_MUTEX         1
#define LWIP_COMPAT_MUTEX_ALLOWED 1
#define LWIP_MAX_MAILBOX          32

#define SYS_MBOX_NULL             NULL
#define SYS_SEM_NULL              NULL

typedef ID sys_sem_t;
typedef ID sys_mbox_t;
typedef ID sys_thread_t;
typedef UINT sys_prot_t;

#define sys_jiffies               sys_now
#define sys_mbox_trypost_fromisr  sys_mbox_trypost

#endif /* __SYS_UTK_H__ */
