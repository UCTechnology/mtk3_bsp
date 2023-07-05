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
#ifdef ADBUN_TMPM3HQF10B

/*
 *	hw_setting.c (ADBUN-TMPM3HQF10B)
 *	startup / shoutdown processing for hardware
 */

#include "kernel.h"
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

	/*------ Fsys Enable All ------*/
	*CGFSYSENA = 0xFFFFFFFF;		/* CG-M3H(2)-D p.38- */
	*CGFSYSENB = 0xF0FFFFFF;		/* CG-M3H(2)-D p.40- */

	/*------ DNF Clock Enable ------*/
	*CGFCEN = CGFCEN_FCIPEN07_SUPPLY;	// TSB_CG_FCEN_FCIPEN07 = 1;

	/*------ ADC Clock & EMT Trace Clock Enable ------*/
	*CGSPCLKEN = CGSPCLKEN_ADCKEN_SUPPLY|CGSPCLKEN_TRCKEN_SUPPLY;
						// TSB_CG_SPCLKEN_ADCKEN = 1;
						// TSB_CG_SPCLKEN_TRCKEN = 1;
	/*------ Port ------*/
	port_initialize();

	/*------ ELOSC Enable ------*/
	external_losc_enable();
}


#define PORT_NO		18	/* PortA-H,J-N,P,R,T-V */
#define REG_NO		12	/* DATA, CR, FR1-6, OD, PUP, PDN, IE */
_UW* CONST reg[PORT_NO][REG_NO] = {	/* List of register addresses. */
					/* Set 0 if there is no register. */
	{	PxDATA('A'), PxCR('A'),
		PxFRn('A',1), PxFRn('A',2), PxFRn('A',3), PxFRn('A',4), PxFRn('A',5), PxFRn('A',6),
		PxOD('A'), PxPUP('A'), PxPDN('A'), PxIE('A')	},
	{	PxDATA('B'), PxCR('B'),
		PxFRn('B',1), PxFRn('B',2), PxFRn('B',3), PxFRn('B',4), PxFRn('B',5), PxFRn('B',6),
		PxOD('B'), PxPUP('B'), PxPDN('B'), PxIE('B')	},
	{	PxDATA('C'), PxCR('C'),
		PxFRn('C',1), PxFRn('C',2), PxFRn('C',3), PxFRn('C',4), PxFRn('C',5), (_UW*)0,
		PxOD('C'), PxPUP('C'), PxPDN('C'), PxIE('C')	},
	{	PxDATA('D'), PxCR('D'),
		(_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('D'), PxPUP('D'), PxPDN('D'), PxIE('D')	},
	{	PxDATA('E'), PxCR('E'),
		(_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('E'), PxPUP('E'), PxPDN('E'), PxIE('E')	},
	{	PxDATA('F'), PxCR('F'),
		(_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('F'), PxPUP('F'), PxPDN('F'), PxIE('F')	},
	{	PxDATA('G'), PxCR('G'),
		PxFRn('G',1), PxFRn('G',2), PxFRn('G',3), PxFRn('G',4), (_UW*)0, (_UW*)0,
		PxOD('G'), PxPUP('G'), PxPDN('G'), PxIE('G')	},
	{	PxDATA('H'), PxCR('H'),
		PxFRn('H',1), (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('H'), PxPUP('H'), PxPDN('H'), PxIE('H')	},
	{	PxDATA('J'), PxCR('J'),
		PxFRn('J',1), PxFRn('J',2), PxFRn('J',3), PxFRn('J',4), PxFRn('J',5), (_UW*)0,
		PxOD('J'), PxPUP('J'), PxPDN('J'), PxIE('J')	},
	{	PxDATA('K'), PxCR('K'),
		PxFRn('K',1), PxFRn('K',2), PxFRn('K',3), PxFRn('K',4), PxFRn('K',5), (_UW*)0,
		PxOD('K'), PxPUP('K'), PxPDN('K'), PxIE('K')	},
	{	PxDATA('L'), PxCR('L'),
		PxFRn('L',1), PxFRn('L',2), PxFRn('L',3), PxFRn('L',4), PxFRn('L',5), (_UW*)0,
		PxOD('L'), PxPUP('L'), PxPDN('L'), PxIE('L')	},
	{	PxDATA('M'), PxCR('M'),
		PxFRn('M',1), PxFRn('M',2), PxFRn('M',3), PxFRn('M',4), PxFRn('M',5), PxFRn('M',6),
		PxOD('M'), PxPUP('M'), PxPDN('M'), PxIE('M')	},
	{	PxDATA('N'), PxCR('N'),
		PxFRn('N',1), PxFRn('N',2), PxFRn('N',3), PxFRn('N',4), PxFRn('N',5), (_UW*)0,
		PxOD('N'), PxPUP('N'), PxPDN('N'), PxIE('N')	},
	{	PxDATA('P'), PxCR('P'),
		PxFRn('P',1), PxFRn('P',2), PxFRn('P',3), PxFRn('P',4), (_UW*)0, (_UW*)0,
		PxOD('P'), PxPUP('P'), PxPDN('P'), PxIE('P')	},
	{	PxDATA('R'), PxCR('R'),
		(_UW*)0, (_UW*)0, PxFRn('R',3), PxFRn('R',4), (_UW*)0, (_UW*)0,
		PxOD('R'), PxPUP('R'), PxPDN('R'), PxIE('R')	},
	{	PxDATA('T'), PxCR('T'),
		PxFRn('T',1), PxFRn('T',2), PxFRn('T',3), PxFRn('T',4), (_UW*)0, (_UW*)0,
		PxOD('T'), PxPUP('T'), PxPDN('T'), PxIE('T')	},
	{	PxDATA('U'), PxCR('U'),
		(_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('U'), PxPUP('U'), PxPDN('U'), PxIE('U')	},
	{	PxDATA('V'), PxCR('V'),
		PxFRn('V',1), PxFRn('V',2), (_UW*)0, (_UW*)0, (_UW*)0, (_UW*)0,
		PxOD('V'), PxPUP('V'), PxPDN('V'), PxIE('V')	}	};
CONST UW setup_regs[PORT_NO][REG_NO] = {	/* List of values to be set in registers. */
	/* DATA     CR    | FR1     FR2     FR3     FR4     FR5     FR6   |   OD     PUP     PDN   |  IE */
 /*A*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*B*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*C*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*D*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*E*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*F*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*G*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*H*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*J*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*K*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*L*/	{ 0x00ul, 0x14ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x1Ful, 0x00ul,  0x00ul, 0x13ul, 0x08ul, 0x1Bul },
 /*M*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*N*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*P*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*R*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*T*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*U*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul },
 /*V*/	{ 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul, 0x00ul,  0x00ul, 0x00ul, 0x00ul, 0x00ul } };

#define PxBLOCK_OFF(x)	(((x)<'I')? 0:(((x)<'O')? 8:(((x)<'Q')? 13:(((x)<'S')? 14:15))))
#define Px2IDX(x)	(((x) - PxBLOCK_C(x)) + PxBLOCK_OFF(x))
EXPORT void init_port(INT portName)
{
	INT	idx;			/* 'A', 'B', 'C', ... --> 0, 1, 2, ... */
	INT	i;			/* DATA, CR, FR1-6, OD, PUP, PDN, IE */

	idx = Px2IDX(portName);
	for( i = 0; i < REG_NO; i++ ){
		if( reg[idx][i] != 0 ){	/* Set only existing registers. */
			*reg[idx][i] = (_UW)setup_regs[idx][i];
		}
	}
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
	wupTime = (UD)((ELOSC_WUP_TIME * ELOSC_CLOCK)/(UD)1000000);
	while( wupTime > 0x0007FFFFul ){
		/* Please check the settings of ELOSC_WUP_TIME and ELOSC_CLOCK. */
	}
	*CGWUPLCR  = ((UW)wupTime & 0xFFFFFFF0ul) << 8;	/* set CGWUPLCR.WUPTL */

	*RLMLOSCCR = RLMLOSCCR_XTEN_OSC;		/* set LOSC Enable */
	*CGWUPLCR |= CGWUPLCR_WULON_START;		/* start Warm-up */
	/* wait for warm-up */
	while( (*CGWUPLCR & CGWUPLCR_WULEF_STAT) != CGWUPLCR_WULEF_END );
}

EXPORT void set_port_func(INT port, INT no, UW func, UW inout )
{
	UW	pat;
	INT	i;

	pat = 0x00000001ul << no;
	/*------ select FUNCTION ------*/
	if( 0 < func ){				// func = NA(0), 1, 2, 3, 4, 5, 6
		for( i = 1; i <= 6; i++ ){
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
	/*===============================================================================*/
	/*  [Port A]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PADATA | PACR | PAFRn | PAOD | PAPUP | PADN | PAIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PA0  | -         | O   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PA1  | UT0TXDA   | O   | 0      | 1    | PAFR1 | 0    | 0     | 0    | 0     */
	/*  PA2  | UT0RXD    | I   | 0      | 0    | PAFR1 | 0    | 0     | 0    | 1     */
	/*  PA3  | -         | I   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PA4  | I2C1SCL   | I/O | 1      | 0    | PAFR1 | 0    | 0     | 0    | 1     */
	/*  PA5  | I2C1SDA   | I/O | 1      | 0    | PAFR1 | 0    | 0     | 0    | 1     */
	/*  PA6  | -         | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PA7  | -         | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('A');
//	set_port_func( 'A', 0, PA0_T32A00OUTA,	PORT_OUT );
//	set_port_func( 'A', 1, PA1_UT0TXDA,	PORT_OUT );
//	set_port_func( 'A', 2, PA2_UT0RXD,	PORT_IN );
	/* PA3 : Use reset value. */
//	set_port_func( 'A', 4, PA4_I2C1SCL,	PORT_INOUT );
//	set_port_func( 'A', 5, PA5_I2C1SDA,	PORT_INOUT );
	/* PA6 : Use reset value. */
	/* PA7 : Use reset value. */

	/*===============================================================================*/
	/*  [Port B]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PBDATA | PBCR | PBFRn | PBOD | PBPUP | PBDN | PBIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PB0  | BOOT_N    | -   | 0      | 0    | 0     | 0    | 0     | 0    | N/A   */
	/*  PB1  | RXIN0     | I   | 0      | 0    | PBFR1 | 0    | 0     | 0    | 1     */
	/*  PB2  | TSPI1SCK  | O   | 0      | 1    | PBFR3 | 0    | 0     | 0    | 0     */
	/*  PB3  | TSPI1TXD  | O   | 0      | 1    | PBFR3 | 0    | 0     | 0    | 0     */
	/*  PB4  | TSPI1RXD  | I   | 0      | 0    | PBFR3 | 0    | 0     | 0    | 1     */
	/*  PB5  | TSPI1CS0  | O   | 0      | 1    | 0     | 0    | 0     | 0    | 0     */
	/*  PB6  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PB7  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('B');
	/* PB0 : Use reset value. */
//	set_port_func( 'B', 1, PB1_RXIN0,	PORT_IN );
//	set_port_func( 'B', 2, PB2_TSPI1SCK,	PORT_IN );
//	set_port_func( 'B', 3, PB3_TSPI1TXD,	PORT_OUT );
//	set_port_func( 'B', 4, PB4_TSPI1RXD,	PORT_IN );
//	set_port_func( 'B', 5, PB5_TSPI1CSIN,	PORT_IN );
	/* PB6 : Use reset value. */
	/* PB7 : Use reset value. */
	
	/*===============================================================================*/
	/*  [Port C]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PCDATA | PCCR | PCFRn | PCOD | PCPUP | PCDN | PCIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PC0  | I2C0SCL   | I/O | 0      | 1    | PCFR1 | 1    | 0     | 0    | 1     */
	/*  PC1  | ISC0SDA   | I/O | 0      | 1    | PCFR1 | 1    | 0     | 0    | 1     */
	/*  PC2  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PC3  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PC4  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PC5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PC6  | -         | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('C');
//	set_port_func( 'C', 0, PC0_I2C0SCL,	PORT_INOUT );
//	set_port_func( 'C', 1, PC1_I2C0SDA,	PORT_INOUT );
//	set_port_func( 'C', 2, PC2_INT02,	PORT_IN );
	/* PC3 : Use reset value. */
	/* PC4 : Use reset value. */
	/* PC5 : Use reset value. */
	/* PC6 : Use reset value. */
	/*===============================================================================*/
	/*  [Port D]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PDDATA | PDCR | PDFRn | PDOD | PDPUP | PDDN | PDIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PD0  | AINA00    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PD1  | AINA01    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PD2  | AINA02    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PD3  | AINA03    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PD4  | AINA19    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PD5  | AINA20    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('D');
	/* PD0 : Use reset value. */
	/* PD1 : Use reset value. */
	/* PD2 : Use reset value. */
	/* PD3 : Use reset value. */
//	set_port_func( 'D', 4, PD4_AINA19,	PORT_NOINOUT );
//	set_port_func( 'D', 5, PD5_AINA20,	PORT_NOINOUT );
	/*===============================================================================*/
	/*  [Port E]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PEDATA | PECR | PEFRn | PEOD | PEPUP | PEDN | PEIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PE0  | AINA04    | I   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PE1  | AINA05    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PE2  | AINA06    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PE3  | AINA07    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PE4  | AINA08    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PE5  | AINA09    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PE6  | AINA10    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('E');
//	set_port_func( 'E', 0, PE0_AINA04,	PORT_NOINOUT );
	/* PE1 : Use reset value. */
	/* PE2 : Use reset value. */
	/* PE3 : Use reset value. */
	/* PE4 : Use reset value. */
	/* PE5 : Use reset value. */
	/* PE6 : Use reset value. */
	/*===============================================================================*/
	/*  [Port F]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PFDATA | PFCR | PFFRn | PFOD | PFPUP | PFDN | PFIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PF0  | AINA11    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PF1  | AINA12    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PF2  | AINA13    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PF3  | AINA14    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PF4  | AINA15    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PF5  | AINA16    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PF6  | AINA17    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PF7  | AINA18    | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('F');
	/* PF0 : Use reset value. */
	/* PF1 : Use reset value. */
	/* PF2 : Use reset value. */
	/* PF3 : Use reset value. */
	/* PF4 : Use reset value. */
	/* PF5 : Use reset value. */
	/* PF6 : Use reset value. */
	/* PF7 : Use reset value. */
	/*===============================================================================*/
	/*  [Port G]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PGDATA | PGCR | PGFRn | PGOD | PGPUP | PGDN | PGIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PG0  | DAC0      | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PG1  | DAC1      | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PG2  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PG3  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PG4  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PG5  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PG6  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PG7  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('G');
	/* PG0 : Use reset value. */
	/* PG1 : Use reset value. */
	/* PG2 : Use reset value. */
	/* PG3 : Use reset value. */
	/* PG4 : Use reset value. */
	/* PG5 : Use reset value. */
	/* PG6 : Use reset value. */
	/* PG7 : Use reset value. */
	/*===============================================================================*/
	/*  [Port H]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PHDATA | PHCR | PHFRn | PHOD | PHPUP | PHDN | PHIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PH0  | X1        | I   | 0      | N/A  | N/A   | N/A  | N/A   | 0    | 0     */
	/*  PH1  | X2        | O   | 0      | N/A  | N/A   | N/A  | N/A   | 0    | 0     */
	/*  PH2  | XT1       | I   | 0      | N/A  | N/A   | N/A  | N/A   | 0    | 0     */
	/*  PH3  | XT2       | O   | 0      | N/A  | N/A   | N/A  | N/A   | 0    | 0     */
	/*  PH4  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PH5  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PH6  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PH7  |  -        | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('H');
	set_port_func( 'H', 0, PH0_X1,	PORT_NOINOUT );
	set_port_func( 'H', 1, PH1_X2,	PORT_NOINOUT );
	set_port_func( 'H', 2, PH2_XT1,	PORT_NOINOUT );
	set_port_func( 'H', 3, PH3_XT2,	PORT_NOINOUT );
	/* PH4 : Use reset value. */
	/* PH5 : Use reset value. */
	/* PH6 : Use reset value. */
	/* PH7 : Use reset value. */
	/*===============================================================================*/
	/*  [Port J]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PJDATA | PJCR | PJFRn | PJOD | PJPUP | PJDN | PJIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PJ0  | T32A3OUTA | O   | 0      | 1    | PJFR3 | 0    | 0     | 0    | 0     */
	/*  PJ1  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PJ2  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PJ3  | T32A3OUTB | O   | 0      | 1    | PJFR3 | 0    | 0     | 0    | 0     */
	/*  PJ4  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PJ5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('J');
//	set_port_func( 'J', 0, PJ0_T32A03OUTA,	PORT_OUT );
	/* PJ1 : Use reset value. */
	/* PJ2 : Use reset value. */
//	set_port_func( 'J', 3, PJ3_T32A03OUTB,	PORT_OUT );
	/* PJ4 : Use reset value. */
	/* PJ5 : Use reset value. */
	/*===============================================================================*/
	/*  [Port K]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PKDATA | PKCR | PKFRn | PKOD | PKPUP | PKDN | PKIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PK0  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PK1  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PK2  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PK3  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PK4  |PORT_LED[0]| O   | 0      | 1    | 0     | 0    | 0     | 0    | 0     */
	/*  PK5  |PORT_LED[1]| O   | 0      | 1    | 0     | 0    | 0     | 0    | 0     */
	/*  PK6  |PORT_LED[2]| O   | 0      | 1    | 0     | 0    | 0     | 0    | 0     */
	/*  PK7  |PORT_LED[3]| O   | 0      | 1    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('K');
	/* PK0 : Use reset value. */
	/* PK1 : Use reset value. */
	/* PK2 : Use reset value. */
	/* PK3 : Use reset value. */
//	set_port_func( 'K', 4, PK4_OUTPUT,	PORT_OUT );
//	set_port_func( 'K', 5, PK5_OUTPUT,	PORT_OUT );
//	set_port_func( 'K', 6, PK6_OUTPUT,	PORT_OUT );
//	set_port_func( 'K', 7, PK7_OUTPUT,	PORT_OUT );
	/*===============================================================================*/
	/*  [Port L]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PLDATA | PLCR | PLFRn | PLOD | PLPUP | PLDN | PLIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PL0  | TRST_N    | -   | 0      | 0    | PKFR5 | 0    | 1     | 0    | 1     */
	/*  PL1  | TDI       | -   | 0      | 0    | PKFR5 | 0    | 1     | 0    | 1     */
	/*  PL2  | TDO/SWV   | -   | 0      | 1    | PKFR5 | 0    | 0     | 0    | 0     */
	/*  PL3  | TCK/SWCLK | -   | 0      | 0    | PKFR5 | 0    | 0     | 1    | 1     */
	/*  PL4  | TMS/SWDIO | -   | 0      | 1    | PKFR5 | 0    | 1     | 0    | 1     */
	/*  PL5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PL6  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PL7  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('L');
	/* PL0 : Use reset value. */
	/* PL1 : Use reset value. */
	/* PL2 : Use reset value. */
	/* PL3 : Use reset value. */
	/* PL4 : Use reset value. */
	/* PL5 : Use reset value. */
	/* PL6 : Use reset value. */
	/* PL7 : Use reset value. */
	/*===============================================================================*/
	/*  [Port M]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PMDATA | PMCR | PMFRn | PMOD | PMPUP | PMDN | PMIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PM0  | T32A00OUTA| -   | 0      | 0    | PMFR4 | 0    | 0     | 0    | 0     */
	/*  PM1  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PM2  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PM3  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PM4  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PM5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PM6  | -         | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PM7  | -         | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('M');
//	set_port_func( 'M', 0, PM0_T32A00OUTA,	PORT_OUT );
	/* PM1 : Use reset value. */
	/* PM2 : Use reset value. */
	/* PM3 : Use reset value. */
	/* PM4 : Use reset value. */
	/* PM5 : Use reset value. */
	/* PM6 : Use reset value. */
	/* PM7 : Use reset value. */
	/*===============================================================================*/
	/*  [Port N]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PNDATA | PNCR | PNFRn | PNOD | PNPUP | PNDN | PNIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PN0  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PN1  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PN2  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PN3  | INT10     | I   | 0      | 0    | 0     | 0    | 0     | 0    | 1     */
	/*  PN4  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PN5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('N');
	/* PN0 : Use reset value. */
	/* PN1 : Use reset value. */
	/* PN2 : Use reset value. */
//	set_port_func( 'N', 3, PN3_INT10,	PORT_IN );
	/* PN4 : Use reset value. */
	/* PN5 : Use reset value. */
	/*===============================================================================*/
	/*  [Port P]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PPDATA | PPCR | PPFRn | PPOD | PPPUP | PPDN | PPIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PP0  | TSPI1CLK  | O   | 0      | 1    | PPFR1 | 0    | 0     | 0    | 0     */
	/*  PP1  | TSPI1TXD  | O   | 0      | 1    | PPFR1 | 0    | 0     | 0    | 0     */
	/*  PP2  | TSPI1RXD  | I   | 0      | 0    | PPFR1 | 0    | 0     | 0    | 1     */
	/*  PP3  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PP4  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PP5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PP6  |PORT_DSW[0]| I   | 0      | 0    | 0     | 0    | 0     | 0    | 1     */
	/*  PP7  |PORT_DSW[1]| I   | 0      | 0    | 0     | 0    | 0     | 0    | 1     */
	/*===============================================================================*/
	init_port('P');
//	set_port_func( 'P', 0, PP0_TSPI2SCK,	PORT_OUT );
//	set_port_func( 'P', 1, PP1_TSPI2TXD,	PORT_OUT );
//	set_port_func( 'P', 2, PP2_TSPI2RXD,	PORT_IN );
	/* PP3 : Use reset value. */
	/* PP4 : Use reset value. */
	/* PP5 : Use reset value. */
//	set_port_func( 'P', 6, PP6_INPUT,	PORT_IN );
//	set_port_func( 'P', 7, PP7_INPUT,	PORT_IN );
	/*===============================================================================*/
	/*  [Port R]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PRDATA | PRCR | PRFRn | PROD | PRPUP | PRDN | PRIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PR0  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PR1  |T32A02INA0 | I   | 0      | 0    | PRFR3 | 0    | 0     | 0    | 1     */
	/*  PR2  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PR3  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PR4  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PR5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PR6  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PR7  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('R');
	/* PR0 : Use reset value. */
//	set_port_func( 'R', 1, PR1_T32A02INA0,	PORT_IN );
	/* PR2 : Use reset value. */
	/* PR3 : Use reset value. */
	/* PR4 : Use reset value. */
	/* PR5 : Use reset value. */
	/* PR6 : Use reset value. */
	/* PR7 : Use reset value. */
	/*===============================================================================*/
	/*  [Port T]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PLDATA | PLCR | PLFRn | PLOD | PLPUP | PLDN | PLIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PT0  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PT1  | TSPI2CS0  | O   | 0      | 1    | PTFR1 | 0    | 0     | 0    | 0     */
	/*  PT2  | TSPI2SCK  | I   | 0      | 0    | PTFR1 | 0    | 0     | 0    | 1     */
	/*  PT3  | TSPI2TXD  | O   | 0      | 1    | PTFR1 | 0    | 0     | 0    | 0     */
	/*  PT4  | TSPI2RXD  | I   | 0      | 0    | PTFR1 | 0    | 0     | 0    | 1     */
	/*  PT5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PT6  |  -        | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('T');
	/* PT0 : Use reset value. */
//	set_port_func( 'T', 1, PT1_TSPI2CS0,	PORT_OUT );
//	set_port_func( 'T', 2, PT2_TSPI2SCK,	PORT_OUT );
//	set_port_func( 'T', 3, PT3_TSPI2TXD,	PORT_OUT );
//	set_port_func( 'T', 4, PT4_TSPI2RXD,	PORT_IN );
	/* PT5 : Use reset value. */
	/* PT6 : Use reset value. */
	/*===============================================================================*/
	/*  [Port U]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PUDATA | PUCR | PUFRn | PUOD | PUPUP | PUDN | PUIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PU0  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PU1  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PU2  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PU3  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PU4  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PU5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('U');
	/* PU0 : Use reset value. */
	/* PU1 : Use reset value. */
	/* PU2 : Use reset value. */
	/* PU3 : Use reset value. */
	/* PU4 : Use reset value. */
	/* PU5 : Use reset value. */
	/*===============================================================================*/
	/*  [Port V]                                                                     */
	/*-------------------------------------------------------------------------------*/
	/*  Port | Function  | I/O | PLDATA | PLCR | PLFRn | PLOD | PLPUP | PLDN | PLIE  */
	/*-------------------------------------------------------------------------------*/
	/*  PV0  |PORT_PSW[0]| I   | 0      | 0    | 0     | 0    | 0     | 0    | 1     */
	/*  PV1  |PORT_PSW[1]| I   | 0      | 0    | 0     | 0    | 0     | 0    | 1     */
	/*  PV2  |PORT_PSW[2]| I   | 0      | 0    | 0     | 0    | 0     | 0    | 1     */
	/*  PV3  |PORT_PSW[3]| I   | 0      | 0    | 0     | 0    | 0     | 0    | 1     */
	/*  PV4  | -         | -   | 0      | 0    | N/A   | 0    | 0     | 0    | 0     */
	/*  PV5  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*  PV6  | -         | -   | 0      | 0    | 0     | 0    | 0     | 0    | 0     */
	/*===============================================================================*/
	init_port('V');
//	set_port_func( 'V', 0, PV0_INPUT,	PORT_IN );
//	set_port_func( 'V', 1, PV1_INPUT,	PORT_IN );
//	set_port_func( 'V', 2, PV2_INPUT,	PORT_IN );
//	set_port_func( 'V', 3, PV3_INPUT,	PORT_IN );
	/* PV4 : Use reset value. */
	/* PV5 : Use reset value. */
	/* PV6 : Use reset value. */
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


#endif /* ADBUN_TMPM3HQF10B */