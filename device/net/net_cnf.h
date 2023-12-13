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

/*
 *	net_cnf.h 
 *	Ethernet device driver
 *		Device configuration file
 */
#ifndef	__DEV_NET_CNF_H__
#define	__DEV_NET_CNF_H__


#define	DEVCNF_NET_DEVNAME	"neta"		// Device name ("neta")


/*----------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#define DEVDEF_NET_CNF_PATH_(a)	#a
#define DEVDEF_NET_CNF_PATH(a)	DEVDEF_NET_CNF_PATH_(a)
#define DEVDEF_NET_CNF_SYSDEP()	DEVDEF_NET_CNF_PATH(sysdepend/TARGET_CPU_DIR/net_cnf_sysdep.h)
#if 0
#include DEVDEF_NET_CNF_SYSDEP()
#endif

#endif		/* __DEV_NET_CNF_H__ */
