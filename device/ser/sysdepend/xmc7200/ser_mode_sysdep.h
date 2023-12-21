/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
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
 *	ser_mode_sysdep.h
 *	Serial communication device driver
 *	Communication mode definition for XMC7200
 */

#ifndef	__DEV_SER_MODE_XMC7200_H__
#define	__DEV_SER_MODE_XMC7200_H__

#define DEV_SER_MODE_PNON	(0x00000000)	/* parity None		*/
#define DEV_SER_MODE_PODD	(0x40000000)	/* parity Odd		*/
#define DEV_SER_MODE_PEVEN	(0x80000000)	/* parity Even		*/
#define DEV_SER_MODE_PBMASK	(0xC0000000)

#define	DEV_SER_MODE_5BIT	(0x00000000)	/* Data Length 5 bits	*/
#define	DEV_SER_MODE_6BIT	(0x10000000)	/* Data Length 6 bits	*/
#define	DEV_SER_MODE_7BIT	(0x20000000)	/* Data Length 7 bits	*/
#define	DEV_SER_MODE_8BIT	(0x30000000)	/* Data Length 8 bits	*/
#define DEV_SER_MODE_DLMASK	(0x30000000)

#define	DEV_SER_MODE_1STOP	(0x00000000)	/* Stop-bit 1 bit	*/
#define	DEV_SER_MODE_1_5STOP	(0x04000000)	/* Stop-bit 1.5 bits	*/
#define	DEV_SER_MODE_2STOP	(0x08000000)	/* Stop-bit 2 bits	*/
#define	DEV_SER_MODE_SBMASK	(0x0C000000)

#define	DEV_SER_MODE_CTSEN	(0x01000000)	/* CTS Hardware flow control enable */
#define	DEV_SER_MODE_RTSEN	(0x02000000)	/* RTS Hardware flow control enable */
#define	DEV_SER_MODE_FLMASK	(0x03000000)

/* Communication Error : SCBx_INTR_RX*/
#define	DEV_SER_ERR_PE		(0x00000200)	/* Parity Error		*/
#define	DEV_SER_ERR_FE		(0x00000100)	/* Framing Error	*/
#define	DEV_SER_ERR_OE		(0x00000020)	/* Overrun Error	*/
#define DEV_SER_ERR_MASK	(0x00000320)

#endif /* __DEV_SER_MODE_XMC7200_H__ */
