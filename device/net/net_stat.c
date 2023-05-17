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
#include "net.h"
#include "net_stat.h"
#include <string.h>

LOCAL NETDRV_STAT netdrv_stat[ETHERC_MAX_CHANNEL];

void netdrv_stat_init( INT ch )
{
	memset( &netdrv_stat[ch], 0, sizeof( NETDRV_STAT ) );
}

ER netdrv_stat_get( INT ch, NetStInfo *stinfo )
{
	if( stinfo == NULL ) {
		return E_PAR;
	}

	stinfo->rxpkt = netdrv_stat[ch].rcvcnt;
	stinfo->rxerr = netdrv_stat[ch].rxerr;
	stinfo->misspkt = netdrv_stat[ch].all_rcvcnt - netdrv_stat[ch].rcvcnt;
	stinfo->invpkt = netdrv_stat[ch].invpkt;
	stinfo->txpkt = netdrv_stat[ch].txpkt;
	stinfo->txerr = netdrv_stat[ch].txerr;
	stinfo->txbusy = netdrv_stat[ch].txbusy;
	stinfo->collision = netdrv_stat[ch].collision;
	stinfo->nint = netdrv_stat[ch].rxint + netdrv_stat[ch].txint;
	stinfo->rxint = netdrv_stat[ch].rxint;
	stinfo->txint = netdrv_stat[ch].txint;
	stinfo->overrun = netdrv_stat[ch].overrun;
	stinfo->hwerr = 0;
	stinfo->other[0] = 0;
	stinfo->other[1] = 0;
	stinfo->other[2] = 0;

	return E_OK;
}

void netdrv_stat_inc_rcvcnt( INT ch )
{
	netdrv_stat[ch].rcvcnt++;
}

void netdrv_stat_inc_all_rcvcnt( INT ch )
{
	netdrv_stat[ch].all_rcvcnt++;
}

void netdrv_stat_inc_rxint( INT ch )
{
	netdrv_stat[ch].rxint++;
}

void netdrv_stat_inc_txint( INT ch )
{
	netdrv_stat[ch].txint++;
}

void netdrv_stat_inc_txerr( INT ch )
{
	netdrv_stat[ch].txerr++;
}

void netdrv_stat_inc_overrun( INT ch )
{
	netdrv_stat[ch].overrun++;
}

void netdrv_stat_add_collision( INT ch, INT collision )
{
	netdrv_stat[ch].collision += collision;
}

void netdrv_stat_inc_txbusy( INT ch )
{
	netdrv_stat[ch].txbusy++;
}

void netdrv_stat_inc_invpkt( INT ch )
{
	netdrv_stat[ch].invpkt++;
}

void netdrv_stat_inc_txpkt( INT ch )
{
	netdrv_stat[ch].txpkt++;
}

void netdrv_stat_inc_rxerr( INT ch )
{
	netdrv_stat[ch].rxerr++;
}
