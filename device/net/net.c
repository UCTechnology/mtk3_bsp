/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */
#include "net.h"
#include <string.h>
#include "net_util.h"

/*
 * Check if a parameter is valid for a read request.
 *
 * This function returns E_OK if the buffer size (req->size) is greater
 * than or equal to the required buffer size, return the required buffer
 * size if the buffer size is 0, otherwise returns E_PAR.
 *
 * @param[in] req  Device request.
 * @param[in] size Required buffer size.
 *
 * @retval >0       Required buffer size if req->size is 0.
 * @retval E_OK(=0) There are sufficient space in the buffer.
 * @retval E_PAR    Parameter error.
 */
LOCAL ER netdrv_check_read_param( T_DEVREQ * req, INT size );

/*
 * Check if a parameter is valid for a write request.
 *
 * This function returns E_OK if the buffer size (req->size) is equal
 * to the required buffer size, return the required buffer size if the
 * buffer size is 0, otherwise returns E_PAR.
 *
 * @param[in] req  Device request.
 * @param[in] size Required buffer size.
 *
 * @retval >0       Required buffer size if req->size is 0.
 * @retval E_OK(=0) There are sufficient space in the buffer.
 * @retval E_PAR    Parameter error.
 */
LOCAL ER netdrv_check_write_param( T_DEVREQ * req, INT size );

/*
 * Process a read device request.
 *
 * If req->size is zero, this function just returns the required buffer
 * size to read data.
 *
 * @param[in]     net device channel no.
 * @param[in,out] req Device request.
 *
 * @retval >=0     Used buffer size in req->buf to read data.
 * @retval E_NOSPT Specified command is not supported.
 * @retval <0      Error.
 */
LOCAL ER netdrv_proc_read_request( INT ch, T_DEVREQ * req );

/*
 * Process a write device request.
 *
 * If req->size is zero, this function just returns the required buffer
 * size to write data.
 *
 * @param[in]     net device channel no.
 * @param[in,out] req Device request.
 *
 * @retval >=0     Used buffer size in req->buf to write data.
 * @retval E_NOSPT Specified command is not supported.
 * @retval <0      Error.
 */
LOCAL ER netdrv_proc_write_request( INT ch, T_DEVREQ * req );

/*
 * Process a packet transmission.
 *
 * This function returns error code E_NOSPT when req->size is 0,
 * since 'the current size of data that can be written' is not clear for
 * a packet transmission.
 *
 * @param[in]     net device channel no.
 * @param[in,out] req Device request.
 *
 * @retval >=0     Transmitted packet size.
 * @retval E_NOSPT If req->size is zero.
 * @retval <0      Error.
 * 
 */
LOCAL ER netdrv_proc_send( INT ch, T_DEVREQ * req );

/*
 * Reset.
 */
LOCAL ER netdrv_proc_reset( INT ch, W data );

/*
 * Main function of the request acceptance task.
 *
 * @param[in]     stacd  Always zero.
 * @param[in,out] exinf  GDI.
 */
LOCAL void netdrv_accept_task( INT stacd, void *exinf );

/*
 * Defines this network device driver.
 *
 * @retval gdi   A pointer to GDI.
 * @retval NULL  Error.
 */
LOCAL GDI netdrv_define( INT ch, const char * str_addr );

/*
 * Open function used in the device driver interface library.
 */
LOCAL ER netdrv_openfn( ID devid, UINT omode, GDI gdi );
/*
 * Close function used in the device driver interface library.
 */
LOCAL ER netdrv_closefn( ID devid, UINT option, GDI gdi );
/*
 * Abort function used in the device driver interface library.
 */
LOCAL ER netdrv_abortfn( T_DEVREQ *req, GDI gdi );
/*
 * Event function used in the device driver interface library.
 */
LOCAL ER netdrv_eventfn( INT evttyp, void *evtinf, GDI gdi );

LOCAL ER netdrv_check_read_param( T_DEVREQ * req, INT size )
{
	if( req->size == 0 ) {
		return size;
	}
	else if( req->size < size ) {
		return E_PAR;
	}

	return E_OK;
}

LOCAL ER netdrv_check_write_param( T_DEVREQ * req, INT size )
{
	if( req->size == 0 ) {
		return size;
	}
	else if( req->size != size ) {
		return E_PAR;
	}

	return E_OK;
}

LOCAL ER netdrv_proc_read_request( INT ch, T_DEVREQ * req )
{
	ER ercd;

	switch( req->start ) {
	case DN_NETEVENT:
		ercd = netdrv_check_read_param( req, sizeof( ID ) );
		if( ercd == E_OK ) {
			ercd = ether_get_msgbuf( ch, (ID *)req->buf );
		}
		break;
	case DN_NETRESET:
		ercd = netdrv_check_read_param( req, sizeof( W ) );
		if( ercd == E_OK ) {
			netdrv_proc_reset( ch, *((W *)req->buf) );
		}
		break;
	case DN_NETADDR:
		ercd = netdrv_check_read_param( req, sizeof( NetAddr ) );
		if( ercd == E_OK ) {
			ercd = ether_get_phyaddr( ch,  (NetAddr *)req->buf );
		}
		break;
	case DN_NETDEVINFO:
		ercd = netdrv_check_read_param( req, sizeof( NetDevInfo ) );
		if( ercd == E_OK ) {
			ercd = ether_get_devinfo( ch, (NetDevInfo *)req->buf );
		}
		break;
	case DN_NETSTINFO:
		ercd = netdrv_check_read_param( req, sizeof( NetStInfo ) );
		if( ercd == E_OK ) {
			ercd = ether_get_stinfo( ch, (NetStInfo *)req->buf );
		}
		break;
	case DN_NETCSTINFO:
		ercd = netdrv_check_read_param( req, sizeof( NetStInfo ) );
		if( ercd == E_OK ) {
			ercd = ether_get_stinfo( ch, (NetStInfo *)req->buf );
			ether_clear_stinfo( ch );
		}
		break;
	case DN_NETRXBUFSZ:
		ercd = netdrv_check_read_param( req, sizeof( NetRxBufSz ) );
		if( ercd == E_OK ) {
			ercd = ether_get_rxbuf_size( ch, (NetRxBufSz *)req->buf );
		}
		break;
	case DN_NETWLANCONFIG:
		/* NOT SUPPORTED */
		ercd = E_NOSPT;
		break;
	case DN_NETWLANSTINFO:
		/* NOT SUPPORTED */
		ercd = E_NOSPT;
		break;
	case DN_NETWLANCSTINFO:
		/* NOT SUPPORTED */
		ercd = E_NOSPT;
		break;
	default:
		ercd = E_NOSPT;
	}

	return ercd;
}

LOCAL ER netdrv_proc_send( INT ch, T_DEVREQ * req )
{
	/*
	 * This function MUST BE called from netdrv_proc_write_request()
	 * since this doesn't check the case that req->size is less than 0.
	 */
	if( req->size == 0 ) {
		return ETH_MAX_DATA_LENGTH;
	}
	else {
		return ether_send( ch, req->buf, req->size, TMO_FEVR );
	}
}

LOCAL ER netdrv_proc_write_request( INT ch, T_DEVREQ * req )
{
	ER ercd;

	switch( req->start ) {
	case 0:
		/**/
		ercd = netdrv_proc_send( ch, req );
		break;
	case 1:
		/* NOT SUPPORTED */
		ercd = E_NOSPT;
		break;
	case DN_NETEVENT:
		ercd = netdrv_check_write_param( req, sizeof( ID ) );
		if( ercd == E_OK ) {
			ercd = ether_set_msgbuf( ch, *((ID *)req->buf) );
		}
		break;
	case DN_NETRESET:
		ercd = netdrv_check_write_param( req, sizeof( W ) );
		if( ercd == E_OK ) {
			ercd = netdrv_proc_reset( ch, *((W *)req->buf) );
		}
		break;
	case DN_NETRXBUF:
		ercd = netdrv_check_write_param( req, sizeof( void * ) );
		if( ercd == E_OK ) {
			ercd = ether_set_rxbuf( ch, *((void**)req->buf) );
		}
		break;
	case DN_NETRXBUFSZ:
		/* NOT SUPPORTED */
		ercd = E_NOSPT;
		ercd = netdrv_check_write_param( req, sizeof( NetRxBufSz ) );
		if( ercd == E_OK ) {
			ercd = ether_set_rxbuf_size( ch, (NetRxBufSz *)req->buf );
		}
		break;
	case DN_SET_MCAST_LIST:
		if(req->size == 0) {
			ether_set_multicast(ch, FALSE);
		}
		else {
			ether_set_multicast(ch, TRUE);
		}
		ercd = E_OK;
		break;
	case DN_SET_ALL_MCAST:
		ether_set_multicast(ch, TRUE);
		ercd = E_OK;
		break;
	case DN_NETWLANCONFIG:
		/* NOT SUPPORTED */
		ercd = E_NOSPT;
		break;
	default:
		ercd = E_NOSPT;
	}

	return ercd;
}

LOCAL ER netdrv_proc_reset( INT ch, W data )
{
	T_IDEV idev;
	ER ercd;
	
	ercd = tk_ref_idv( &idev );
	if( ercd < 0 ) {
		return ercd;
	}

	return ether_reset( ch, data, idev.evtmbfid );
}

LOCAL void netdrv_accept_task( INT stacd, void *exinf )
{
	T_DEVREQ *req;
	GDI gdi;
	ER ercd;
	INT ch = stacd;

	gdi = (GDI)exinf;

	for(;;) {
		/* Wait for a request */
		ercd = GDI_Accept( &req, DRP_NORMREQ | DRP_USERCMD, TMO_FEVR, gdi );
		if( ercd < 0 ) {
			continue;
		}

		/*
		 * Process a request.
		 */

		/* Check if there is a user command. */
		if( ercd & DRP_USERCMD ) {
			if( ercd & NETDRV_USERCMD_STOP ) {
				/* Stop all process */
				break;
			}
		}

		/* Do nothing when there is no device request object. */
		if( req ) {
			if( req->size < 0 ) {
				ercd = E_PAR;
			}
			else if( req->cmd == TDC_READ ) {
				ercd = netdrv_proc_read_request( ch, req);
			}
			else if( req->cmd == TDC_WRITE ) {
				ercd = netdrv_proc_write_request( ch, req );
			}

			/* Set return values */
			if( ercd < 0 ) {
				req->asize = 0;
				req->error = ercd;
			}
			else {
				req->asize = ercd;
				req->error = E_OK;
			}
		}
		
		/* Reply to the request. */
		if( req ) {
			GDI_Reply( req, gdi );
		}
	}
}

LOCAL GDI netdrv_define( INT ch, const char * str_addr )
{
	GDefDev ddev;
	T_IDEV idev;
	T_CTSK ctsk;
	GDI gdi;
	ID  tskid;
	ER ercd;
	NetAddr macaddr;

	/*
	 * Convert a mac address in string representation to a NetAddr 
	 * type value.
	 */
	ercd = netdrv_str2addr( str_addr, &macaddr );
	if( ercd < 0 ) {
		return NULL;
	}

	/* Initialize a network device. */
	ercd = ether_init( ch, &macaddr, NETDRV_MII_MODE, NETDRV_PROMISCUOUS_MODE );
	if( ercd < 0 ) {
		return NULL;
	}

	/* Set parameters for GDefDevice. */
	ddev.exinf = NULL;
	strcpy( (char *)ddev.devnm, NET_DEVNM);
	ddev.devnm[sizeof(NET_DEVNM) - 2] += ch;		/* Neta, Netb ... */
	ddev.maxreqq = NET_MAXREQQ;
	ddev.drvatr = 0;
	ddev.devatr = 0;
	ddev.nsub = 0;
	ddev.blksz = 1;
	ddev.open = netdrv_openfn;
	ddev.close = netdrv_closefn;
	ddev.abort = netdrv_abortfn;
	ddev.event = netdrv_eventfn;

	/* Register this device driver. */
	ercd = GDefDevice( &ddev, &idev, &gdi );
	if( ercd < 0 ) {
		return NULL;
	}

	/* mbfid which may be created during initializing. */
	ether_set_msgbuf( ch, idev.evtmbfid );

	/* Create and start the request acceptance task. */
	ctsk.exinf   = gdi;
	ctsk.tskatr  = TA_HLNG | TA_RNG0;
	ctsk.task    = netdrv_accept_task;
	ctsk.itskpri = NET_TASKPRI;
	ctsk.stksz   = NET_TASKSTKSZ;
	ercd = tk_cre_tsk( &ctsk );
	if( ercd < 0 ) {
		return NULL;
	}
	tskid = ercd;
	ercd = tk_sta_tsk( tskid, ch );
	if( ercd < 0 ) {
		return NULL;
	}

	return gdi;
}

LOCAL ER netdrv_openfn( ID devid, UINT omode, GDI gdi )
{
	/*
	 * Nothing to do
	 */
	return E_OK;
}

LOCAL ER netdrv_closefn( ID devid, UINT option, GDI gdi )
{
	/*
	 * Nothing to do
	 */
	return E_OK;
}

/*
 * This function IS NOT implemented.
 */
LOCAL ER netdrv_abortfn( T_DEVREQ *req, GDI gdi )
{
	/*
	 * Wake up the task if the task is processing the corresponding
	 * request and the task is waiting.
	 */

	return E_OK;
}

LOCAL ER netdrv_eventfn( INT evttyp, void *evtinf, GDI gdi )
{
	
	if( GDI_exinf( gdi ) ) {
	}

	switch( evttyp ) {
	case TDV_SUSPEND:
		/* Suspend event */
		/*
		 * Current implementation DOES NOT support TDV_SUSPEND and 
		 * just returns E_OK.
		 */
		return E_OK;
	case TDV_RESUME:
		/* Resume event */
		/*
		 * Current implementation DOES NOT support TDV_RESUME and 
		 * just returns E_OK.
		 */
		return E_OK;
	default:
		/* Undefined event */
		return E_NOSPT;
	}
}

EXPORT ER NetDrvEntry( INT ac, UB *av[] )
{
	static GDI gdi = NULL;
	ER er = 0;

	if( ac < 0 ) {
		/* Unregister */

		/*
		 * Not implemented.
		 */
		return E_NOSPT;
	}

	/* Check parameters. */
	if( ac < 1 ) {
		return E_PAR;
	}

	if ( ac > 0 && av[0] != NULL ) { 
		/* Define net driver channel 0 into the system. */
		gdi = netdrv_define( 0, (const char *)av[0] );
		if( !gdi ) {
			return E_SYS;
		}
		
		if( !ether_check_link(0) ) {
			er |= (1 << 0);
		}
	}
	
	if ( ac > 1 &&  av[1] != NULL ) {
		/* Define net driver channel 1 into the system. */
		gdi = netdrv_define( 1, (const char *)av[1] );
		if( !gdi ) {
			return E_SYS;
		}
		
		if( !ether_check_link(1) ) {
			er |= (1 << 1);
		}
	}
	
	return er;
}
