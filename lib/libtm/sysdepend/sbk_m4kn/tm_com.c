/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the SBK-M4KN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (SBK-M4KN)
 */

#include <tk/typedef.h>
#include <sys/sysdef.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef SBK_M4KN
#if TM_COM_SERIAL_DEV

#define UART0_BASE	(0x400CE000ul)		// TYPE2 : BASE + port*0x400
#define UART1_BASE	(0x400CE400ul)
#define UART2_BASE	(0x400CE800ul)
#define UART3_BASE	(0x400CEC00ul)

#define UART_BASE	UART0_BASE		// T-Monitor uses UART0

/*------ UART Registers ------*/
#define UART_SWRST	(_UW*)(UART_BASE+0x0000ul)	/* Software reset register */
#define UART_CR0	(_UW*)(UART_BASE+0x0004ul)	/* Control register 0 */
#define UART_CR1	(_UW*)(UART_BASE+0x0008ul)	/* Control register 1 */
#define UART_CLK	(_UW*)(UART_BASE+0x000Cul)	/* Clock control register */
#define UART_BRD	(_UW*)(UART_BASE+0x0010ul)	/* Baud rate register */
#define UART_TRANS	(_UW*)(UART_BASE+0x0014ul)	/* Transfer enable register */
#define UART_DR		(_UW*)(UART_BASE+0x0018ul)	/* Data register */
#define UART_SR		(_UW*)(UART_BASE+0x001Cul)	/* Status register */
#define UART_FIFOCLR	(_UW*)(UART_BASE+0x0020ul)	/* FIFO clear register */
#define UART_ERR	(_UW*)(UART_BASE+0x0024ul)	/* Error register */

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



#define TX_FIFO_SIZE	8
#define RX_FIFO_SIZE	8

// Get Data Size in Tx FIFO to wait Transmit.	[SR]<TLVL>[11:8]
Inline	W getTxSizeInFifo(void){ return (*UART_SR & UART_SR_TLVL_STAT)>>8; }

// Get Readable Data Size in Rx FIFO.		[SR]<RLVL>[3:0]
Inline	W getRxSizeInFifo(void){ return *UART_SR & UART_SR_RLVL_STAT; }

				/* Wait for the transmission data to be written. */
#define UART_isTXREADY		(getTxSizeInFifo()<TX_FIFO_SIZE)

				/* Wait for all data to be transmitted. */
#define UART_isTXCOMPLETE	(getTxSizeInFifo()<=0)

				/* Wait for the reception data to be readable. */
#define UART_isRXREADY		(getRxSizeInFifo()>0)

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while( !UART_isTXREADY );
		*UART_DR = *b;
	}
	while( !UART_isTXCOMPLETE );	
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while( !UART_isRXREADY );
		*buf = *UART_DR & 0xff;
	}
}

/*----------------------------------------------------------------------*/
/*	Initialize UART for T-Monitor (CONSOLE)				*/
/*----------------------------------------------------------------------*/
#define UART_BAUD	(115200)
#define UART_WORD_LEN	UART_WORD_LEN_8
#define UART_PARITY	UART_PARITY_NONE
#define UART_STOP_BIT	UART_STOP_BIT_1
#define UART_HW_FLOW	UART_HW_FLOW_DISABLE

#if   UART_BAUD == 115200
#define BRD_KEN		1
#define BRD_BRK		48
#define BRD_BRN		54
#elif UART_BAUD ==  57600
#define BRD_KEN		1
#define BRD_BRK		32
#define BRD_BRN		108
#elif UART_BAUD ==  38400
#define BRD_KEN		1
#define BRD_BRK		15
#define BRD_BRN		162
#elif UART_BAUD ==  19200
#define BRD_KEN		1
#define BRD_BRK		31
#define BRD_BRN		325
#elif UART_BAUD ==   9600
#define BRD_KEN		1
#define BRD_BRK		61
#define BRD_BRN		651
#elif UART_BAUD ==   4800
#define BRD_KEN		1
#define BRD_BRK		59
#define BRD_BRN		1302
#elif UART_BAUD ==   2400
#define BRD_KEN		1
#define BRD_BRK		53
#define BRD_BRN		2596
#elif UART_BAUD ==   1200
#define BRD_KEN		1
#define BRD_BRK		43
#define BRD_BRN		5208
#else
#error "Your Baudrate is NOT SUPPORTED"
#endif

EXPORT	void	tm_com_init(void)
{
	/* DISABLE Send & Receive */
	*UART_TRANS &= ~(UART_TRANS_TXE_ENABLE);
	*UART_TRANS &= ~(UART_TRANS_RXE_ENABLE);

	/* Set UART parameters */
	// SW Reset & FIFO CLR
	while( (*UART_SWRST & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );
	*UART_SWRST = UART_SWRST_SWRST_1ST;
	*UART_SWRST = UART_SWRST_SWRST_2ND;
	while( (*UART_SWRST & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );

	*UART_FIFOCLR = UART_FIFOCLR_TFCLR | UART_FIFOCLR_RFCLR;

	// BAUDRATE
	*UART_CLK = UART_CLK_PRSEL_D1;	/* FTx = FT0/1 */
	*UART_BRD = ((UW)BRD_KEN << 23)|((UW)BRD_BRK << 16)|((UW)BRD_BRN);

	// PARAMETERS
	*UART_CR0 = UART_STOP_BIT | UART_PARITY | UART_WORD_LEN
		  | UART_CR0_HBSEN_DISABLE | UART_CR0_LPB_DISABLE | UART_CR0_NF_NONE
		  | UART_CR0_CTSE_DISABLE | UART_CR0_RTSE_DISABLE | UART_CR0_WU_DISABLE
		  | UART_CR0_IV_DISABLE | UART_CR0_DIR_LSB;

	// INTERRUPTS
	*UART_CR1 = UART_CR1_TIL_0 | UART_CR1_RIL_1
		  | UART_CR1_INTTXFE_DISABLE | UART_CR1_INTTXWE_DISABLE
		  | UART_CR1_INTRXFE_DISABLE | UART_CR1_INTRXWE_DISABLE
		  | UART_CR1_INTERR_DISABLE
		  | UART_CR1_DMATE_DISABLE | UART_CR1_DMARE_DISABLE;

	/* ENABLE Send & Receive */
	*UART_TRANS |= UART_TRANS_TXE_ENABLE;
	*UART_TRANS |= UART_TRANS_RXE_ENABLE;
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* SBK_M4KN */
#endif /* USE_TMONITOR */
