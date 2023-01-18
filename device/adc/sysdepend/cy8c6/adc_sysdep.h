/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.0
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
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
 *	adc_sysdep.h
 *	A/D converter device driver
 *	System-dependent definition for CY8C6
 */

#ifndef	__DEV_ADC_CY8C6_H__
#define	__DEV_ADC_CY8C6_H__

#define	DEV_ADC_UNITNM	(0)	/* Number of devive units */
#define DEV_ADC_UNIT0	(0)

/* Number of ADC chanels */
#define	ADC_CH_NUM	(8)	/* P10[0-7] */

/*
 * A/D converter interrupt number
*/
#define	INTNO_INTADC	(155)	/* SAR ADC interrupt */

#endif		/* __DEV_ADC_CY8C6_H__ */
