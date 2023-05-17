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

#include <tk/tkernel.h>

#include "lwip/sys.h"
#include "lwip/def.h"

int errno;

typedef struct _LWIP_MBX{
	T_MSG	t;
	void *msg;
} LWIP_MBX;

static ID sys_mbx_pool = (ID) NULL;
static struct _sys_prot{
  FastLock lock;
  ID ownertask;
  W ownercnt;
} sys_prot_lock;

void
sys_init(void) {
  T_CMPF cmpf;
  ER ercd;
  
  cmpf.exinf = NULL;
  cmpf.mpfatr = TA_TFIFO | TA_RNG0;
  cmpf.mpfcnt = LWIP_MAX_MAILBOX;
  cmpf.blfsz = sizeof(LWIP_MBX);
  ercd = tk_cre_mpf(&cmpf);
  if(ercd < E_OK){
    tm_putstring((UB *)"lwIP Initialize failed: Cannot create memory pool for mailbox.\n");
    return;
  }
  sys_mbx_pool = ercd;
  
  CreateLock(&sys_prot_lock.lock, NULL);
  sys_prot_lock.ownertask = 0;
  sys_prot_lock.ownercnt = 0;
  
  return;
}

err_t
sys_sem_new(sys_sem_t *sem, u8_t count)
{
  T_CSEM csem;
  ID semid;
  
  csem.exinf = NULL;
  csem.sematr = TA_TFIFO | TA_FIRST;
  csem.isemcnt = count;
  csem.maxsem = 1;
  semid = tk_cre_sem(&csem);
  if(semid < E_OK){
    return ERR_MEM;
  }
  *sem = semid;
  
  return ERR_OK;
}

void
sys_sem_free(sys_sem_t *sem)
{
  tk_del_sem(*sem);
}

void
sys_sem_signal(sys_sem_t *sem)
{
  tk_sig_sem(*sem, 1);
}

u32_t
sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  ER ercd;
  u32_t sttim = sys_now();
  
  ercd = tk_wai_sem(*sem, 1, timeout > 0 ? ((W) timeout) : TMO_FEVR);
  
  if(ercd < E_OK){
    return SYS_ARCH_TIMEOUT;
  }
  
  return (sys_now() - sttim);
}

int
sys_sem_valid(sys_sem_t *sem)
{
  return ((*sem == (sys_sem_t) SYS_SEM_NULL) ? 0 : 1);
}

void
sys_sem_set_invalid(sys_sem_t *sem)
{
  *sem = (sys_sem_t) SYS_SEM_NULL;
}

err_t
sys_mbox_new(sys_mbox_t *mbox, int size)
{
  T_CMBX cmbx;
  ID mboxid;
  
  cmbx.exinf = NULL;
  cmbx.mbxatr = TA_TFIFO | TA_MFIFO;
  mboxid = tk_cre_mbx(&cmbx);
  if(mboxid < E_OK){
    return ERR_MEM;
  }
  *mbox = mboxid;
  
  return ERR_OK;
}

void
sys_mbox_free(sys_mbox_t *mbox)
{
  tk_del_mbx(*mbox);
}

void
sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
  LWIP_MBX *p;

  tk_get_mpf(sys_mbx_pool, (void **)&p, TMO_FEVR);
  
  p->msg = msg;
  tk_snd_mbx(*mbox, (T_MSG *) p);
}

err_t
sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
  LWIP_MBX *p;

  if(tk_get_mpf(sys_mbx_pool, (void **) &p, TMO_POL) < E_OK){
    return ERR_MEM;
  }
  
  p->msg = msg;
  tk_snd_mbx(*mbox, (T_MSG *) p);
  return ERR_OK;
}

u32_t
sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
  ER ercd;
  LWIP_MBX *p;
  u32_t sttim = sys_now();
  
  ercd = tk_rcv_mbx(*mbox, (T_MSG **)&p, timeout > 0 ? ((W) timeout) : TMO_FEVR);
  if(ercd < E_OK){
    return SYS_MBOX_EMPTY;
  }
  *msg = p->msg;
  tk_rel_mpf(sys_mbx_pool, p);
  
  return (sys_now() - sttim);
}

u32_t
sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
  ER ercd;
  LWIP_MBX *p;
  ercd = tk_rcv_mbx(*mbox, (T_MSG **)&p, TMO_POL);
  
  if(ercd < E_OK){
    return SYS_MBOX_EMPTY;
  }
  *msg = p->msg;
  tk_rel_mpf(sys_mbx_pool, p);
  
  return 0;
}

int
sys_mbox_valid(sys_mbox_t *mbox)
{
    return ((*mbox == (sys_mbox_t) SYS_MBOX_NULL) ? 0 : 1);
}

void
sys_mbox_set_invalid(sys_mbox_t *mbox)
{
  *mbox = (sys_mbox_t) SYS_MBOX_NULL;
}

static void
thread_task ( INT stacd , void * exinf )
{
  ((FP) stacd)(exinf);
  
  tk_exd_tsk();
}

sys_thread_t
sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
  T_CTSK ctsk;
  ID tskid;
  
  ctsk.exinf = (void *) arg;
  ctsk.tskatr = TA_HLNG | TA_RNG0;
  ctsk.task = (FP) thread_task;
  ctsk.itskpri = (prio > 0) ? prio : DEFAULT_THREAD_PRIO;
  ctsk.stksz = (stacksize > 0) ? stacksize : DEFAULT_THREAD_STACKSIZE;
  tskid = tk_cre_tsk(&ctsk);
  if(tskid < E_OK){
    return ERR_MEM;
  }
  
  tk_sta_tsk(tskid, (INT) thread);
  return tskid;
}

u32_t
sys_now(void)
{
  SYSTIM tim;
  
  tk_get_tim(&tim);
  
  return tim.lo;
}

sys_prot_t 
sys_arch_protect(void)
{
  if(sys_prot_lock.ownertask != tk_get_tid()){
    Lock(&sys_prot_lock.lock);
    sys_prot_lock.ownertask = tk_get_tid();
  }
  
  return sys_prot_lock.ownercnt++;
}

void 
sys_arch_unprotect(sys_prot_t pval)
{
  if(sys_prot_lock.ownertask == tk_get_tid()){
    if(--sys_prot_lock.ownercnt == 0){
      sys_prot_lock.ownertask = 0;
      Unlock(&sys_prot_lock.lock);
    }
  }
}
