/*
 *----------------------------------------------------------------------
 *    UCT micro T-Kernel Version 2.00.18
 *
 *    Copyright (c) 2013-2016 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Released by UC Technology (http://www.uctec.com/) at 2013/06/10.
 *    Modified by UC Technology at 2015/06/04.
 *    Modified by UC Technology at 2016/10/24.
 *
 *----------------------------------------------------------------------
 */

#include "net_ether.h"
#include <sys/queue.h>
#include <string.h>
#include "../../net_stat.h"
#include "net_cnf_sysdep.h"
#include "net_w5500.h"

LOCAL const char *ether_devname[] = {
	ETHER_DEVINFO_DEVNAME "Ether_Shield",
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


void dma_notify_tx_end(void)
{
	tk_set_flg(ethercb[0].flgid, ETHER_FLGPTN_DMA_TX);
}

void dma_notify_rx_end(void)
{
	tk_set_flg(ethercb[0].flgid, ETHER_FLGPTN_DMA_RX);
}

void dma_notify_error(void)
{
	tk_set_flg(ethercb[0].flgid, ETHER_FLGPTN_DMA_ERROR);
}

int dma_wait_tx(void){
	INT ercd;
	UINT flgptn;
	
	ercd = tk_wai_flg(ethercb[0].flgid, ETHER_FLGPTN_DMA_TX | ETHER_FLGPTN_DMA_ERROR, 
					  TWF_ORW | TWF_BITCLR, &flgptn, ETHER_SPI_DMA_TMOUT);
	if (ercd < E_OK || (flgptn & ETHER_FLGPTN_DMA_ERROR)) {
		return -1;
	}
	
	return 0;
}

int dma_wait_rx(void){
		INT ercd;
	UINT flgptn;
	
	ercd = tk_wai_flg(ethercb[0].flgid, ETHER_FLGPTN_DMA_RX | ETHER_FLGPTN_DMA_ERROR, 
					  TWF_ORW | TWF_BITCLR, &flgptn, ETHER_SPI_DMA_TMOUT);
	if (ercd < E_OK || (flgptn & ETHER_FLGPTN_DMA_ERROR)) {
		return -1;
	}
	
	return 0;
}

/*
 * Task for receiving packets.
 */
LOCAL void ether_recv_task( INT stacd, void *exinf );
/*
 * Initializes a network adaptor.
 */
LOCAL ER ether_init_hw( INT ch );

LOCAL void ether_lock( INT ch )
{
	Lock( &ethercb[ch].lock );
}

LOCAL void ether_unlock( INT ch )
{
	Unlock( &ethercb[ch].lock );
}

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
	ctsk.task    = ether_recv_task;
	ctsk.itskpri = ETHER_RECV_TASK_PRIORITY;
	ctsk.stksz   = ETHER_RECV_TASK_STASK_SIZE;
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
	T_DINT	dint;

	ether_lock( ch );
	w5500_init(ethercb[ch].macaddr.c, ethercb[ch].promiscuous_mode ? 1 : 0);
	ether_unlock( ch );
	
	// Enable interrupt for SPI1 TX RX DMA.
	dint.intatr = TA_HLNG;
	dint.inthdr = dma_rx_int_handler;
	tk_def_int(INTNO_DMA1_CHANNEL2, &dint);
	EnableInt(INTNO_DMA1_CHANNEL2, DEVCNF_SPI1_DMA_INTPRI);
	
	dint.inthdr = dma_tx_int_handler;
	tk_def_int(INTNO_DMA1_CHANNEL3, &dint);
	EnableInt(INTNO_DMA1_CHANNEL3, DEVCNF_SPI1_DMA_INTPRI);

	// Trick: Wait for PHY Connection status update.
	tk_dly_tsk(3000);

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
LOCAL void ether_recv_task( INT stacd, void *exinf )
{
	UINT flgptn;
	NetEvent event;
	ER ercd;
	UB status;
	INT ch = stacd;

	/* Main loop. */
	for(;;) {
		/* Waits for notifications. */
		ercd = tk_wai_flg( ethercb[ch].flgid, ETHER_FLGPTN_FATAL_ERR,
				   TWF_ORW | TWF_BITCLR, &flgptn, ETHER_POLLING_INTERVAL );

		/* Reset DMA and SPI and W5500 for fatal error. */
		if( ercd >= E_OK && (flgptn & ETHER_FLGPTN_FATAL_ERR) != 0) {
			ether_init_hw(ch);
			
			/* Cancel all transmit wait. */
			tk_set_flg(ethercb[ch].flgid, ETHER_FLGPTN_SEND);
			tk_wai_flg(ethercb[ch].flgid, ETHER_FLGPTN_SEND, 
			   TWF_ORW | TWF_BITCLR, &flgptn, TMO_POL);
			
			continue;
		}

		ether_lock( ch );
		ercd = w5500_get_status(&status);
		ether_unlock( ch );
		
		
		// Check send status.
		if (ethercb[ch].send_wait && (status & W5500_Sn_IR_SEND_OK)) {
			ethercb[ch].send_wait = FALSE;
			tk_set_flg(ethercb[ch].flgid, ETHER_FLGPTN_SEND);
		}
		
		if (status & W5500_Sn_IR_RECV) {
			
			ether_lock( ch );
			/* Retrives all received packet. */
			do {
				if (isQueEmpty( &ethercb[ch].freeq ) == FALSE) {
					event.buf = (UB *) QueRemoveNext( &ethercb[ch].freeq );
					
					ercd = w5500_recv(event.buf, ethercb[ch].bufsz.maxsz, &event.len);
					if (ercd < 0) {
						/* When receiv error occurred. */
						QueInsert((QUEUE *) event.buf, &ethercb[ch].freeq);
						break;
					}
					
					netdrv_stat_inc_all_rcvcnt( ch );
					if( ethercb[ch].mbfid <= 0 ) {
						/* No one cares!!! Discard the packet. */
						QueInsert((QUEUE *) event.buf, &ethercb[ch].freeq);
						continue;
					}
					
					/* Check a message size */
					if( event.len < ethercb[ch].bufsz.minsz ||
						event.len > ethercb[ch].bufsz.maxsz ) {
						/* Discard the packet. */
						QueInsert((QUEUE *) event.buf, &ethercb[ch].freeq);
						continue;
					}
					
					ercd = tk_snd_mbf( ethercb[ch].mbfid, &event, sizeof( NetEvent ), TMO_POL );
					if( ercd < 0 ) {
						QueInsert((QUEUE *) event.buf, &ethercb[ch].freeq);
						continue;
					}
					/* Increment a counter. */
					netdrv_stat_inc_rcvcnt( ch );
				}
				else {
					break;
				}

				ercd = w5500_get_status(&status);
				if (ercd != 0) {
					break;
				}
			} while(status & W5500_Sn_IR_RECV);
			
			ether_unlock( ch );
		}
	}
}

ER ether_send( INT ch, const void * buf, INT len, TMO tmout )
{
	UINT flgptn;
	ER ercd;
	NetEvent event;
	int retval;

	event.buf = NULL;
	event.len = 0;

	ether_lock( ch );
	retval = w5500_send(buf, len);
	ether_unlock( ch );
	
	if (retval == 0) {
		ethercb[ch].send_wait = TRUE;
		/* Wait for completing a transmission of the packet. */
		ercd = tk_wai_flg( ethercb[ch].flgid, ETHER_FLGPTN_SEND, 
						   TWF_ORW | TWF_BITCLR, &flgptn, tmout );
		if( ercd >= E_OK ) {
			tk_snd_mbf( ethercb[ch].mbfid, &event, sizeof( NetEvent ), TMO_POL );
			return len;
		}
	}
	else {
		ercd = E_IO;
	}
	
	return ercd;

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
	devinfo->stat   = ether_check_link( ch ) ? 0 : -1;

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
	ether_lock( ch );
	if( buf ) {
		QueInsert((QUEUE *) buf, &ethercb[ch].freeq);
	}
	else {
		QueInit(&ethercb[ch].freeq);
	}
	ether_unlock( ch );

	return sizeof( void * );
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
	/* Check a paramter */
	if( !bufsz ) {
		return E_PAR;
	}
	if( bufsz->minsz > bufsz->maxsz ) {
		return E_PAR;
	}

	ethercb[ch].bufsz = *bufsz;
	
	return sizeof( NetRxBufSz );
}

BOOL ether_check_link( INT ch )
{
	int ret;
	
	ether_lock( ch );
	ret = w5500_check_link();
	ether_unlock( ch );
	
	return  ret != 0 ? TRUE : FALSE;
}

/*
 * Enable/Disable Multicast receive.
 */
void ether_set_multicast(INT ch, BOOL enable){
	if (ethercb[ch].promiscuous_mode == FALSE) {
		ether_lock( ch );
		w5500_set_multicast(enable ? 1 : 0);
		ether_unlock( ch );
	}
}
