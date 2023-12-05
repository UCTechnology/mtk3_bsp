/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.02
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21 .
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef ADBUN_M4GR

/*
 *	hw_setting.c (ADBUN-TMPM4GRF20FG)
 *	startup / shoutdown processing for hardware
 */

#include <kernel.h>
#include <tm/tmonitor.h>

#include "sysdepend.h"

LOCAL void external_losc_enable(void);
LOCAL void port_initialize(void);

/*
 * Semi-Independent Watchdog Timer (SIWDT) Configuration
 */
#define SIWDEN_Val	(SIWDxEN_WDTE_DISABLE)
#define SIWDCR_Val	(SIWDxEN_WDCR_DISABLE)

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
    /* Semi-Independent Watchdog Timer(SIWD) Disable */
	*SIWDxEN(0) = SIWDEN_Val;			// TSB_SIWD0->EN = SIWDEN_Val;
	*SIWDxCR(0) = SIWDCR_Val;			// TSB_SIWD0->CR = SIWDCR_Val;

	startup_clock();

	/*------ UART, PORT Enable ------*/
	*CGPROTECT = 0x000000C1;		// UNLOCK, CG register write protection
	*CGFSYSMENA |= CGFSYSMENA_UART0;
	*CGFSYSMENB = 0x003FFFFC;   /* PORT Enable */
	*CGPROTECT = 0x00000000;		// LOCK, CG register write protection

	/*------ Port ------*/
	port_initialize();

	/* Setup Pin Function (for UART0) */
	set_port_func( 'E', 3, PE3_UT_TX, PORT_OUT );
	set_port_func( 'E', 2, PE2_UT_RX, PORT_IN );

	/*------ ELOSC Enable ------*/
	external_losc_enable();
}


#define ELOSC_WUP_TIME		((UD)(5000))	/* Warm up time(us) */
#define ELOSC_CLOCK			((UD)(32768))	/* Clock(Hz) : "X2" */
#if defined (__ICCARM__)
#pragma optimize=low
#else
#error	It is necessary to disable optimization.
#endif
LOCAL	void	external_losc_enable(void)
{
	_UD	wupTime;			/* Warm up time */
	UW	work;

	wupTime = (UD)(((ELOSC_WUP_TIME * ELOSC_CLOCK)/(UD)1000000) - 16U);
	while( wupTime > 0x0007FFFFul ){
		/* Please check the settings of ELOSC_WUP_TIME and ELOSC_CLOCK. */
	}
	*CGWUPLCR  = ((UW)wupTime & 0xFFFFFFF0ul) << 8;	/* set CGWUPLCR.WUPTL */
    *RLMLOSCCR = (RLMLOSCCR_XTEN_OSC | RLMLOSCCR_DRCOSCL_ELOSC);		/* set LOSC Enable */
    work = (*CGWUPLCR & CGWUPLCR_WUPTL_MASK);
    *CGWUPLCR = (UW)(work | CGWUPLCR_WULON_START);		/* start Warm-up */
	/* wait for warm-up */
	while( (*CGWUPLCR & CGWUPLCR_WULEF_STAT) != CGWUPLCR_WULEF_END );
}

#define PORT_NO		20	/* PortA-H,J-N,P,R,T-Y */
#define REG_NO		14	/* DATA, CR, FR1-8, OD, PUP, PDN, IE */
LOCAL CONST INT port_num[PORT_NO] = {
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
//	'I', 
	'J', 
	'K', 
	'L', 
	'M', 
	'N', 
//	'O', 
	'P', 
	'R', 
//	'S', 
	'T', 
	'U', 
	'V', 
	'W', 
//	'X', 
	'Y', 
};

_UW* CONST reg[PORT_NO][REG_NO] = {	/* List of register addresses. */
					/* Set 0 if there is no register. */
	{	PxDATA('A'), PxCR('A'),
		PxFRn('A',1), PxFRn('A',2), PxFRn('A',3), (_UW*)0, PxFRn('A',5), PxFRn('A',6), PxFRn('A',7), (_UW*)0,
		PxOD('A'), PxPUP('A'), PxPDN('A'), PxIE('A')	},
	{	PxDATA('B'), PxCR('B'),
		PxFRn('B',1), PxFRn('B',2), PxFRn('B',3), PxFRn('B',4), PxFRn('B',5), PxFRn('B',6), (_UW*)0, (_UW*)0,
		PxOD('B'), PxPUP('B'), PxPDN('B'), PxIE('B')	},
	{	PxDATA('C'), PxCR('C'),
		PxFRn('C',1), (_UW*)0, PxFRn('C',3), PxFRn('C',4), PxFRn('C',5), (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('C'), PxPUP('C'), PxPDN('C'), PxIE('C')	},
	{	PxDATA('D'), PxCR('D'),
		PxFRn('D',1), PxFRn('D',2), PxFRn('D',3), PxFRn('D',4), PxFRn('D',5), PxFRn('D',6), PxFRn('D',7), PxFRn('D',8),
		PxOD('D'), PxPUP('D'), PxPDN('D'), PxIE('D')	},
	{	PxDATA('E'), PxCR('E'),
		PxFRn('E',1), PxFRn('E',2), PxFRn('E',3), PxFRn('E',4), PxFRn('E',5), PxFRn('E',6), PxFRn('E',7), PxFRn('E',8),
		PxOD('E'), PxPUP('E'), PxPDN('E'), PxIE('E')	},
	{	PxDATA('F'), PxCR('F'),
		PxFRn('F',1), (_UW*)0, (_UW*)0, PxFRn('F',4), PxFRn('F',5), PxFRn('F',6), PxFRn('F',7), (_UW*)0,
		PxOD('F'), PxPUP('F'), PxPDN('F'), PxIE('F')	},
	{	PxDATA('G'), PxCR('G'),
		PxFRn('G',1), PxFRn('G',2), PxFRn('G',3), PxFRn('G',4), PxFRn('G',5), PxFRn('G',6), PxFRn('G',7), (_UW*)0,
		PxOD('G'), PxPUP('G'), PxPDN('G'), PxIE('G')	},
	{	PxDATA('H'), PxCR('H'),
		PxFRn('H',1), (_UW*)0, PxFRn('H',3), PxFRn('H',4), PxFRn('H',5), (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('H'), PxPUP('H'), PxPDN('H'), PxIE('H')	},
	{	PxDATA('J'), PxCR('J'),
		(_UW*)0, PxFRn('J',2), PxFRn('J',3), (_UW*)0, PxFRn('J',5), PxFRn('J',6), PxFRn('J',7), (_UW*)0,
		PxOD('J'), PxPUP('J'), PxPDN('J'), PxIE('J')	},
	{	PxDATA('K'), PxCR('K'),
		PxFRn('K',1), PxFRn('K',2), PxFRn('K',3), PxFRn('K',4), PxFRn('K',5), PxFRn('K',6), PxFRn('K',7), (_UW*)0,
		PxOD('K'), PxPUP('K'), PxPDN('K'), PxIE('K')	},
	{	PxDATA('L'), PxCR('L'),
		PxFRn('L',1), PxFRn('L',2), PxFRn('L',3), (_UW*)0, PxFRn('L',5), PxFRn('L',6), PxFRn('L',7), (_UW*)0,
		PxOD('L'), PxPUP('L'), PxPDN('L'), PxIE('L')	},
	{	PxDATA('M'), PxCR('M'),
		PxFRn('M',1), PxFRn('M',2), PxFRn('M',3), PxFRn('M',4), PxFRn('M',5), PxFRn('M',6), PxFRn('M',7), (_UW*)0,
		PxOD('M'), PxPUP('M'), PxPDN('M'), PxIE('M')	},
	{	PxDATA('N'), PxCR('N'),
		(_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('N'), PxPUP('N'), PxPDN('N'), PxIE('N')	},
	{	PxDATA('P'), PxCR('P'),
		(_UW*)0, PxFRn('P',2), PxFRn('P',3), (_UW*)0, PxFRn('P',5), (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('P'), PxPUP('P'), PxPDN('P'), PxIE('P')	},
	{	PxDATA('R'), PxCR('R'),
		(_UW*)0, PxFRn('R',2), PxFRn('R',3), (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('R'), PxPUP('R'), PxPDN('R'), PxIE('R')	},
	{	PxDATA('T'), PxCR('T'),
		PxFRn('T',1), PxFRn('T',2), PxFRn('T',3), (_UW*)0, (_UW*)0, PxFRn('T',6), PxFRn('T',7), (_UW*)0,
		PxOD('T'), PxPUP('T'), PxPDN('T'), PxIE('T')	},
	{	PxDATA('U'), PxCR('U'),
		(_UW*)0, PxFRn('U',2), PxFRn('U',3), (_UW*)0, (_UW*)0, PxFRn('U',6), PxFRn('U',7), (_UW*)0,
		PxOD('U'), PxPUP('U'), PxPDN('U'), PxIE('U')	},
	{	PxDATA('V'), PxCR('V'),
		PxFRn('V',1), PxFRn('V',2), PxFRn('V',3), PxFRn('V',4), PxFRn('V',5), PxFRn('V',6), PxFRn('U',7), (_UW*)0,
		PxOD('V'), PxPUP('V'), PxPDN('V'), PxIE('V')	},
	{	PxDATA('W'), PxCR('W'),
		(_UW*)0, (_UW*)0, PxFRn('W',3), PxFRn('W',4), PxFRn('W',5), PxFRn('W',6), PxFRn('W',7), (_UW*)0,
		PxOD('W'), PxPUP('W'), PxPDN('W'), PxIE('W')	},
	{	PxDATA('Y'), PxCR('Y'),
		PxFRn('Y',1), (_UW*)0, (_UW*)0, PxFRn('Y',4), (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('Y'), PxPUP('Y'), PxPDN('Y'), PxIE('Y')	},
};
CONST UW val[PORT_NO][REG_NO] = {	/* List of values to be set in registers. */
		/* DATA     CR    | FR1     FR2     FR3     FR4     FR5     FR6     FR7     FR8   |   OD     PUP     PDN   |  IE */
 /*A*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*B*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*C*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*D*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*E*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*F*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*G*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*H*/	{ 0x00ul, 0x50ul,  0xF8ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x98ul, 0x20ul, 0xB8ul },
 /*J*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*K*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*L*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*M*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*N*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*P*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*R*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*T*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*U*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*V*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*W*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*Y*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
};

#define PxBLOCK_OFF(x)	(((x)<'I')? 0: \
						(((x)<'O')? 8: \
						(((x)<'Q')? 13: \
						(((x)<'S')? 14: \
						(((x)<'X')? 15:19)))))
#define Px2IDX(x)	(((x) - PxBLOCK_C(x)) + PxBLOCK_OFF(x))
EXPORT void init_port(INT portName)
{
	INT	idx;			/* 'A', 'B', 'C', ... --> 0, 1, 2, ... */
	INT	i;			/* DATA, CR, FR1-8, OD, PUP, PDN, IE */

	idx = Px2IDX(portName);
	for( i = 0; i < REG_NO; i++ ){
		if( reg[idx][i] != 0 ){	/* Set only existing registers. */
			*reg[idx][i] = (_UW)val[idx][i];
		}
	}
}

EXPORT void set_port_func(INT port, INT no, UW func, UW inout )
{
	UW	pat;
	INT	i;

	pat = 0x00000001ul << no;
	/*------ select FUNCTION ------*/
	if( 0 < func ){				// func = NA(0), 1, 2, 3, 4, 5, 6, 7, 8
		for( i = 1; i <= 8; i++ ){
			*PxFRn(port,i) &= ~pat;
		}
		*PxFRn(port,func) |= pat;
	}

	/*------ select INPUT/OUTPUT ------*/
	*PxCR(port) &= ~pat;			// Disable Output
	*PxIE(port) &= ~pat;			// Disable Input
	if( inout & PORT_IN ){
		*PxIE(port) |= pat;		// Enable Input
	}
	if( inout & PORT_OUT ){
		*PxCR(port) |= pat;		// Enable Output
	}
}

LOCAL void port_initialize(void)
{
	INT num;

	for(num = 0; num < PORT_NO; num++)
	{
		init_port(port_num[num]);
	}
}

#if USE_SHUTDOWN
/*
 * Shutdown device
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start device
 *	mode = -1		reset and re-start	(Reset -> Boot -> Start)
 *	mode = -2		fast re-start		(Start)
 *	mode = -3		Normal re-start		(Boot -> Start)
 */
EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}


#endif /* ADBUN_M4GR */
