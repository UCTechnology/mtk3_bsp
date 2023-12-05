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
 *	ser_mode_sysdep.h
 *	Serial communication device driver
 *	Communication mode definition for TMPM4KNK
 */

#ifndef	__DEV_SER_MODE_M4GR_H__
#define	__DEV_SER_MODE_M4GR_H__

#define	DEV_SER_MODE_7BIT	(0x00000000)	/* Word length 7bit	UART_CR0_SM_7 */
#define	DEV_SER_MODE_8BIT	(0x00000001)	/* Word length 8bit	UART_CR0_SM_8 */

#define	DEV_SER_MODE_1STOP	(0x00000000)	/* Stop-bit 1bit	UART_CR0_SBLEN_1 */
#define	DEV_SER_MODE_2STOP	(0x00000010)	/* Stop-bit 2bit	UART_CR0_SBLEN_2 */

#define DEV_SER_MODE_PODD	(0x00000004)	/* parity Odd	UART_CR0_EVEN_ODD |UART_CR0_PE_ENABLE */
#define DEV_SER_MODE_PEVEN	(0x0000000C)	/* parity Even	UART_CR0_EVEN_EVEN|UART_CR0_PE_ENABLE */
#define DEV_SER_MODE_PNON	(0x00000000)	/* parity None			   UART_CR0_PE_NONE */

#define	DEV_SER_MODE_CTSEN	(0x00000400)	/* CTS HW flow control enable	UART_CR0_CTSE_ENABLE */
#define	DEV_SER_MODE_RTSEN	(0x00000200)	/* RTS HW flow control enable	UART_CR0_RTSE_ENABLE */

/* Communication Error */
#define DEV_SER_ERR_OE		(0x00000008)	/* Overrun Error UART_ERR_OVRERR_ERR */ 
#define DEV_SER_ERR_BE		(0x00000001)	/* Break Error	 UART_ERR_BERR_ERR */ 
#define DEV_SER_ERR_PE		(0x00000004)	/* Parity Error	 UART_ERR_PERR_ERR */ 
#define DEV_SER_ERR_FE		(0x00000002)	/* Framing Error UART_ERR_FERR_ERR */ 

#endif /* __DEV_SER_MODE_M4GR_H__ */
