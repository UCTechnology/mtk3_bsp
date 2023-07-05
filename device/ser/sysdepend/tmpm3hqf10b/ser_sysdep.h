/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
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
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for ADBUN_TMPM3HQF10B
 */

#ifndef	__DEV_SER_TMPM3HQF10B_H__
#define	__DEV_SER_TMPM3HQF10B_H__

#define	DEV_SER_UNITNM		(1)	/* Number of devive channels */
#define DEV_SER_UNIT0		(0)	/* UART Ch.0 */

/*
 * UART registers
 */

/* Base address for each channel */
/*
 * UARTn : n = 0 - 5
 */
#define UART_BASE	(0x400BB000ul)
#define UART0_BASE	(0x400BB000ul)
#define UART1_BASE	(0x400BB100ul)
#define UART2_BASE	(0x400BB200ul)
#define UART3_BASE	(0x400BB300ul)
#define UART4_BASE	(0x400BBD00ul)	/* <-- ATTENTION!! Discontinuity */
#define UART5_BASE	(0x400BBE00ul)

/* Register offset */
#define UARTxSWRS		(0x0000ul)	/* Software reset register */
#define UARTxCR0		(0x0004ul)	/* Control register 0 */
#define UARTxCR1		(0x0008ul)	/* Control register 1 */
#define UARTxCLK		(0x000Cul)	/* Clock control register */
#define UARTxBRD		(0x0010ul)	/* Baud rate register */
#define UARTxTRANS		(0x0014ul)	/* Transfer enable register */
#define UARTxDR			(0x0018ul)	/* Data register */
#define UARTxSR			(0x001Cul)	/* Status register */
#define UARTxFIFOCLR	(0x0020ul)	/* FIFO clear register */
#define UARTxERR		(0x0024ul)	/* Error register */

/* Register bit definition */
/*------ VALUE in UARTxSWRST Register ------*/
#define UARTxSWRST_SWRSTF_STAT	0x00000080ul	/* Software reset flag */
#define UARTxSWRST_SWRSTF_DONE	0x00000000ul	/*	Not Resetting */
#define UARTxSWRST_SWRSTF_RUN	0x00000080ul	/*	Resetting Now */
#define UARTxSWRST_SWRST_MASK	0x00000003ul	/* Software reset */
#define UARTxSWRST_SWRST_1ST	0x00000002ul	/*	1st command */
#define UARTxSWRST_SWRST_2ND	0x00000001ul	/*	2nd command */
/*------ VALUE in UARTxCR0 Register ------*/
#define UARTxCR0_HBSST_MASK	0x00040000ul	/* half clock mode communication start control */
#define UARTxCR0_HBSST_UTxTXDA	0x00000000ul	/*	Output starts from the UTxTXDA pin */
#define UARTxCR0_HBSST_UTxTXDB	0x00040000ul	/*	Output starts from the UTxTXDB terminal */
#define UARTxCR0_HBSMD_MASK	0x00020000ul	/* half clock mode communication output control */
#define UARTxCR0_HBSMD_1PIN	0x00000000ul    /*	1 pin mode */
#define UARTxCR0_HBSMD_2PIN	0x00020000ul	/*	2 pin mode */
#define UARTxCR0_HBSEN_MASK	0x00010000ul	/* half clock mode control */
#define UARTxCR0_HBSEN_DISABLE	0x00000000ul	/*	Disable */
#define UARTxCR0_HBSEN_ENABLE	0x00010000ul	/*	Enable */
#define UARTxCR0_LPB_MASK	0x00008000ul	/* loopback function */
#define UARTxCR0_LPB_DISABLE	0x00000000ul	/*	Disable */
#define UARTxCR0_LPB_ENABLE	0x00008000ul	/*	Enable */
#define UARTxCR0_NF_MASK	0x00007000ul	/* UTxRXD noise elimination time selection */
#define UARTxCR0_NF_NONE	0x00000000ul	/*	No Noise Filter */
#define UARTxCR0_NF_F2		0x00001000ul	/*	2 cycles or less with Ft0 clock */
#define UARTxCR0_NF_F4		0x00002000ul	/*	4 cycles or less with Ft0 clock */
#define UARTxCR0_NF_F8		0x00003000ul	/*	8 cycles or less with Ft0 clock */
#define UARTxCR0_NF_T2		0x00004000ul	/*	2 cycles or less with transfer clock */
#define UARTxCR0_NF_T3		0x00005000ul	/*	3 cycles or less with transfer clock */
#define UARTxCR0_NF_T4		0x00006000ul	/*	4 cycles or less with transfer clock */
#define UARTxCR0_NF_T5		0x00007000ul	/*	5 cycles or less with transfer clock */
#define UARTxCR0_CTSE_MASK	0x00000400ul	/* CTS handshake function */
#define UARTxCR0_CTSE_DISABLE	0x00000000ul	/*	Disable */
#define UARTxCR0_CTSE_ENABLE	0x00000400ul	/*	Enable */
#define UARTxCR0_RTSE_MASK	0x00000200ul	/* RTS handshake function */
#define UARTxCR0_RTSE_DISABLE	0x00000000ul	/*	Disable */
#define UARTxCR0_RTSE_ENABLE	0x00000200ul	/*	Enable */
#define UARTxCR0_WU_MASK	0x00000100ul	/* wakeup function */
#define UARTxCR0_WU_DISABLE	0x00000000ul	/*	Disable */
#define UARTxCR0_WU_ENABLE	0x00000100ul	/*	Enable */
#define UARTxCR0_IV_MASK	0x00000040ul	/* data signal inversion function */
#define UARTxCR0_IV_DISABLE	0x00000000ul	/*	Disable (not inverted) */
#define UARTxCR0_IV_ENABLE	0x00000040ul	/*	Valid (invert) */
#define UARTxCR0_DIR_MASK	0x00000020ul	/* data transfer direction selection */
#define UARTxCR0_DIR_LSB	0x00000000ul    /*	LSB first */
#define UARTxCR0_DIR_MSB	0x00000020ul	/*	MSB first */
#define UARTxCR0_SBLEN_MASK	0x00000010ul	/* Stop bit length selection */
#define UARTxCR0_SBLEN_1	0x00000000ul	/*	1 bit */
#define UARTxCR0_SBLEN_2	0x00000010ul	/*	2 bits */
#define UARTxCR0_EVEN_MASK	0x00000008ul	/* Even parity selection */
#define UARTxCR0_EVEN_ODD	0x00000000ul	/* 	Odd parity */
#define UARTxCR0_EVEN_EVEN	0x00000008ul	/*	Even parity */
#define UARTxCR0_PE_MASK	0x00000004ul	/* Parity addition function */
#define UARTxCR0_PE_NONE	0x00000000ul	/*	None */
#define UARTxCR0_PE_ENABLE	0x00000004ul	/*	Enable */
#define UARTxCR0_SM_MASK	0x00000003ul	/* Data length selection */
#define UARTxCR0_SM_7		0x00000000ul	/*	7 bits */
#define UARTxCR0_SM_8		0x00000001ul	/*	8 bits */
#define UARTxCR0_SM_9		0x00000002ul	/*	9 bits */
/*------ VALUE in UARTxCR1 Register ------*/
#define UARTxCR1_TIL_MASK	 0x00007000ul	/* Transmission Fill level setting */
#define UARTxCR1_TIL_0		 0x00000000ul	/*	Stage 0 */
#define UARTxCR1_TIL_1		 0x00001000ul	/*	Stage 1 */
#define UARTxCR1_TIL_2		 0x00002000ul	/*	Stage 2 */
#define UARTxCR1_TIL_3		 0x00003000ul	/*	Stage 3 */
#define UARTxCR1_TIL_4		 0x00004000ul	/*	Stage 4 */
#define UARTxCR1_TIL_5		 0x00005000ul	/*	Stage 5 */
#define UARTxCR1_TIL_6		 0x00006000ul	/*	Stage 6 */
#define UARTxCR1_TIL_7		 0x00007000ul	/*	Stage 7 */
#define UARTxCR1_RIL_MASK	 0x00000700ul	/* Reception Fill level setting */
#define UARTxCR1_RIL_8		 0x00000000ul	/*	Stage 8 */
#define UARTxCR1_RIL_1		 0x00000100ul	/*	Stage 1 */
#define UARTxCR1_RIL_2		 0x00000200ul	/*	Stage 2 */
#define UARTxCR1_RIL_3		 0x00000300ul	/*	Stage 3 */
#define UARTxCR1_RIL_4		 0x00000400ul	/*	Stage 4 */
#define UARTxCR1_RIL_5		 0x00000500ul	/*	Stage 5 */
#define UARTxCR1_RIL_6		 0x00000600ul	/*	Stage 6 */
#define UARTxCR1_RIL_7		 0x00000700ul	/*	Stage 7 */
#define UARTxCR1_INTTXFE_MASK	 0x00000080ul	/* Transmit FIFO interrupt control */
#define UARTxCR1_INTTXFE_DISABLE 0x00000000ul	/*	Disable */
#define UARTxCR1_INTTXFE_ENABLE	 0x00000080ul	/*	Enable */
#define UARTxCR1_INTTXWE_MASK	 0x00000040ul	/* Transmission completion interrupt control */
#define UARTxCR1_INTTXWE_DISABLE 0x00000000ul	/*	Disable */
#define UARTxCR1_INTTXWE_ENABLE	 0x00000040ul	/*	Enable */
#define UARTxCR1_INTRXFE_MASK	 0x00000020ul	/* Receive FIFO interrupt control */
#define UARTxCR1_INTRXFE_DISABLE 0x00000000ul	/*	Disable */
#define UARTxCR1_INTRXFE_ENABLE	 0x00000020ul	/*	Enable */
#define UARTxCR1_INTRXWE_MASK	 0x00000010ul	/* Reception complete interrupt control */
#define UARTxCR1_INTRXWE_DISABLE 0x00000000ul	/*	Disable */
#define UARTxCR1_INTRXWE_ENABLE	 0x00000010ul	/*	Enable */
#define UARTxCR1_INTERR_MASK	 0x00000004ul	/* Error interrupt control */
#define UARTxCR1_INTERR_DISABLE	 0x00000000ul	/*	Disable */
#define UARTxCR1_INTERR_ENABLE	 0x00000004ul	/*	Enable */
#define UARTxCR1_DMATE_MASK	 0x00000002ul	/* Transmission DMA control */
#define UARTxCR1_DMATE_DISABLE	 0x00000000ul	/*	Disable */
#define UARTxCR1_DMATE_ENABLE	 0x00000002ul	/*	Enable */
#define UARTxCR1_DMARE_MASK	 0x00000001ul	/* Reception DMA control */
#define UARTxCR1_DMARE_DISABLE	 0x00000000ul	/*	Disable */
#define UARTxCR1_DMARE_ENABLE	 0x00000001ul	/*	Enable */
/*------ VALUE in UARTxCLK Register ------*/
#define UARTxCLK_PRSEL_MASK	0x000000F0ul	/* Prescaler division selection */
#define UARTxCLK_PRSEL_D1	0x00000000ul	/*	1/1 */
#define UARTxCLK_PRSEL_D2	0x00000010ul	/*	1/2 */
#define UARTxCLK_PRSEL_D4	0x00000020ul	/*	1/4 */
#define UARTxCLK_PRSEL_D8	0x00000030ul	/*	1/8 */
#define UARTxCLK_PRSEL_D16	0x00000040ul	/*	1/16 */
#define UARTxCLK_PRSEL_D32	0x00000050ul	/*	1/32 */
#define UARTxCLK_PRSEL_D64	0x00000060ul	/*	1/64 */
#define UARTxCLK_PRSEL_D128	0x00000070ul	/*	1/128 */
#define UARTxCLK_PRSEL_D256	0x00000080ul	/*	1/256 */
#define UARTxCLK_PRSEL_D512	0x00000090ul	/*	1/512 */
/*------ VALUE in UARTxBRD Register ------*/
#define UARTxBRD_KEN_MASK	0x00800000ul	/* Control of N+(64-K)/64 division function. */
#define UARTxBRD_KEN_DISABLE	0x00000000ul	/*	Disable */
#define UARTxBRD_KEN_ENABLE	0x00800000ul	/*	Enable */
#define UARTxBRD_BRK_MASK	0x003F0000ul	/* Setting of 'K' in N+(64-K)/64 */
#define UARTxBRD_BRN_MASK	0x0000FFFFul	/* Setting of 'N' in N+(64-K)/64 or N division */
/*------ VALUE in UARTxTRANS Register ------*/
#define UARTxTRANS_BK_MASK	 0x00000008ul	/* Break Signal control */
#define UARTxTRANS_BK_STOP	 0x00000000ul	/*	Stop Signal */
#define UARTxTRANS_BK_SEND	 0x00000008ul	/*	Send Signal */
#define UARTxTRANS_TXTRG_MASK	 0x00000004ul	/* Trigger transmission control */
#define UARTxTRANS_TXTRG_DISABLE 0x00000000ul	/*	Disable */
#define UARTxTRANS_TXTRG_ENABLE	 0x00000004ul	/*	Enable */
#define UARTxTRANS_TXE_MASK	 0x00000002ul	/* Transmission control */
#define UARTxTRANS_TXE_DISABLE	 0x00000000ul	/*	Disable */
#define UARTxTRANS_TXE_ENABLE	 0x00000002ul	/*	Enable */
#define UARTxTRANS_RXE_MASK	 0x00000001ul	/* Reception control */
#define UARTxTRANS_RXE_DISABLE	 0x00000000ul	/*	Disable */
#define UARTxTRANS_RXE_ENABLE	 0x00000001ul	/*	Enable */
/*------ VALUE in UARTxDR Register ------*/
#define UARTxDR_PERR_MASK	0x00040000ul	/* Parity error */
#define UARTxDR_PERR_NONE	0x00000000ul	/*	NO Error */
#define UARTxDR_PERR_ERR	0x00000000ul	/*	With Error */
#define UARTxDR_FERR_MASK	0x00020000ul	/* Framing error */
#define UARTxDR_FERR_NONE	0x00000000ul	/*	NO Error */
#define UARTxDR_FERR_ERR	0x00000000ul	/*	With Error */
#define UARTxDR_BERR_MASK	0x00010000ul	/* Break error flag */
#define UARTxDR_BERR_NONE	0x00000000ul	/*	NO Error */
#define UARTxDR_BERR_ERR	0x00000000ul	/*	With Error */
#define UARTxDR_DR_STAT		0x000001FFul	/* Receive data / Transmit data */
/*------ VALUE in UARTxSR Register ------*/
#define UARTxSR_SUE_STAT	0x80000000ul	/* Settable state flag */
#define UARTxSR_SUE_ABLE	0x00000000ul	/*	Able to set */
#define UARTxSR_SUE_UNABLE	0x80000000ul	/*	Unable to set */
#define UARTxSR_TXRUN_STAT	0x00008000ul	/* Transmission in operation flag */
#define UARTxSR_TXRUN_STOP	0x00000000ul	/*	Transmission stopped */
#define UARTxSR_TXRUN_RUN	0x00008000ul	/*	Transmitting operation in progress */
#define UARTxSR_TXEND_MASK	0x00004000ul	/* Transmission completion flag */
#define UARTxSR_TXEND_DONE	0x00004000ul	/*	Send completely */
#define UARTxSR_TXEND_CLR	0x00004000ul	/*	Clear TXEND flag */
#define UARTxSR_TXFF_MASK	0x00002000ul	/* Transmit FIFO flag */
#define UARTxSR_TXFF_FILL	0x00002000ul	/*	Reached set transmission fill level */
#define UARTxSR_TXFF_CLR	0x00002000ul	/*	Clear FIFO Flag */
#define UARTxSR_TLVL_STAT	0x00000F00ul	/* Transmit FIFO fill level status */
#define UARTxSR_RXRUN_STAT	0x00000080ul	/* Receiving operation flag */
#define UARTxSR_RXRUN_STOP	0x00000000ul	/*	Stop receiving */
#define UARTxSR_RXRUN_RUN	0x00000080ul	/*	Reception operation in progress */
#define UARTxSR_RXEND_MASK	0x00000040ul	/* Reception complete flag */
#define UARTxSR_RXEND_DONE	0x00000040ul	/*	Receive completely */
#define UARTxSR_RXEND_CLR	0x00000040ul	/*	Clear RXEND flag */
#define UARTxSR_RXFF_MASK	0x00000020ul	/* Receive FIFO flag */
#define UARTxSR_RXFF_FILL	0x00000020ul	/*	Reached set receive fill level */
#define UARTxSR_RXFF_CLR	0x00000020ul	/*	Clear FIFO Flag */
#define UARTxSR_RLVL_STAT	0x0000000Ful	/* Receive FIFO Fill level status */
/*------ VALUE in UARTxFIFOCLR Register ------*/
#define UARTxFIFOCLR_TFCLR	0x00000002ul	/* Transmit buffer clear */
#define UARTxFIFOCLR_RFCLR	0x00000001ul	/* Receive buffer clear */
/*------ VALUE in UARTxERR Register ------*/
#define UARTxERR_TRGERR_MASK	0x00000010ul	/* Trigger transmission error flag */
#define UARTxERR_TRGERR_ERR	0x00000010ul	/*	Detect error */
#define UARTxERR_TRGERR_CLR	0x00000010ul	/*	Clear Flag */
#define UARTxERR_OVRERR_MASK	0x00000008ul	/* Overrun error flag */
#define UARTxERR_OVRERR_ERR	0x00000008ul	/*	Detect error */
#define UARTxERR_OVRERR_CLR	0x00000008ul	/*	Clear Flag */
#define UARTxERR_PERR_MASK	0x00000004ul	/* Parity error flag */
#define UARTxERR_PERR_ERR	0x00000004ul	/*	Detect error */
#define UARTxERR_PERR_CLR	0x00000004ul	/*	Clear Flag */
#define UARTxERR_FERR_MASK	0x00000002ul	/* Framing error flag */
#define UARTxERR_FERR_ERR	0x00000002ul	/*	Detect error */
#define UARTxERR_FERR_CLR	0x00000002ul	/*	Clear Flag */
#define UARTxERR_BERR_MASK	0x00000001ul	/* Break error flag */
#define UARTxERR_BERR_ERR	0x00000001ul	/*	Detect error */
#define UARTxERR_BERR_CLR	0x00000001ul	/*	Clear Flag */

/* UART interrupt number */
/*
 * UART Interrupts
 */
#define INTNO_UART0_RX		67
#define INTNO_UART0_TX		68
#define INTNO_UART0_ER		69
#define INTNO_UART1_RX		70
#define INTNO_UART1_TX		71
#define INTNO_UART1_ER		72
#define INTNO_UART2_RX		73
#define INTNO_UART2_TX		74
#define INTNO_UART2_ER		75
#define INTNO_UART3_RX		76
#define INTNO_UART3_TX		77
#define INTNO_UART3_ER		78
#define INTNO_UART4_RX		79
#define INTNO_UART4_TX		80
#define INTNO_UART4_ER		81
#define INTNO_UART5_RX		82
#define INTNO_UART5_TX		83
#define INTNO_UART5_ER		84

#endif		/* __DEV_SER_TMPM3HQF10B_H__ */
