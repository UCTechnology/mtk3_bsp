/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.0
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2023/11/22
 * 
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */


/*
 *	ser_cnf_sysdep.h 
 *	Serial Device configuration file
 *		for XMC7200
 */
#ifndef	__DEV_SER_CNF_XMC7200_H__
#define	__DEV_SER_CNF_XMC7200_H__

/* Interrupt priority */
#define DEVCNF_UART_INTPRI	(3)
#define DEVCNF_UART0_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART1_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART2_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART3_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART4_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART5_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART6_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART7_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART8_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART9_INTPRI	(DEVCNF_UART_INTPRI)
#define DEVCNF_UART10_INTPRI	(DEVCNF_UART_INTPRI)

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  -1 : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define DEVCNF_SER_DBGUN	3		// SCB3 is used by T-Monitor
#else 
#define DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif	/* __DEV_SER_CNF_XMC7200_H__ */
