/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.0
 *
 *    Copyright (C) 2022-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modified by UC Technology at 2022/12/07
 *    Modified by UC Technology at 2023/01/17
 * 
 *    Copyright (c) 2022-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */


/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for CY8C6
 */

#ifndef	__DEV_SER_CY8C6_H__
#define	__DEV_SER_CY8C6_H__

#define DEV_SER_UNITNM		(13)	/* Number of devive channels */
#define DEV_SER_UNIT0		(0)	/* SCB0			*/
#define DEV_SER_UNIT1		(1)	/* SCB1 : ARD_A[0:3]	*/
#define DEV_SER_UNIT2		(2)	/* SCB2 : ARD_A[8:11]	*/
#define DEV_SER_UNIT3		(3)	/* SCB3	: ARD_SCL/_SDA	*/
#define DEV_SER_UNIT4		(4)	/* SCB4			*/
#define DEV_SER_UNIT5		(5)	/* SCB5 : USB/T-Monitor	*/
#define DEV_SER_UNIT6		(6)	/* SCB6			*/
#define DEV_SER_UNIT7		(7)	/* SCB7			*/
#define DEV_SER_UNIT8		(8)	/* SCB8 : NOTE IRQ18	*/
#define DEV_SER_UNIT9		(9)	/* SCB9			*/
#define DEV_SER_UNIT10		(10)	/* SCB10		*/
#define DEV_SER_UNIT11		(11)	/* SCB11		*/
#define DEV_SER_UNIT12		(12)	/* SCB12		*/

/*
 * USART registers
 */
/* USE : CySCB_Type --> CySCB_V1_Type : mtb_shared/mtb-pdl-cat1/release-v2.4.0/devices/COMPONENT_CAT1A/include/ip/cyip_scb.h */

/* UART interrupt number */
#define INTNO_UART0		(39)	/* SCB0			*/
#define INTNO_UART1		(40)	/* SCB1 : ARD_A[0:3]	*/
#define INTNO_UART2		(41)	/* SCB2 : ARD_A[8:11]	*/
#define INTNO_UART3		(42)	/* SCB3	: USB-I2C, P6[0,1] */
#define INTNO_UART4		(43)	/* SCB4			*/
#define INTNO_UART5		(44)	/* SCB5 : USB/T-Monitor	*/
#define INTNO_UART6		(45)	/* SCB6			*/
#define INTNO_UART7		(46)	/* SCB7			*/
#define INTNO_UART8		(18)	/* SCB8 : DISCONTINUITY	*/
#define INTNO_UART9		(47)	/* SCB9			*/
#define INTNO_UART10		(48)	/* SCB10		*/
#define INTNO_UART11		(59)	/* SCB11		*/
#define INTNO_UART12		(50)	/* SCB12		*/

#endif		/* __DEV_SER_CY8C6_H__ */
