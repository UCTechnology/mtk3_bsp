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
 *    Modifications: Adapted to the ADBUN-M4NR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for TMPM4KN
 */

#ifndef	__DEV_SER_M4NR_H__
#define	__DEV_SER_M4NR_H__

/*----------------------------------------------------------------------*/
/*	Device Configurations						*/
/*----------------------------------------------------------------------*/
#define	DEV_SER_UNITNM		(6)		/* Number of devive channels */
#define DEV_SER_UNIT0		(0)		/* UART0 : T-Monitor (USB2) */
#define DEV_SER_UNIT1		(1)		/* UART1 */
#define DEV_SER_UNIT2		(2)		/* UART2 */
#define DEV_SER_UNIT3		(3)		/* UART3 */
#define DEV_SER_UNIT4		(4)		/* UART4 */
#define DEV_SER_UNIT5		(5)		/* UART5 */

#define TX_FIFO_SIZE		8
#define RX_FIFO_SIZE		8

#define RX_BUFFER_SIZE		64

/*----------------------------------------------------------------------*/
/*	Macros for controlling Process					*/
/*----------------------------------------------------------------------*/
#define FLG_TX_CMP		0x00000001
#define FLG_RX_RDY		0x00000002
#define FLG_TX_ABT		0x00000004
#define FLG_RX_ABT		0x00000008

#define FLG_TX_COMPLETE(p)	(FLG_TX_CMP<<((p)<<2))	/* p = port No.	*/
#define FLG_RX_READY(p)		(FLG_RX_RDY<<((p)<<2))	/* p = 0,1,...7	*/
#define FLG_TX_ABORT(p)		(FLG_TX_ABT<<((p)<<2))	/* (p)<<2 = p*4	*/
#define FLG_RX_ABORT(p)		(FLG_RX_ABT<<((p)<<2))

#define RS_MAXREQQ		(16)	/* maximum queued requests */

#define SET_FLAG		(1)
#define RESET_FLAG		(0)

/*----------------------------------------------------------------------*/
/*	FUNCTIONS (Internal use only)					*/
/*----------------------------------------------------------------------*/
IMPORT	void	initSioParam( W port );			/* ser_base.c	*/
IMPORT	ER	initUartBase( W port );
IMPORT	ER	systemInit( W port );

IMPORT	ER	initRxBuf(W port);			/* ser_rxbuf.c	*/
IMPORT	ER	deleteRxBuf(W port);
IMPORT	W	resizeRxBuf(W port, W size);
IMPORT	W	getRxBufFreeSize(W port);
IMPORT	W	getDataSizeInRxBuf(W port);
IMPORT	W	setRxBufFromDR(W port, _UW *data, W sz);
IMPORT	W	getDataFromRxBuf(W port, UB *data, W maxSize);

IMPORT	ER	uct_serial_init( W port );		/* sio_cntl.c	*/
IMPORT	ER	uct_serial_ctl( W port, W kind, UW *arg );
IMPORT	ER	uct_serial_rel( W port );

IMPORT	ER	defIntHandler( W port );		/* sio_inout.c	*/
IMPORT	ER	uct_serial_out( W port, B *buf, W len, W *alen, W tmout );
IMPORT	ER	uct_serial_in( W port, B *buf, W len, W *alen, W tmout );

IMPORT	void	uartInitIRQ( W port );			/* ser_inthdr.c	*/
IMPORT	ER	defIntHandler( W port );
IMPORT	ER	undefIntHandler( W port );
EXPORT	UINT	getIntNoSnd( W port );
EXPORT	UINT	getIntNoRcv( W port );
EXPORT	UINT	getIntNoErr( W port );
EXPORT	UINT	getIntPri( W port );

/*----------------------------------------------------------------------*/
/*	UART registers							*/
/*----------------------------------------------------------------------*/
/* Base address for each channel */
#define UART0_BASE		(0x400CE000ul)	// TYPE2 : UART0_BASE + channel*0x400
#define UART1_BASE		(0x400CE400ul)
#define UART2_BASE		(0x400CE800ul)
#define UART3_BASE		(0x400CEC00ul)
#define UART4_BASE		(0x400CF000ul)
#define UART5_BASE		(0x400CF400ul)

/*------ Device register base address ------*/
IMPORT CONST UW ba[DEV_SER_UNITNM];		/* ser_m4nr.c		*/
#define UART_SWRST(x)		(_UW*)(ba[x]+0x0000)	/* Software reset register */
#define UART_CR0(x)		(_UW*)(ba[x]+0x0004)	/* Control register 0 */
#define UART_CR1(x)		(_UW*)(ba[x]+0x0008)	/* Control register 1 */
#define UART_CLK(x)		(_UW*)(ba[x]+0x000C)	/* Clock control register */
#define UART_BRD(x)		(_UW*)(ba[x]+0x0010)	/* Baud rate register */
#define UART_TRANS(x)		(_UW*)(ba[x]+0x0014)	/* Transfer enable register */
#define UART_DR(x)		(_UW*)(ba[x]+0x0018)	/* Data register */
#define UART_SR(x)		(_UW*)(ba[x]+0x001C)	/* Status register */
#define UART_FIFOCLR(x)		(_UW*)(ba[x]+0x0020)	/* FIFO clear register */
#define UART_ERR(x)		(_UW*)(ba[x]+0x0024)	/* Error register */


/*------ VALUE in UART_SWRST Register ------*/
#define UART_SWRST_SWRSTF_STAT	0x00000080ul	/* Software reset flag */
#define UART_SWRST_SWRSTF_DONE	0x00000000ul	/*	Not Resetting */
#define UART_SWRST_SWRSTF_RUN	0x00000080ul	/*	Resetting Now */
#define UART_SWRST_SWRST_MASK	0x00000003ul	/* Software reset */
#define UART_SWRST_SWRST_1ST	0x00000002ul	/*	1st command */
#define UART_SWRST_SWRST_2ND	0x00000001ul	/*	2nd command */

/*------ VALUE in UART_CR0 Register ------*/
#define UART_CR0_HBSST_MASK	0x00040000ul	/* half clock mode communication start control */
#define UART_CR0_HBSST_UTxTXDA	0x00000000ul	/*	Output starts from the UTxTXDA pin */
#define UART_CR0_HBSST_UTxTXDB	0x00040000ul	/*	Output starts from the UTxTXDB terminal */
#define UART_CR0_HBSMD_MASK	0x00020000ul	/* half clock mode communication output control */
#define UART_CR0_HBSMD_1PIN	0x00000000ul    /*	1 pin mode */
#define UART_CR0_HBSMD_2PIN	0x00020000ul	/*	2 pin mode */
#define UART_CR0_HBSEN_MASK	0x00010000ul	/* half clock mode control */
#define UART_CR0_HBSEN_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR0_HBSEN_ENABLE	0x00010000ul	/*	Enable */
#define UART_CR0_LPB_MASK	0x00008000ul	/* loopback function */
#define UART_CR0_LPB_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR0_LPB_ENABLE	0x00008000ul	/*	Enable */
#define UART_CR0_NF_MASK	0x00007000ul	/* UTxRXD noise elimination time selection */
#define UART_CR0_NF_NONE	0x00000000ul	/*	No Noise Filter */
#define UART_CR0_NF_F2		0x00001000ul	/*	2 cycles or less with Ft0 clock */
#define UART_CR0_NF_F4		0x00002000ul	/*	4 cycles or less with Ft0 clock */
#define UART_CR0_NF_F8		0x00003000ul	/*	8 cycles or less with Ft0 clock */
#define UART_CR0_NF_T2		0x00004000ul	/*	2 cycles or less with transfer clock */
#define UART_CR0_NF_T3		0x00005000ul	/*	3 cycles or less with transfer clock */
#define UART_CR0_NF_T4		0x00006000ul	/*	4 cycles or less with transfer clock */
#define UART_CR0_NF_T5		0x00007000ul	/*	5 cycles or less with transfer clock */
#define UART_CR0_CTSE_MASK	0x00000400ul	/* CTS handshake function */
#define UART_CR0_CTSE_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR0_CTSE_ENABLE	0x00000400ul	/*	Enable */
#define UART_CR0_RTSE_MASK	0x00000200ul	/* RTS handshake function */
#define UART_CR0_RTSE_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR0_RTSE_ENABLE	0x00000200ul	/*	Enable */
#define UART_CR0_WU_MASK	0x00000100ul	/* wakeup function */
#define UART_CR0_WU_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR0_WU_ENABLE	0x00000100ul	/*	Enable */
#define UART_CR0_IV_MASK	0x00000040ul	/* data signal inversion function */
#define UART_CR0_IV_DISABLE	0x00000000ul	/*	Disable (not inverted) */
#define UART_CR0_IV_ENABLE	0x00000040ul	/*	Valid (invert) */
#define UART_CR0_DIR_MASK	0x00000020ul	/* data transfer direction selection */
#define UART_CR0_DIR_LSB	0x00000000ul    /*	LSB first */
#define UART_CR0_DIR_MSB	0x00000020ul	/*	MSB first */
#define UART_CR0_SBLEN_MASK	0x00000010ul	/* Stop bit length selection */
#define UART_CR0_SBLEN_1	0x00000000ul	/*	1 bit */
#define UART_CR0_SBLEN_2	0x00000010ul	/*	2 bits */
#define UART_CR0_EVEN_MASK	0x00000008ul	/* Even parity selection */
#define UART_CR0_EVEN_ODD	0x00000000ul	/* 	Odd parity */
#define UART_CR0_EVEN_EVEN	0x00000008ul	/*	Even parity */
#define UART_CR0_PE_MASK	0x00000004ul	/* Parity addition function */
#define UART_CR0_PE_NONE	0x00000000ul	/*	None */
#define UART_CR0_PE_ENABLE	0x00000004ul	/*	Enable */
#define UART_CR0_SM_MASK	0x00000003ul	/* Data length selection */
#define UART_CR0_SM_7		0x00000000ul	/*	7 bits */
#define UART_CR0_SM_8		0x00000001ul	/*	8 bits */
#define UART_CR0_SM_9		0x00000002ul	/*	9 bits */

/*------ VALUE in UART_CR1 Register ------*/
#define UART_CR1_TIL_MASK	 0x00007000ul	/* Transmission Fill level setting */
#define UART_CR1_TIL_0		 0x00000000ul	/*	Stage 0 */
#define UART_CR1_TIL_1		 0x00001000ul	/*	Stage 1 */
#define UART_CR1_TIL_2		 0x00002000ul	/*	Stage 2 */
#define UART_CR1_TIL_3		 0x00003000ul	/*	Stage 3 */
#define UART_CR1_TIL_4		 0x00004000ul	/*	Stage 4 */
#define UART_CR1_TIL_5		 0x00005000ul	/*	Stage 5 */
#define UART_CR1_TIL_6		 0x00006000ul	/*	Stage 6 */
#define UART_CR1_TIL_7		 0x00007000ul	/*	Stage 7 */
#define UART_CR1_RIL_MASK	 0x00000700ul	/* Reception Fill level setting */
#define UART_CR1_RIL_8		 0x00000000ul	/*	Stage 8 */
#define UART_CR1_RIL_1		 0x00000100ul	/*	Stage 1 */
#define UART_CR1_RIL_2		 0x00000200ul	/*	Stage 2 */
#define UART_CR1_RIL_3		 0x00000300ul	/*	Stage 3 */
#define UART_CR1_RIL_4		 0x00000400ul	/*	Stage 4 */
#define UART_CR1_RIL_5		 0x00000500ul	/*	Stage 5 */
#define UART_CR1_RIL_6		 0x00000600ul	/*	Stage 6 */
#define UART_CR1_RIL_7		 0x00000700ul	/*	Stage 7 */
#define UART_CR1_INTTXFE_MASK	 0x00000080ul	/* Transmit FIFO interrupt control */
#define UART_CR1_INTTXFE_DISABLE 0x00000000ul	/*	Disable */
#define UART_CR1_INTTXFE_ENABLE	 0x00000080ul	/*	Enable */
#define UART_CR1_INTTXWE_MASK	 0x00000040ul	/* Transmission completion interrupt control */
#define UART_CR1_INTTXWE_DISABLE 0x00000000ul	/*	Disable */
#define UART_CR1_INTTXWE_ENABLE	 0x00000040ul	/*	Enable */
#define UART_CR1_INTRXFE_MASK	 0x00000020ul	/* Receive FIFO interrupt control */
#define UART_CR1_INTRXFE_DISABLE 0x00000000ul	/*	Disable */
#define UART_CR1_INTRXFE_ENABLE	 0x00000020ul	/*	Enable */
#define UART_CR1_INTRXWE_MASK	 0x00000010ul	/* Reception complete interrupt control */
#define UART_CR1_INTRXWE_DISABLE 0x00000000ul	/*	Disable */
#define UART_CR1_INTRXWE_ENABLE	 0x00000010ul	/*	Enable */
#define UART_CR1_INTERR_MASK	 0x00000004ul	/* Error interrupt control */
#define UART_CR1_INTERR_DISABLE	 0x00000000ul	/*	Disable */
#define UART_CR1_INTERR_ENABLE	 0x00000004ul	/*	Enable */
#define UART_CR1_DMATE_MASK	 0x00000002ul	/* Transmission DMA control */
#define UART_CR1_DMATE_DISABLE	 0x00000000ul	/*	Disable */
#define UART_CR1_DMATE_ENABLE	 0x00000002ul	/*	Enable */
#define UART_CR1_DMARE_MASK	 0x00000001ul	/* Reception DMA control */
#define UART_CR1_DMARE_DISABLE	 0x00000000ul	/*	Disable */
#define UART_CR1_DMARE_ENABLE	 0x00000001ul	/*	Enable */

/*------ VALUE in UART_CLK Register ------*/
#define UART_CLK_PRSEL_MASK	0x000000F0ul	/* Prescaler division selection */
#define UART_CLK_PRSEL_D1	0x00000000ul	/*	1/1 */
#define UART_CLK_PRSEL_D2	0x00000010ul	/*	1/2 */
#define UART_CLK_PRSEL_D4	0x00000020ul	/*	1/4 */
#define UART_CLK_PRSEL_D8	0x00000030ul	/*	1/8 */
#define UART_CLK_PRSEL_D16	0x00000040ul	/*	1/16 */
#define UART_CLK_PRSEL_D32	0x00000050ul	/*	1/32 */
#define UART_CLK_PRSEL_D64	0x00000060ul	/*	1/64 */
#define UART_CLK_PRSEL_D128	0x00000070ul	/*	1/128 */
#define UART_CLK_PRSEL_D256	0x00000080ul	/*	1/256 */
#define UART_CLK_PRSEL_D512	0x00000090ul	/*	1/512 */

/*------ VALUE in UART_BRD Register ------*/
#define UART_BRD_KEN_MASK	0x00800000ul	/* Control of N+(64-K)/64 division function. */
#define UART_BRD_KEN_DISABLE	0x00000000ul	/*	Disable */
#define UART_BRD_KEN_ENABLE	0x00800000ul	/*	Enable */
#define UART_BRD_BRK_MASK	0x003F0000ul	/* Setting of 'K' in N+(64-K)/64 */
#define UART_BRD_BRN_MASK	0x0000FFFFul	/* Setting of 'N' in N+(64-K)/64 or N division */

/*------ VALUE in UART_TRANS Register ------*/
#define UART_TRANS_BK_MASK	 0x00000008ul	/* Break Signal control */
#define UART_TRANS_BK_STOP	 0x00000000ul	/*	Stop Signal */
#define UART_TRANS_BK_SEND	 0x00000008ul	/*	Send Signal */
#define UART_TRANS_TXTRG_MASK	 0x00000004ul	/* Trigger transmission control */
#define UART_TRANS_TXTRG_DISABLE 0x00000000ul	/*	Disable */
#define UART_TRANS_TXTRG_ENABLE	 0x00000004ul	/*	Enable */
#define UART_TRANS_TXE_MASK	 0x00000002ul	/* Transmission control */
#define UART_TRANS_TXE_DISABLE	 0x00000000ul	/*	Disable */
#define UART_TRANS_TXE_ENABLE	 0x00000002ul	/*	Enable */
#define UART_TRANS_RXE_MASK	 0x00000001ul	/* Reception control */
#define UART_TRANS_RXE_DISABLE	 0x00000000ul	/*	Disable */
#define UART_TRANS_RXE_ENABLE	 0x00000001ul	/*	Enable */

/*------ VALUE in UART_DR Register ------*/
#define UART_DR_PERR_MASK	0x00040000ul	/* Parity error */
#define UART_DR_PERR_NONE	0x00000000ul	/*	NO Error */
#define UART_DR_PERR_ERR	0x00000000ul	/*	With Error */
#define UART_DR_FERR_MASK	0x00020000ul	/* Framing error */
#define UART_DR_FERR_NONE	0x00000000ul	/*	NO Error */
#define UART_DR_FERR_ERR	0x00000000ul	/*	With Error */
#define UART_DR_BERR_MASK	0x00010000ul	/* Break error flag */
#define UART_DR_BERR_NONE	0x00000000ul	/*	NO Error */
#define UART_DR_BERR_ERR	0x00000000ul	/*	With Error */
#define UART_DR_DR_STAT		0x000001FFul	/* Receive data / Transmit data */

/*------ VALUE in UART_SR Register ------*/
#define UART_SR_SUE_STAT	0x80000000ul	/* Settable state flag */
#define UART_SR_SUE_ABLE	0x00000000ul	/*	Able to set */
#define UART_SR_SUE_UNABLE	0x80000000ul	/*	Unable to set */
#define UART_SR_TXRUN_STAT	0x00008000ul	/* Transmission in operation flag */
#define UART_SR_TXRUN_STOP	0x00000000ul	/*	Transmission stopped */
#define UART_SR_TXRUN_RUN	0x00008000ul	/*	Transmitting operation in progress */
#define UART_SR_TXEND_MASK	0x00004000ul	/* Transmission completion flag */
#define UART_SR_TXEND_DONE	0x00004000ul	/*	Send completely */
#define UART_SR_TXEND_CLR	0x00004000ul	/*	Clear TXEND flag */
#define UART_SR_TXFF_MASK	0x00002000ul	/* Transmit FIFO flag */
#define UART_SR_TXFF_FILL	0x00002000ul	/*	Reached set transmission fill level */
#define UART_SR_TXFF_CLR	0x00002000ul	/*	Clear FIFO Flag */
#define UART_SR_TLVL_STAT	0x00000F00ul	/* Transmit FIFO fill level status */
#define UART_SR_RXRUN_STAT	0x00000080ul	/* Receiving operation flag */
#define UART_SR_RXRUN_STOP	0x00000000ul	/*	Stop receiving */
#define UART_SR_RXRUN_RUN	0x00000080ul	/*	Reception operation in progress */
#define UART_SR_RXEND_MASK	0x00000040ul	/* Reception complete flag */
#define UART_SR_RXEND_DONE	0x00000040ul	/*	Receive completely */
#define UART_SR_RXEND_CLR	0x00000040ul	/*	Clear RXEND flag */
#define UART_SR_RXFF_MASK	0x00000020ul	/* Receive FIFO flag */
#define UART_SR_RXFF_FILL	0x00000020ul	/*	Reached set receive fill level */
#define UART_SR_RXFF_CLR	0x00000020ul	/*	Clear FIFO Flag */
#define UART_SR_RLVL_STAT	0x0000000Ful	/* Receive FIFO Fill level status */

/*------ VALUE in UART_FIFOCLR Register ------*/
#define UART_FIFOCLR_TFCLR	0x00000002ul	/* Transmit buffer clear */
#define UART_FIFOCLR_RFCLR	0x00000001ul	/* Receive buffer clear */

/*------ VALUE in UART_ERR Register ------*/
#define UART_ERR_TRGERR_MASK	0x00000010ul	/* Trigger transmission error flag */
#define UART_ERR_TRGERR_ERR	0x00000010ul	/*	Detect error */
#define UART_ERR_TRGERR_CLR	0x00000010ul	/*	Clear Flag */
#define UART_ERR_OVRERR_MASK	0x00000008ul	/* Overrun error flag */
#define UART_ERR_OVRERR_ERR	0x00000008ul	/*	Detect error */
#define UART_ERR_OVRERR_CLR	0x00000008ul	/*	Clear Flag */
#define UART_ERR_PERR_MASK	0x00000004ul	/* Parity error flag */
#define UART_ERR_PERR_ERR	0x00000004ul	/*	Detect error */
#define UART_ERR_PERR_CLR	0x00000004ul	/*	Clear Flag */
#define UART_ERR_FERR_MASK	0x00000002ul	/* Framing error flag */
#define UART_ERR_FERR_ERR	0x00000002ul	/*	Detect error */
#define UART_ERR_FERR_CLR	0x00000002ul	/*	Clear Flag */
#define UART_ERR_BERR_MASK	0x00000001ul	/* Break error flag */
#define UART_ERR_BERR_ERR	0x00000001ul	/*	Detect error */
#define UART_ERR_BERR_CLR	0x00000001ul	/*	Clear Flag */

/*====== DIRECT PARAMETER for UART ======*/
//#define UART_WORD_LEN_5	/*NONE*/
//#define UART_WORD_LEN_6	/*NONE*/
#define UART_WORD_LEN_7		UART_CR0_SM_7
#define UART_WORD_LEN_8		UART_CR0_SM_8
#define UART_WORD_LEN_9		UART_CR0_SM_9
#define UART_WORD_LEN_MASK	UART_CR0_SM_MASK

#define UART_PARITY_NONE	UART_CR0_PE_NONE
#define UART_PARITY_EVEN	(UART_CR0_PE_ENABLE|UART_CR0_EVEN_EVEN)
#define UART_PARITY_ODD		(UART_CR0_PE_ENABLE|UART_CR0_EVEN_ODD)
#define UART_PARITY_MASK	(UART_CR0_PE_MASK|UART_CR0_EVEN_MASK)

//#define UART_STOP_BIT_05	/*NONE*/		/* 0.5 Stop Bit */
#define UART_STOP_BIT_1		UART_CR0_SBLEN_1	/* 1 Stop Bit */
//#define UART_STOP_BIT_15	/*NONE*/		/* 1.5 Stop Bit */
#define UART_STOP_BIT_2		UART_CR0_SBLEN_2	/* 2 Stop Bit */
#define UART_STOP_BIT_MASK	UART_CR0_SBLEN_MASK

#define UART_HW_FLOW_DISABLE	(UART_CR0_CTSE_DISABLE|UART_CR0_RTSE_DISABLE)
#define UART_HW_FLOW_CTS	 UART_CR0_CTSE_ENABLE
#define UART_HW_FLOW_RTS	 UART_CR0_RTSE_ENABLE
#define UART_HW_FLOW_ENABLE	(UART_CR0_CTSE_ENABLE|UART_CR0_RTSE_ENABLE)
#define UART_HW_FLOW_MASK	(UART_CR0_CTSE_MASK|UART_CR0_RTSE_MASK)

/*
 * UART Interrupts
 */
#define UART0_RX_IRQ_NUMBER	89	/* BASE		*/
#define UART0_TX_IRQ_NUMBER	90	/* BASE	+ 1	*/
#define UART0_ER_IRQ_NUMBER	91	/* BASE + 2	*/
#define UART1_RX_IRQ_NUMBER	92	/* BASE		*/
#define UART1_TX_IRQ_NUMBER	93	/* BASE	+ 1	*/
#define UART1_ER_IRQ_NUMBER	94	/* BASE + 2	*/
#define UART2_RX_IRQ_NUMBER	95	/* BASE		*/
#define UART2_TX_IRQ_NUMBER	96	/* BASE	+ 1	*/
#define UART2_ER_IRQ_NUMBER	97	/* BASE + 2	*/
#define UART3_RX_IRQ_NUMBER	98	/* BASE		*/
#define UART3_TX_IRQ_NUMBER	99	/* BASE	+ 1	*/
#define UART3_ER_IRQ_NUMBER     100	/* BASE + 2	*/
#define UART4_RX_IRQ_NUMBER	101	/* BASE		*/
#define UART4_TX_IRQ_NUMBER	102	/* BASE	+ 1	*/
#define UART4_ER_IRQ_NUMBER     103	/* BASE + 2	*/
#define UART5_RX_IRQ_NUMBER	104	/* BASE		*/
#define UART5_TX_IRQ_NUMBER	105	/* BASE	+ 1	*/
#define UART5_ER_IRQ_NUMBER     106	/* BASE + 2	*/

#define UART_RX_IRQ_PRI		DEVCNF_UART0_INTPRI
#define UART_RX_IRQ_SUBPRI	0
#define UART_TX_IRQ_PRI		DEVCNF_UART0_INTPRI
#define UART_TX_IRQ_SUBPRI	0

#define UART0_RX_IRQ_PRI	UART_RX_IRQ_PRI
#define UART0_RX_IRQ_SUBPRI	UART_RX_IRQ_SUBPRI
#define UART0_TX_IRQ_PRI	UART_TX_IRQ_PRI
#define UART0_TX_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI
#define UART0_ERR_IRQ_PRI	UART_TX_IRQ_PRI
#define UART0_ERR_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI

#define UART1_RX_IRQ_PRI	UART_RX_IRQ_PRI
#define UART1_RX_IRQ_SUBPRI	UART_RX_IRQ_SUBPRI
#define UART1_TX_IRQ_PRI	UART_TX_IRQ_PRI
#define UART1_TX_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI
#define UART1_ERR_IRQ_PRI	UART_TX_IRQ_PRI
#define UART1_ERR_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI

#define UART2_RX_IRQ_PRI	UART_RX_IRQ_PRI
#define UART2_RX_IRQ_SUBPRI	UART_RX_IRQ_SUBPRI
#define UART2_TX_IRQ_PRI	UART_TX_IRQ_PRI
#define UART2_TX_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI
#define UART2_ERR_IRQ_PRI	UART_TX_IRQ_PRI
#define UART2_ERR_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI

#define UART3_RX_IRQ_PRI	UART_RX_IRQ_PRI
#define UART3_RX_IRQ_SUBPRI	UART_RX_IRQ_SUBPRI
#define UART3_TX_IRQ_PRI	UART_TX_IRQ_PRI
#define UART3_TX_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI
#define UART3_ERR_IRQ_PRI	UART_TX_IRQ_PRI
#define UART3_ERR_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI

#define UART4_RX_IRQ_PRI	UART_RX_IRQ_PRI
#define UART4_RX_IRQ_SUBPRI	UART_RX_IRQ_SUBPRI
#define UART4_TX_IRQ_PRI	UART_TX_IRQ_PRI
#define UART4_TX_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI
#define UART4_ERR_IRQ_PRI	UART_TX_IRQ_PRI
#define UART4_ERR_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI

#define UART5_RX_IRQ_PRI	UART_RX_IRQ_PRI
#define UART5_RX_IRQ_SUBPRI	UART_RX_IRQ_SUBPRI
#define UART5_TX_IRQ_PRI	UART_TX_IRQ_PRI
#define UART5_TX_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI
#define UART5_ERR_IRQ_PRI	UART_TX_IRQ_PRI
#define UART5_ERR_IRQ_SUBPRI	UART_TX_IRQ_SUBPRI


/*----------------------------------------------------------------------*/
/*	Macros for controlling UART					*/
/*----------------------------------------------------------------------*/
#define  ENABLE_UART_TE(port)	(*UART_TRANS(port) |=   UART_TRANS_TXE_ENABLE)
#define DISABLE_UART_TE(port)	(*UART_TRANS(port) &= ~(UART_TRANS_TXE_ENABLE))
#define  ENABLE_UART_RE(port)	(*UART_TRANS(port) |=   UART_TRANS_RXE_ENABLE)
#define DISABLE_UART_RE(port)	(*UART_TRANS(port) &= ~(UART_TRANS_RXE_ENABLE))

#define  ENABLE_TIE_INTRRPT(port) (*UART_CR1(port) |=   UART_CR1_INTTXWE_ENABLE)
#define DISABLE_TIE_INTRRPT(port) (*UART_CR1(port) &= ~(UART_CR1_INTTXWE_ENABLE))
#define  ENABLE_RIE_INTRRPT(port) (*UART_CR1(port) |=   UART_CR1_INTRXWE_ENABLE)
#define DISABLE_RIE_INTRRPT(port) (*UART_CR1(port) &= ~(UART_CR1_INTRXWE_ENABLE))
#define  ENABLE_ERR_INTRRPT(port) (*UART_CR1(port) |=   UART_CR1_INTERR_ENABLE)
#define DISABLE_ERR_INTRRPT(port) (*UART_CR1(port) &= ~(UART_CR1_INTERR_ENABLE))

#define  ENABLE_DTR(port)	/* NOP */
#define DISABLE_DTR(port)	/* NOP */
#define  ENABLE_RTS(port)	/* NOP */
#define DISABLE_RTS(port)	/* NOP */

#define  ENABLE_CTSFLOW(port)	(*UART_CR0(port) |=   UART_CR0_CTSE_ENABLE)
#define DISABLE_CTSFLOW(port)	(*UART_CR0(port) &= ~(UART_CR0_CTSE_ENABLE))
#define  ENABLE_RTSFLOW(port)	(*UART_CR0(port) |=   UART_CR0_RTSE_ENABLE)
#define DISABLE_RTSFLOW(port)	(*UART_CR0(port) &= ~(UART_CR0_RTSE_ENABLE))
#define ISCTSFLOW(port)		(*UART_CR0(port) &    UART_CR0_CTSE_ENABLE)
#define ISRTSFLOW(port)		(*UART_CR0(port) &    UART_CR0_RTSE_ENABLE)

#define SEND_BREAK(port)	(*UART_TRANS(port) |=   UART_TRANS_BK_SEND)
#define STOP_BREAK(port)	(*UART_TRANS(port) &= ~(UART_TRANS_BK_SEND))

/*----------------------------------------------------------------------*/
/*	Macros for sendLine(), sencChar(), getChar() --> sio.c		*/
/*----------------------------------------------------------------------*/
#define UART_TDR(port)		UART_DR(port)		/* Tx Data Register */
#define UART_RDR(port)		UART_DR(port)		/* Rx Data Register */

				/* Wait for the transmission data to be written. */
#define UART_isTXREADY(port)	(getTxSizeInFifo(TERM_PORT)<TX_FIFO_SIZE)

				/* Wait for all data to be transmitted. */
#define UART_isTXCOMPLETE(port)	(getTxSizeInFifo(TERM_PORT)<=0)

				/* Wait for the reception data to be readable. */
#define UART_isRXREADY(port)	(getRxSizeInFifo(TERM_PORT)>0)


/*----------------------------------------------------------------------*/
/*	Default settings of CONSOLE : 8N1, 115.2Kbps			*/
/*	--------------------------------------------------------	*/
/*	UART_XXX are defined in tm_com.c for T-Monitor			*/
/*		--> lib/libtm/sysdepend/sbk_m4nr/tm_com.c		*/
/*----------------------------------------------------------------------*/
#define UART_WORD_LEN	UART_WORD_LEN_8
#define UART_PARITY	UART_PARITY_NONE
#define UART_STOP_BIT	UART_STOP_BIT_1
#define UART_HW_FLOW	UART_HW_FLOW_DISABLE
#define UART_BAUD	(115200)

/*----------------------------------------------------------------------*/
/*	Macros for matching RsMode setting values and bit patterns.	*/
/*	--------------------------------------------------------	*/
/*	It can not deal with parameters that HW does not support.	*/
/*	  e.g) STM32F4xx supports 8 bits or 9 bits as the data length.	*/
/*	In case of undefined value, set default value.			*/
/*----------------------------------------------------------------------*/
/* DN_RSMODE : RsMode */
#define RSMODE_WLEN_0	UART_WORD_LEN		/* datalen = 0 : 5 bit	*/
#define RSMODE_WLEN_1	UART_WORD_LEN		/*	   = 1 : 6 bit	*/
#define RSMODE_WLEN_2	UART_WORD_LEN_7		/*	   = 2 : 7 bit	*/
#define RSMODE_WLEN_3	UART_WORD_LEN_8		/*	   = 3 : 8 bit	*/
#define RSMODE_PBIT_0	UART_PARITY_NONE	/* parity  = 0 : NONE	*/
#define RSMODE_PBIT_1	UART_PARITY_ODD		/*	   = 1 : ODD	*/
#define RSMODE_PBIT_2	UART_PARITY_EVEN	/*	   = 2 : EVEN	*/
#define RSMODE_PBIT_3	UART_PARITY		/*	   = 3 : -	*/
#define RSMODE_SBIT_0	UART_STOP_BIT_1		/* stopbits= 0 : 1 bit	*/
#define RSMODE_SBIT_1	UART_STOP_BIT		/*	   = 1 : 1.5 bit*/
#define RSMODE_SBIT_2	UART_STOP_BIT_2		/*	   = 2 : 2 bit	*/
#define RSMODE_SBIT_3	UART_STOP_BIT		/*	   = 3 : -	*/

/* DN_RSFLOW : RsFlow */
#define RSFLOW_CSFLOW_0	0x00000000ul		/* csflow = 0 : Disable	*/
#define RSFLOW_CSFLOW_1	UART_HW_FLOW_CTS	/*	  = 1 : Enable	*/
#define RSFLOW_RSFLOW_0	0x00000000ul		/* rsflow = 0 : Disable	*/
#define RSFLOW_RSFLOW_1	UART_HW_FLOW_RTS	/*	  = 1 : Enable	*/


/*----------------------------------------------------------------------*/
/*	INLINE FUNCTIONS (Internal use only)				*/
/*----------------------------------------------------------------------*/
/*
 *	Enable / Disable Tx and Rx in UART
 *	It is for polling, INTERRUPTS IS NOT CARE.
 */
Inline	void	enableSndAndRcv( W port )
{
	ENABLE_UART_RE(port);
	ENABLE_UART_TE(port);
}

Inline	void	disableSndAndRcv( W port )
{
	DISABLE_UART_TE(port);
	DISABLE_UART_RE(port);
}

/*
 *	Enable / Disable Interrupts
 */
Inline	void	enableTxInterrupt( W port )
{
	ENABLE_TIE_INTRRPT(port);
}

Inline	void	disableTxInterrupt( W port )
{
	DISABLE_TIE_INTRRPT(port);
}

Inline	void	enableRxInterrupt( W port )
{
	ENABLE_RIE_INTRRPT(port);
}

Inline	void	disableRxInterrupt( W port )
{
	DISABLE_RIE_INTRRPT(port);
}

Inline	void	enableErrInterrupt( W port )
{
	ENABLE_ERR_INTRRPT(port);
}

Inline	void	disableErrInterrupt( W port )
{
	DISABLE_ERR_INTRRPT(port);
}

Inline	void	enableRxRelatedInterrupt( W port )
{
	ENABLE_RIE_INTRRPT(port);
	ENABLE_ERR_INTRRPT(port);
}

Inline	void	disableRxRelatedInterrupt( W port )
{
	DISABLE_ERR_INTRRPT(port);
	DISABLE_RIE_INTRRPT(port);
}

/*
 *	Line Control (DTR/RTS)
 */
Inline	void	enableLineDTR( W port )
{
	ENABLE_DTR(port);
}

Inline	void	disableLineDTR( W port )
{
	DISABLE_DTR(port);
}

Inline	void	enableLineRTS( W port )
{
	ENABLE_RTS(port);
}

Inline	void	disableLineRTS( W port )
{
	DISABLE_RTS(port);
}

Inline	void	sendBreakSignal( W port )
{
	SEND_BREAK(port);
}

Inline	void	stopBreakSignal( W port )
{
	STOP_BREAK(port);
}

/*
 *	Line Status (CI=RI/CS=CTS/CD=DCD/DR=DSR/BD)
 */
Inline	UW	getLineStatusCI( W port )	/* CI : Calling Indicator(RI) */
{
	return 0;
}

Inline	UW	getLineStatusCS( W port )	/* CS : Clear to Send (CTS) */
{
	return 0;	/* This bit does not exist in the status register. */
}

Inline	UW	getLineStatusCD( W port )	/* CD : Carrier Detect (DCD) */
{
	return 0;
}

Inline	UW	getLineStatusDR( W port )	/* DR : Dataset Ready (DSR) */
{
	return 0;
}

Inline	UW	getLineStatusBD( W port )	/* BD : Break Detect */
{
	return 0;
}

Inline	UW	getLineStatusXF( W port )	/* Recv XOFF */
{
	return 0;
}

/*
 *	set HW flow control (CTS/RTS)
 */
Inline	void	enableFlowControlCTS( W port )
{
	ENABLE_CTSFLOW(port);
}

Inline	void	disableFlowControlCTS( W port )
{
	DISABLE_CTSFLOW(port);
}

Inline	void	enableFlowControlRTS( W port )
{
	ENABLE_RTSFLOW(port);
}

Inline	void	disableFlowControlRTS( W port )
{
	DISABLE_RTSFLOW(port);
}

/*
 *	Get Control Settings
 */
Inline	UW	getFlowControlSettingsCTS( W port )
{
	return !!ISCTSFLOW(port);
}

Inline	UW	getFlowControlSettingsRTS( W port )
{
	return !!ISRTSFLOW(port);
}

/*
 *	Get Readable Data Size in Rx FIFO.
 *	Call this function after confirming that there is received data.
 */
Inline	W	getRxSizeInFifo( W port )
{
#if	RX_FIFO_SIZE == 0
	return 1;	/* 1 means that data is stored in the RxD register. */
#else
	return *UART_SR(port) & UART_SR_RLVL_STAT;
				/* [SR]<RLVL>[3:0] */
#endif
}

/*
 *	Get Data Size in Tx FIFO to wait Transmit.
 */
Inline	W	getTxSizeInFifo( W port )
{
#if	TX_FIFO_SIZE == 0
	return 0;	/* 0 means that there is NO Tx FIFO. */
#else
	return  (*UART_SR(port) & UART_SR_TLVL_STAT) >> 8;
				/* [SR]<TLVL>[11:8] */
#endif
}

/*----------------------------------------------------------------------*/
/*	INLINE FUNCTIONS : Share with CONSOLE (T-Monitor)		*/
/*			--> lib/libtm/src/sysdepend/[TARGET]/sio.c	*/
/*----------------------------------------------------------------------*/
/*
 *	Set System Clock (RCC) for Enable IP Clock.
 */
Inline	ER	initUartRcc( W port )
{
	switch( port ){
	  case	0:	/* UART0 */
		*CGPROTECT = 0xC1;
//		*CGFSYSMENA |= (CGFSYSMENA_PC | CGFSYSMENA_UART0);
		*CGFSYSMENA |= CGFSYSMENA_UART0;
		*CGPROTECT = 0x00;
		break;
	  case	1:	/* UART1 */
		*CGFSYSMENA |= CGFSYSMENA_UART1;
		break;
	  case	2:	/* UART2 */
		*CGFSYSMENA |= CGFSYSMENA_UART2;
		break;
	  case	3:	/* UART3 */
		*CGFSYSMENA |= CGFSYSMENA_UART3;
		break;
	  default:
		return E_PAR;
	}

	return E_OK;
}

/*
 *	Set GPIO register to select I/O Multi-function.
 */
IMPORT	void	set_port_func(INT port, INT no, UW func, UW inout );
Inline	ER	initUartGpio( W port )
{
	switch( port ){
	  case	0:	/* UART0 */
		set_port_func( 'E', 3, PE3_UT0TXDA,	PORT_OUT );
		set_port_func( 'E', 2, PE2_UT0RXD,	PORT_IN );
		break;
	  case	1:	/* UART1 */
	  case	2:	/* UART2 */
	  case	3:	/* UART3 */
		return E_NOSPT;
	  default:
		return E_PAR;
	}
	return E_OK;
}

/*
 *	Initialize Uart Port.
 *	"wl", "par", "sb", "flow" are bitpatern (UART_XXX).
 *	"baud" must be over 1200(bps).
 */
typedef	struct	_brdTbl {
	INT	baud;		/*   <-- Baud Rate */
	UB	ken;		/*   --> [UART_BRD]<ken[23:23]> */
	UB	brk;		/* K --> [UART_BRD]<brk[21:16]> */
	UH	brn;		/* N --> [UART_BRD]<brn[15:0]> */
} BRDTBL;
Inline	void	initUartParam( W port, UW wl, UW par, UW sb, UW baud, UW flow )
{
const	BRDTBL	brdTbl[] = {	 /* baud  KEN BRK     BRN */
	{115200, 1, 48,   54 }, {57600, 1, 32,  108 }, {38400, 1, 15,  162 },
	{ 19200, 1, 31,  325 }, { 9600, 1, 61,  651 }, { 4800, 1, 59, 1302 },
	{  2400, 1, 53, 2596 }, { 1200, 1, 43, 5208 }, {0,0,0,0}  };
        INT	idx;

	/*------------------------------*/
	/*	Check Baudrate		*/
	/*------------------------------*/
	for( idx = 0; brdTbl[idx].baud != 0; idx++ ){
		if( baud == brdTbl[idx].baud ){
			break;
		}
	}
	if( brdTbl[idx].baud == 0 ){
		return;		/* Baudrate Error */
	}

	/*------------------------------*/
	/*	SW Reset & FIFO CLR	*/
	/*------------------------------*/
	while( (*UART_SWRST(port) & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );
	*UART_SWRST(port) = UART_SWRST_SWRST_1ST;
	*UART_SWRST(port) = UART_SWRST_SWRST_2ND;
	while( (*UART_SWRST(port) & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );

	*UART_FIFOCLR(port) = UART_FIFOCLR_TFCLR | UART_FIFOCLR_RFCLR;

	/*------------------------------*/
	/*	BAUDRATE		*/
	/*------------------------------*/
	*UART_CLK(port) = UART_CLK_PRSEL_D1;	/* FTx = FT0/1 */
	*UART_BRD(port) = ((UW)brdTbl[idx].ken << 23)
			| ((UW)brdTbl[idx].brk << 16)
			|  (UW)brdTbl[idx].brn;

	/*------------------------------*/
	/*	PARAMETERS		*/
	/*------------------------------*/
	*UART_CR0(port) = UART_CR0_HBSEN_DISABLE | UART_CR0_LPB_DISABLE | UART_CR0_NF_NONE
			| UART_CR0_CTSE_DISABLE | UART_CR0_RTSE_DISABLE | UART_CR0_WU_DISABLE
			| UART_CR0_IV_DISABLE | UART_CR0_DIR_LSB
			| sb | par | wl | flow;

	/*------------------------------*/
	/*	INTERRUPTS -> disable	*/
	/*------------------------------*/
	*UART_CR1(port) = UART_CR1_TIL_0 | UART_CR1_RIL_1
			| UART_CR1_INTTXFE_DISABLE | UART_CR1_INTTXWE_DISABLE
			| UART_CR1_INTRXFE_DISABLE | UART_CR1_INTRXWE_DISABLE
			| UART_CR1_INTERR_DISABLE
			| UART_CR1_DMATE_DISABLE | UART_CR1_DMARE_DISABLE;
}

#endif	/* __DEV_SER_M4NR_H__ */
