/*
 *----------------------------------------------------------------------
 *    UCT micro T-Kernel Version 2.00.12
 *
 *    Copyright (c) 2013-2015 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Released by UC Technology (http://www.uctec.com/) at 2013/06/10.
 *    Modified by UC Technology at 2015/06/04.
 *
 *----------------------------------------------------------------------
 */

#ifndef __NETDRV_STAT_H__
#define __NETDRV_STAT_H__

#include <tk/tkernel.h>
#include <../include/dev_net.h>

typedef struct {
	UW rxint;
	UW txint;
	UW invpkt;
	UW txpkt;
	UW txerr;
	UW txbusy;
	UW rxerr;
	UW overrun;
	UW rcvcnt;
	UW collision;
	UW all_rcvcnt;
} NETDRV_STAT;

void netdrv_stat_init( INT ch );
void netdrv_stat_inc_rcvcnt( INT ch );
void netdrv_stat_inc_all_rcvcnt( INT ch );
void netdrv_stat_inc_rxint( INT ch );
void netdrv_stat_inc_txint( INT ch );
void netdrv_stat_inc_txerr( INT ch );
void netdrv_stat_inc_overrun( INT ch );
void netdrv_stat_inc_txbusy( INT ch );
void netdrv_stat_add_collision( INT ch, INT collision );
void netdrv_stat_inc_invpkt( INT ch );
void netdrv_stat_inc_txpkt( INT ch );
void netdrv_stat_inc_rxerr( INT ch );

ER netdrv_stat_get( INT ch, NetStInfo *stinfo );

#endif /* __NETDRV_STAT_H__ */
