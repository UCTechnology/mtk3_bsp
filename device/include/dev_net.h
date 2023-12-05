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

#ifndef __DEVINC_NET_H__
#define __DEVINC_NET_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Attribute Data */
typedef enum {
	DN_NETEVENT		= -100,	/* Event notification message buffer ID */
	DN_NETRESET		= -103,	/* Reset */
	DN_NETADDR		= -105,	/* Network physical address */
	DN_NETDEVINFO		= -110,	/* Network device information */
	DN_NETSTINFO		= -111,	/* Network statistics information */
	DN_NETCSTINFO		= -112,	/* Clears network statistics information */
	DN_NETRXBUF		= -113,	/* Receive buffer */
	DN_NETRXBUFSZ		= -114,	/* Size of receive buffer */
	DN_SET_MCAST_LIST	= -115,	/* Multicast setting */
	DN_SET_ALL_MCAST	= -116,	/* All multicast settings */
	DN_NETWLANCONFIG	= -130,	/* Wireless LAN settings */
	DN_NETWLANSTINFO	= -131,	/* Gets line information for wireless LAN */
	DN_NETWLANCSTINFO	= -132,	/* Clears wireless LAN line information */
} NetDrvDataNo;

/*
 * DN_NETEVENT
 */
typedef struct {
	UH	len;	/* Number of bytes of received data. */
	void	*buf;	/* Receive buffer address */
} NetEvent;

/*
 * DN_NETADDR
 */
typedef	struct	{
	UB	c[6];
} NetAddr;

/*
 * DN_NETDEVINFO
 */
#define	L_NETPNAME	(40)		/* Product name length */

typedef	struct {
	UB	name[L_NETPNAME];	/* Product name (ASCII) */
	UW	iobase;			/* IO start address */
	UW	iosize;			/* IO size */
	UW	intno;			/* Interrupt number */
	UW	kind;			/* Index by hardware type */
	UW	ifconn;			/* Connector */
	W	stat;			/* Operating status (>=0: Normal) */
} NetDevInfo;

/* Connector */
#define	IFC_UNKNOWN	(0)		/* Unknown / Default */
#define	IFC_AUI		(1)		/* AUI (10 Base 5) */
#define	IFC_TPE		(2)		/* TPE (10 Base T) */
#define	IFC_BNC		(3)		/* BNC (10 Base 2) */
#define	IFC_100TX	(4)		/* 100 Base TX */
#define	IFC_100FX	(5)		/* 100 Base FX */
#define	IFC_AUTO	(6)		/* Auto */

/*
 * DN_NETSTINFO
 * DN_NETCSTINFO
 */
typedef struct	{
	UW	rxpkt;			/* Number of packets received */
	UW	rxerr;			/* Receive error rate */
	UW	misspkt;		/* Number of discarded packets */
	UW	invpkt;			/* Number of invalid packets */
	UW	txpkt;			/* Number of sent (requested) packets */
	UW	txerr;			/* Transmission error rate */
	UW	txbusy;			/* Number of instances transmission was busy */
	UW	collision;		/* Number of collisions */
	UW	nint;			/* Number of interrupts */
	UW	rxint;			/* Number of receive interrupts */
	UW	txint;			/* Number of transmission interrupts */
	UW	overrun;		/* Number of hardware overruns */
	UW	hwerr;			/* Number of hardware errors */
	UW	other[3];		/* Other information */
} NetStInfo;

/*
 * DN_NETRXBUFSZ
 */
typedef	struct {
	W	minsz;			/* Minimum receive packet size */
	W	maxsz;			/* Maximum receive packet size */
} NetRxBufSz;

/*
 * DN_NETWLANCONFIG
 */ 
#define	WLAN_SSID_LEN	32		/* Maximum SSID length */
#define	WLAN_WEP_LEN	16		/* Maximum WEP key length */

typedef	struct {
	W	porttype;		/* Network type (rw) */
	W	channel;		/* Channel used (rw) */
	W	ssidlen;		/* SSID length (in bytes) (rw) */
	UB	ssid[WLAN_SSID_LEN];	/* SSID (rw) */
	W	wepkeylen;		/* WEP key length (in bytes) (rw) */
	UB	wepkey[WLAN_WEP_LEN];	/* WEP key (wo) */
	W	systemscale;		/* Sensitivity (rw) */
	W	fragmentthreshold;	/* Fragment threshold (rw) */
	W	rtsthreshold;		/* RTS threshold (rw) */
	W	txratecontrol;		/* Transmission rate (rw) */
	UW	function;		/* Extended functions (ro) */
	UW	channellist;		/* Available channels (ro) */
} WLANConfig;


/*
 * DN_NETWLANSTINFO
 * DN_NETWLANCSTINFO
 */
typedef struct {
	UB	ssid[WLAN_SSID_LEN + 2];/* Destination SSID */
	UB	bssid[6];		/* Destination BSSID */
	W	channel;		/* Current channel */
	W	txrate;			/* Transmission rate (kbps) */
	W	quality;		/* Line quality */
	W	signal;			/* Signal level */
	W	noise;			/* Noise level */
	UW	misc[16];		/* Extended statistics information */
} WLANStatus;

#ifdef __cplusplus
}
#endif
#endif /* __DEVINC_NET_H__ */
