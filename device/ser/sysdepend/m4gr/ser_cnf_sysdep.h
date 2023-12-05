/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the ADBUN-M4GR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	ser_cnf_sysdep.h 
 *	Serial Device configuration file
 *		for TMPM4GR
 */
#ifndef	__DEV_SER_CNF_M4GR_H__
#define	__DEV_SER_CNF_M4GR_H__

/* Interrupt priority */
#define	DEVCNF_UART0_INTPRI	6
#define	DEVCNF_UART1_INTPRI	6
#define	DEVCNF_UART2_INTPRI	6
#define	DEVCNF_UART3_INTPRI	6

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  -1 : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	0		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif	/* __DEV_SER_CNF_M4GR_H__ */
