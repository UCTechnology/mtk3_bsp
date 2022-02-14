﻿/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
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
 *	adc_cnf.h 
 *	A/D converter device driver configuration file
 */
#ifndef	__DEV_ADC_CNF_H__
#define	__DEV_ADC_CNF_H__

#define	DEVCNF_ADC_DEVNAME	"adc"		/* Device name ("adc") */


/*---------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#define DEVDEF_ADC_CNF_PATH_(a)	#a
#define DEVDEF_ADC_CNF_PATH(a)	DEVDEF_ADC_CNF_PATH_(a)
#define DEVDEF_ADC_CNF_SYSDEP()	DEVDEF_ADC_CNF_PATH(sysdepend/TARGET_CPU_DIR/adc_cnf_sysdep.h)
#include DEVDEF_ADC_CNF_SYSDEP()

#endif		/* __DEV_ADC_CNF_H__ */
