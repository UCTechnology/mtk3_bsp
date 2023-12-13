/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the ADBUN-M4NR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */


#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <sys/queue.h>
#include <string.h>

#include "net_ether.h"
#include "../../net_stat.h"
//#include "net_cnf_sysdep.h"
#include "netdrv_macctrl.h"

#define ETHER_INIT_HW_END_WAIT 200
#define ETHER_CHECK_LINK_RETRY_MAX 1
LOCAL const char *ether_devname[] = {
	ETHER_DEVINFO_DEVNAME
};

typedef struct {
	/*
	 * Receiving task ID.
	 *
	 * This variable will not be changed after initialization until
	 * the device driver is reset.
	 */
	ID tskid; 
	/*
	 * Event-flag for notifying completion of transmission or reception.
	 * Receiving task ID.
	 *
	 * This variable will not be changed after initialization until
	 * the device driver is reset.
	 */
	ID flgid;
	/*
	 * Message buffer for sending a NETEVENT.
	 *
	 * This variable will not be changed after initialization until
	 * the device driver is reset.
	 */
	ID mbfid;
	/*
	 * MII/RMII mode.
	 *
	 * MII mode if this value is TRUE(!=0), otherwise RMII mode.
	 *
	 * This variable will not be changed after initialization until
	 * the device driver is reset.
	 */
	BOOL mii_mode;
	/*
	 * Promiscuous mode.
	 *
	 * Promiscuous mode if this value is TRUE(!=0).
	 */
	BOOL promiscuous_mode;
	/*
	 * MAC address.
	 */
	NetAddr macaddr;
	/*
	 * Receive buffer size.
	 */
	NetRxBufSz bufsz;
	/*
	 * Lock variable
	 */
	FastLock lock;
	/*
	 * Receive buffer queue.
	 */
	QUEUE freeq;
	/*
	 * Is waiting Transmit complete
	 */
	volatile BOOL send_wait;
} ETHER_CONTROL_BLOCK;
LOCAL ETHER_CONTROL_BLOCK ethercb[ETHERC_MAX_CHANNEL];

LOCAL void ether_lock( INT ch )
{
	Lock( &ethercb[ch].lock );
}

LOCAL void ether_unlock( INT ch )
{
	Unlock( &ethercb[ch].lock );
}

/*
 * Task for receiving packets.
 */
//LOCAL void ether_recv_task( INT stacd, void *exinf );
LOCAL void lan_recv_task( INT stacd, void *exinf );
/*
 * Initializes a network adaptor.
 */
LOCAL ER ether_init_hw( INT ch );

ER ether_init( INT ch, const NetAddr * macaddr, BOOL mii_mode, BOOL promiscuous_mode )
{
	T_CTSK ctsk;
	T_CFLG cflg;
	ER ercd;
	UB lock_name[] = {'n', 'e', 't', 'a' + ch, 0};

	/* Check parameter. */
	if( !macaddr ) {
		return E_PAR;
	}
	ethercb[ch].mbfid = -1;
	ethercb[ch].macaddr = *macaddr;
	ethercb[ch].mii_mode = mii_mode;
	ethercb[ch].promiscuous_mode = promiscuous_mode;
	QueInit( &ethercb[ch].freeq );

	/* Creates a task for receiving packets. */
	ctsk.exinf   = NULL;
	ctsk.tskatr  = TA_HLNG | TA_RNG0;
	//ctsk.task    = ether_recv_task;
	if(ch == 0){
		ctsk.task    = lan_recv_task;
	}
	else{
		return E_PAR;
	}
	ctsk.itskpri = ETHER_RECV_TASK_PRIORITY;
	ctsk.stksz   = ETHER_RECV_TASK_STACK_SIZE;
	ercd = tk_cre_tsk( &ctsk );
	if( ercd < 0 ) {
		return ercd;
	}
	ethercb[ch].tskid = ercd;

	/* Creates an event-flag for notifying events. */
	cflg.exinf   = NULL;
	cflg.flgatr  = TA_TPRI | TA_WMUL;
	cflg.iflgptn = 0;
	ercd = tk_cre_flg( &cflg );
	if( ercd < 0 ) {
		return ercd;
	}
	ethercb[ch].flgid = ercd;

	/* Creates a lock. */
	ercd = CreateLock( &ethercb[ch].lock, lock_name );
	if( ercd < 0 ) {
		return ercd;
	}

	/* Initializes hardware. */
	ercd = ether_init_hw( ch );
	if( ercd < 0 ) {
		/* Error occurred. */
		return ercd;
	}

	/* Initialize a counter. */
	netdrv_stat_init( ch );

	/* Starts the receiving task. */
	ercd = tk_sta_tsk( ethercb[ch].tskid, ch );
	if( ercd < 0 ) {
		return ercd;
	}
	
	return E_OK;
}

LOCAL ER ether_init_hw( INT ch )
{
	ER ercd;

	if(ch == 0){
		/* Initializes the Ethernet module. */
		ether_lock( ch );
		ercd = netdrv_macctrl_init( ethercb[ch].flgid, &ethercb[ch].macaddr,
                                              ethercb[ch].mii_mode );
		ether_unlock( ch );
		if( ercd < 0 ) {
			//ether_unlock( ch );
			return ercd;
		}

		/* Enable receiving packets. */
		netdrv_macctrl_start_rx();
		
		/* Enable transmit.  */
		netdrv_macctrl_start_tx();
		//ether_unlock( ch );
	}
	else if(ch == 1){
		return E_PAR;
	}
	// Trick: Wait for PHY Connection status update.
	tk_dly_tsk(ETHER_INIT_HW_END_WAIT);

	return E_OK;
}

/*
 * This task only waits for notifications of packet reception.
 *
 * This task performs as follows.
 *  1. Waits for notifications.
 *  2. Retrives all received packets, then
 *  3. Sends each of them using a message buffer as a NETEVENT.
 */
LOCAL void lan_recv_task( INT stacd, void *exinf )
{
	ID mbfid;
	UINT flgptn;
	NetEvent event;
	ER ercd;
	INT ch = stacd;

	/* Main loop. */
	for(;;) {
		/* Waits for notifications. */
		ercd = tk_wai_flg( ethercb[ch].flgid, ETHER_FLGPTN_RECV,
				   TWF_ORW | TWF_BITCLR, &flgptn, TMO_FEVR );
		if( ercd < 0 ) {
			continue;
		}

		if( (flgptn & ETHER_FLGPTN_RECV) != 0 ) {
			ether_lock(ch);
			mbfid = ethercb[ch].mbfid;

			/* Retrives all received packet. */
			for(;;) {
				ercd = netdrv_macctrl_get_recv_event( &event );
				if( ercd < 0 ) {
					/* When there are no more received packets. */
					break;
				}
				/* Increment a counter */
				netdrv_stat_inc_all_rcvcnt( ch );
				if( mbfid <= 0 ) {
					netdrv_macctrl_enqueue_rxbuf( event.buf );
					continue;
				}

				/* Check a message size */
				if( event.len < ethercb[ch].bufsz.minsz ||
					event.len > ethercb[ch].bufsz.maxsz ) {
					/* Discard the packet. */
					netdrv_macctrl_enqueue_rxbuf( event.buf );
					continue;
				}

				ercd = tk_snd_mbf( mbfid, &event, sizeof( NetEvent ), 
						   TMO_POL );
				if( ercd < 0 ) {
					netdrv_macctrl_enqueue_rxbuf( event.buf );
					continue;
				}
				/* Increment a counter. */
				netdrv_stat_inc_rcvcnt( ch );
			}
			
			ether_unlock(ch);
		}
	}
}

ER ether_send( INT ch, const void * buf, INT len, TMO tmout )
{
	UINT flgptn;
	ER ercd;
	NetEvent event;
//	int retval;

	event.buf = NULL;
	event.len = 0;

	if(ch == 0){
		/* Request for sending a packet. */
		ether_lock( ch );
		ercd = netdrv_macctrl_request_send( buf, len );
		ether_unlock( ch );
		if( ercd < 0 ) {
			return ercd;
		}

		/* Wait for completing a transmission of the packet. */
		ercd = tk_wai_flg( ethercb[ch].flgid, ETHER_FLGPTN_SEND, 
				   TWF_ORW | TWF_BITCLR, &flgptn, tmout );
		if( ercd < 0 ) {
			return ercd;
		}
		else {
			tk_snd_mbf( ethercb[ch].mbfid, &event, sizeof( NetEvent ), TMO_POL );
			return len;
		}
	}
	else{
		return E_PAR;
	}
}

/*
 * Sets event notification message buffer ID.
 */
ER ether_set_msgbuf( INT ch, ID id )
{
	ethercb[ch].mbfid = id;

	return sizeof( ID );
}

/*
 * Gets event notification message buffer ID.
 */
ER ether_get_msgbuf( INT ch, ID * id )
{
	if( !id ) {
		return E_PAR;
	}

	*id = ethercb[ch].mbfid;

	return sizeof( ID );
}


/*
 * Resets the network adaptor.
 *
 * When this function is called, there are no other jobs except for 
 * receiving a packet since this device driver only accept one request
 * at once.
 */
ER ether_reset( INT ch, W data, ID mbfid )
{
	ER ercd;

	/* Reset a contol block. */
	ethercb[ch].mbfid = mbfid;
	ethercb[ch].bufsz.maxsz = ETHER_RXBUF_MAX_SIZE;
	ethercb[ch].bufsz.minsz = ETHER_RXBUF_MIN_SIZE;

	/* Initializes hardware. */
	ercd = ether_init_hw( ch );
	if( ercd < 0 ) {
		/* Error occurred. */
		return ercd;
	}

	/* Reset an event-flag for notifying events. */
	tk_clr_flg( ethercb[ch].flgid, 0 );

	return sizeof(W);
}

/*
 * Gets the ethernet physical address.
 */
ER ether_get_phyaddr( INT ch, NetAddr * addr )
{
	if( !addr ) {
		return E_PAR;
	}

	*addr = ethercb[ch].macaddr;

	return sizeof( NetAddr );
}


/*
 * Gets device informatio non the network adaptor.
 */
ER ether_get_devinfo( INT ch, NetDevInfo * devinfo )
{
	if( !devinfo ) {
		return E_PAR;
	}

	strcpy( (char *)devinfo->name, ether_devname[ch] );
	devinfo->iobase = ETHER_DEVINFO_IOBASE;
	devinfo->iosize = ETHER_DEVINFO_IOSIZE;
	devinfo->intno  = ETHER_DEVINFO_INTNO;
	devinfo->kind   = 0; /* Always 0 */
	devinfo->ifconn = ETHER_DEVINFO_IFCONN;
	if(ch == 0){
		devinfo->stat   = netdrv_macctrl_check_link() ? 0 : -1;
	}
	else{
		return E_PAR;
	}
	return sizeof( NetDevInfo );
}

/*
 * Gets network adaptor statistics information.
 */
ER ether_get_stinfo( INT ch, NetStInfo * stinfo )
{
	ER ercd;

	ercd = netdrv_stat_get( ch, stinfo );
	if( ercd < 0 ) {
		return ercd;
	}

	return sizeof( NetStInfo );
}

/*
 * Clears network adaptor statistics information to zero.
 */
ER ether_clear_stinfo( INT ch )
{
	netdrv_stat_init( ch );

	return E_OK;
}


/*
 * Sets the receive buffer.
 *
 * If buf is NULL, then this function unsets all buffers.
 */
ER ether_set_rxbuf( INT ch, void *buf )
{
	ER ercd;

	if(ch == 0){
		ether_unlock( ch );
		if( buf ) {
			ercd = netdrv_macctrl_enqueue_rxbuf( buf );
		}
		else {
			ercd = netdrv_macctrl_clear_rxbuf();
		}
		ether_unlock( ch );

		if( ercd < 0 ) {
			return ercd;
		}
		else {
			return sizeof( void * );
		}
	}
	else{
		return E_PAR;
	}
}

/*
 * Gets the maximum and minimum sizes of apckets to be received.
 */
ER ether_get_rxbuf_size( INT ch, NetRxBufSz * bufsz )
{
	if( !bufsz ) {
		return E_PAR;
	}

	*bufsz = ethercb[ch].bufsz;

	return sizeof( NetRxBufSz );
}

/*
 * Sets the maximum and minimum sizes of apckets to be received.
 */
ER ether_set_rxbuf_size( INT ch, const NetRxBufSz * bufsz )
{
	ER ercd;

	/* Check a paramter */
	if( !bufsz ) {
		return E_PAR;
	}
	if( bufsz->minsz > bufsz->maxsz ) {
		return E_PAR;
	}

	ethercb[ch].bufsz = *bufsz;
	
	if(ch == 0){
		ethercb[ch].bufsz = *bufsz;
		ercd = netdrv_macctrl_set_rxbuf_size( ethercb[ch].bufsz.maxsz );
		if( ercd < 0 ) {
			return ercd;
		}
	}
	
	return sizeof( NetRxBufSz );
}

BOOL ether_check_link( INT ch )
{
	int ret;
	BOOL rc;
	
	if(ch == 0){
		for( ret = 0; ret < ETHER_CHECK_LINK_RETRY_MAX; ret++){
			tk_dly_tsk(100);
			ether_lock( ch );
			rc = netdrv_macctrl_check_link();
			ether_unlock( ch );
			if(rc == TRUE)
				return TRUE;
		}
		return rc;
	}
	else{
		return FALSE;
	}
}

