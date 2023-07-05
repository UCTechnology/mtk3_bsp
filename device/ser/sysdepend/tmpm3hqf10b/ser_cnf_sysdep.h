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
 *	ser_cnf_sysdep.h 
 *	Serial Device configuration file
 *		for ADBUN_TMPM3HQF10B (TMPM3HQF10B)
 */
#ifndef	__DEV_SER_CNF_TMPM3HQF10B_H__
#define	__DEV_SER_CNF_TMPM3HQF10B_H__

/* Interrupt priority */
#define	DEVCNF_UART0_RX_INTPRI	5
#define	DEVCNF_UART0_TX_INTPRI	5
#define	DEVCNF_UART0_ER_INTPRI	5

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  0: "sera" - UART0
 *	  other : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	0		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif		/* __DEV_SER_CNF_TMPM3HQF10B_H__ */
