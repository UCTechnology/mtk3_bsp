/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (ADBUN-TMPM3HQF10B )
 */

#include <tk/typedef.h>
#include <sys/sysdef.h>
#include <tk/errno.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef ADBUN_TMPM3HQF10B
#if TM_COM_SERIAL_DEV

#define UART_CH0 (0)
#define TERM_PORT (UART_CH0)
#define MAX_UART_NO		(6)	/* = 1 - 8 : Number of UART ports */

#define UART_BASE	(0x400BB000ul)
#define UART0_BASE	(0x400BB000ul)
#define UART1_BASE	(0x400BB100ul)
#define UART2_BASE	(0x400BB200ul)
#define UART3_BASE	(0x400BB300ul)
#define UART4_BASE	(0x400BBD00ul)	/* <-- ATTENTION!! Discontinuity */
#define UART5_BASE	(0x400BBE00ul)

#define UARTxSWRST(p)	(_UW*)(uart_reg_base[p]+0x0000ul)	/* Software reset register */
#define UARTxCR0(p)	(_UW*)(uart_reg_base[p]+0x0004ul)	/* Control register 0 */
#define UARTxCR1(p)	(_UW*)(uart_reg_base[p]+0x0008ul)	/* Control register 1 */
#define UARTxCLK(p)	(_UW*)(uart_reg_base[p]+0x000Cul)	/* Clock control register */
#define UARTxBRD(p)	(_UW*)(uart_reg_base[p]+0x0010ul)	/* Baud rate register */
#define UARTxTRANS(p)	(_UW*)(uart_reg_base[p]+0x0014ul)	/* Transfer enable register */
#define UARTxDR(p)	(_UW*)(uart_reg_base[p]+0x0018ul)	/* Data register */
#define UARTxSR(p)	(_UW*)(uart_reg_base[p]+0x001Cul)	/* Status register */
#define UARTxFIFOCLR(p)	(_UW*)(uart_reg_base[p]+0x0020ul)	/* FIFO clear register */

/*------ VALUE in UARTxSWRST Register ------*/
#define UART_SWRST_SWRSTF_STAT		0x00000080ul	/* Software reset flag */
#define UART_SWRST_SWRSTF_DONE		0x00000000ul	/*	Not Resetting */
#define UART_SWRST_SWRSTF_RUN		0x00000080ul	/*	Resetting Now */
#define UART_SWRST_SWRST_MASK		0x00000003ul	/* Software reset */
#define UART_SWRST_SWRST_1ST		0x00000002ul	/*	1st command */
#define UART_SWRST_SWRST_2ND		0x00000001ul	/*	2nd command */
/*------ VALUE in UARTxCR0 Register ------*/
#define UART_CR0_HBSEN_DISABLE		0x00000000ul	/*	Disable */
#define UART_CR0_LPB_DISABLE		0x00000000ul	/*	Disable */
#define UART_CR0_NF_NONE		0x00000000ul	/*	No Noise Filter */
#define UART_CR0_CTSE_MASK		0x00000400ul	/* CTS handshake function */
#define UART_CR0_CTSE_DISABLE		0x00000000ul	/*	Disable */
#define UART_CR0_CTSE_ENABLE		0x00000400ul	/*	Enable */
#define UART_CR0_RTSE_MASK		0x00000200ul	/* RTS handshake function */
#define UART_CR0_RTSE_DISABLE		0x00000000ul	/*	Disable */
#define UART_CR0_RTSE_ENABLE		0x00000200ul	/*	Enable */
#define UART_CR0_WU_DISABLE		0x00000000ul	/*	Disable */
#define UART_CR0_IV_DISABLE		0x00000000ul	/*	Disable (not inverted) */
#define UART_CR0_DIR_LSB		0x00000000ul   	/*	LSB first */
#define UART_CR0_SBLEN_MASK		0x00000010ul	/* Stop bit length selection */
#define UART_CR0_SBLEN_1		0x00000000ul	/*	1 bit */
#define UART_CR0_SBLEN_2		0x00000010ul	/*	2 bits */
#define UART_CR0_EVEN_MASK		0x00000008ul	/* Even parity selection */
#define UART_CR0_EVEN_ODD		0x00000000ul	/* 	Odd parity */
#define UART_CR0_EVEN_EVEN		0x00000008ul	/*	Even parity */
#define UART_CR0_PE_MASK		0x00000004ul	/* Parity addition function */
#define UART_CR0_PE_NONE		0x00000000ul	/*	None */
#define UART_CR0_PE_ENABLE		0x00000004ul	/*	Enable */
#define UART_CR0_SM_7			0x00000000ul	/*	7 bits */
#define UART_CR0_SM_8			0x00000001ul	/*	8 bits */
#define UART_CR0_SM_9			0x00000002ul	/*	9 bits */
/*------ VALUE in UARTxCR1 Register ------*/
#define UART_CR1_TIL_0			0x00000000ul	/*	Stage 0 */
#define UART_CR1_RIL_1			0x00000100ul	/*	Stage 1 */
#define UART_CR1_INTTXFE_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR1_INTTXWE_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR1_INTTXWE_ENABLE	 	0x00000040ul	/*	Enable */
#define UART_CR1_INTRXFE_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR1_INTRXWE_DISABLE	0x00000000ul	/*	Disable */
#define UART_CR1_INTRXWE_ENABLE		0x00000010ul	/*	Enable */
#define UART_CR1_INTERR_DISABLE		0x00000000ul	/*	Disable */
#define UART_CR1_INTERR_ENABLE		0x00000004ul	/*	Enable */
#define UART_CR1_DMATE_DISABLE		0x00000000ul	/*	Disable */
#define UART_CR1_DMARE_DISABLE		0x00000000ul	/*	Disable */
/*------ VALUE in UARTxCLK Register ------*/
#define UART_CLK_PRSEL_D1		0x00000000ul	/*	1/1 */
/*------ VALUE in UARTxTRANS Register ------*/
#define UART_TRANS_TXE_ENABLE	 	0x00000002ul	/*	Enable */
#define UART_TRANS_RXE_ENABLE	 	0x00000001ul	/*	Enable */
/*------ VALUE in UARTxDR Register ------*/
#define UART_DR_DR_STAT			0x000001FFul	/* Receive data / Transmit data */
/*------ VALUE in UARTxSR Register ------*/
#define UART_SR_TXRUN_STAT		0x00008000ul	/* Transmission in operation flag */
#define UART_SR_TLVL_STAT		0x00000F00ul	/* Transmit FIFO fill level status */
#define UART_SR_RLVL_STAT		0x0000000Ful	/* Receive FIFO Fill level status */
/*------ VALUE in UARTxFIFOCLR Register ------*/
#define UART_FIFOCLR_TFCLR		0x00000002ul	/* Transmit buffer clear */
#define UART_FIFOCLR_RFCLR		0x00000001ul	/* Receive buffer clear */

#define  ENABLE_UART_TE(x)	(*UARTxTRANS(x) |=   UART_TRANS_TXE_ENABLE)
#define DISABLE_UART_TE(x)	(*UARTxTRANS(x) &= ~(UART_TRANS_TXE_ENABLE))
#define  ENABLE_UART_RE(x)	(*UARTxTRANS(x) |=   UART_TRANS_RXE_ENABLE)
#define DISABLE_UART_RE(x)	(*UARTxTRANS(x) &= ~(UART_TRANS_RXE_ENABLE))
#define  ENABLE_TIE_INTRRPT(x)	(*UARTxCR1(x)	|=   UART_CR1_INTTXWE_ENABLE)
#define DISABLE_TIE_INTRRPT(x)	(*UARTxCR1(x)	&= ~(UART_CR1_INTTXWE_ENABLE))
#define  ENABLE_RIE_INTRRPT(x)	(*UARTxCR1(x)	|=   UART_CR1_INTRXWE_ENABLE)
#define DISABLE_RIE_INTRRPT(x)	(*UARTxCR1(x)	&= ~(UART_CR1_INTRXWE_ENABLE))
#define  ENABLE_ERR_INTRRPT(x)	(*UARTxCR1(x)	|=   UART_CR1_INTERR_ENABLE)
#define DISABLE_ERR_INTRRPT(x)	(*UARTxCR1(x)	&= ~(UART_CR1_INTERR_ENABLE))

/*====== DIRECT PARAMETER for UARTx ======*/
#define UART_WORD_LEN_7		 UART_CR0_SM_7
#define UART_WORD_LEN_8		 UART_CR0_SM_8
#define UART_WORD_LEN_9		 UART_CR0_SM_9
#define UART_WORD_LEN_MASK	 UART_CR0_SM_MASK
#define UART_PARITY_NONE	 UART_CR0_PE_NONE
#define UART_PARITY_EVEN	(UART_CR0_PE_ENABLE|UART_CR0_EVEN_EVEN)
#define UART_PARITY_ODD		(UART_CR0_PE_ENABLE|UART_CR0_EVEN_ODD)
#define UART_PARITY_MASK	(UART_CR0_PE_MASK  |UART_CR0_EVEN_MASK)
#define UART_STOP_BIT_1		 UART_CR0_SBLEN_1	/* 1 Stop Bit */
#define UART_STOP_BIT_2		 UART_CR0_SBLEN_2	/* 2 Stop Bit */
#define UART_STOP_BIT_MASK	 UART_CR0_SBLEN_MASK
#define UART_HW_FLOW_DISABLE	(UART_CR0_CTSE_DISABLE|UART_CR0_RTSE_DISABLE)
#define UART_HW_FLOW_CTS	 UART_CR0_CTSE_ENABLE
#define UART_HW_FLOW_RTS	 UART_CR0_RTSE_ENABLE
#define UART_HW_FLOW_ENABLE	(UART_CR0_CTSE_ENABLE|UART_CR0_RTSE_ENABLE)
#define UART_HW_FLOW_MASK	(UART_CR0_CTSE_MASK  |UART_CR0_RTSE_MASK)

#define UART_WORD_LEN		UART_WORD_LEN_8
#define UART_PARITY		UART_PARITY_NONE
#define UART_STOP_BIT		UART_STOP_BIT_1
#define UART_HW_FLOW		UART_HW_FLOW_DISABLE
#define UART_BAUD		(115200)

EXPORT ER initUartGpio( W port );
EXPORT ER initUartRcc( W port );
EXPORT void initUartParam( W port, UW wl, UW par, UW sb, UW baud, UW flow );

LOCAL void sendLine( const UB *buf );
LOCAL UB getChar( UB *buf );
LOCAL void sendChar( const UB *buf );
LOCAL W getTxSizeInFifo( W port );
LOCAL W getRxSizeInFifo( W port );

/*----------------------------------------------------------------------*/
/*	Register Base Address of UARTx					*/
/*	It is used to specify the register address with UART_XXX(x).	*/
/*----------------------------------------------------------------------*/
LOCAL	const	UW	uart_reg_base[MAX_UART_NO] = {
		UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE,
		UART4_BASE, UART5_BASE,
};

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	if( size == 1 ){		/* for tm_putchar */
		sendChar(buf);
	}else if( size >= 2 ){		/* for tm_putstring */
		sendLine(buf);
	}else{
		/* NOP */
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	if( size == 1 ){		/* for tm_getchar */
		getChar( buf );
	}else if( size >= 2 ){		/* for tm_getline with MAX & without CONTROL KEY */
		while( --size > 0 ){
			getChar( buf );
			if( *buf == CHR_ETX ){
				break;	/* CHR_ETX is overwritten with NULL */
			}
			sendChar(buf);	/* ECHO BACK */
			if( *buf == CHR_CR ){
				break;	/* CHR_CR is overwritten with NULL */
			}
			buf++;
		}
		*buf = '\0';		/* == NULL */
	}else{
		/* NOP */
	}
}


EXPORT	void	tm_com_init(void)
{
	initUartRcc(TERM_PORT);		/* Setup System Clock & SUPPLY POWER */
	initUartGpio(TERM_PORT);

	DISABLE_UART_TE(TERM_PORT);
	DISABLE_UART_RE(TERM_PORT);

	initUartParam( TERM_PORT, UART_WORD_LEN, UART_PARITY, UART_STOP_BIT, UART_BAUD, UART_HW_FLOW );

    DISABLE_TIE_INTRRPT(TERM_PORT);
	DISABLE_ERR_INTRRPT(TERM_PORT);
	DISABLE_RIE_INTRRPT(TERM_PORT);

	ENABLE_UART_RE(TERM_PORT);
	ENABLE_UART_TE(TERM_PORT);
}

/*
 *	Get Data Size in Tx FIFO to wait Transmit.
 */
LOCAL W getTxSizeInFifo( W port )
{
	return  (*UARTxSR(port) & UART_SR_TLVL_STAT) >> 8;
				/* [SR]<TLVL>[11:8] */
}

/*
 *	Get Readable Data Size in Rx FIFO.
 *	Call this function after confirming that there is received data.
 */
LOCAL W getRxSizeInFifo( W port )
{
	return *UARTxSR(port) & UART_SR_RLVL_STAT;
				/* [SR]<RLVL>[3:0] */
}

/*----------------------------------------------------------------------*/
/*	Send strings to CONSOLE by polling				*/
/*----------------------------------------------------------------------*/
LOCAL void sendLine( const UB *buf )
{
	while( *buf != '\0' ){
		while( getTxSizeInFifo(TERM_PORT) >= 8 );	/* FIFO is FULL */
		*UARTxDR(TERM_PORT) = *buf++;
	}

	/* Wait for all data to be transmitted. --> Blocking Call */
	while( *UARTxSR(TERM_PORT) & (UART_SR_TXRUN_STAT|UART_SR_TLVL_STAT) );
}


/*----------------------------------------------------------------------*/
/*	Send one Byte to CONSOLE by polling				*/
/*----------------------------------------------------------------------*/
LOCAL void sendChar( const UB *buf )
{
	while( getTxSizeInFifo(TERM_PORT) >= 8 );	/* FIFO is FULL */
	*UARTxDR(TERM_PORT) = *buf;

	/* Wait for all data to be transmitted. --> Blocking Call */
	while( *UARTxSR(TERM_PORT) & (UART_SR_TXRUN_STAT|UART_SR_TLVL_STAT) );
}


/*----------------------------------------------------------------------*/
/*	Receive on Byte from CONSOLE by polling				*/
/*----------------------------------------------------------------------*/
LOCAL UB getChar( UB *buf )
{
	while( getRxSizeInFifo(TERM_PORT) < 1 );	/* FIFO is EMPTY */
	return *buf = (UB)(*UARTxDR(TERM_PORT) & UART_DR_DR_STAT);
}

/*
 *	Set System Clock (RCC) for Enable IP Clock.
 */
EXPORT ER initUartRcc( W port )
{
	switch( port ){
	  case	0:	/* UART0 */
		*CGFSYSENB |= CGFSYSENB_UART0;
		break;
	  case	1:	/* UART1 */
		*CGFSYSENB |= CGFSYSENB_UART1;
		break;
	  case	2:	/* UART2 */
		*CGFSYSENB |= CGFSYSENB_UART2;
		break;
	  case	3:	/* UART3 */
		*CGFSYSENB |= CGFSYSENB_UART3;
		break;
	  case	4:	/* UART4 */
		*CGFSYSENB |= CGFSYSENB_UART4;
		break;
	  case	5:	/* UART5 */
		*CGFSYSENB |= CGFSYSENB_UART5;
		break;
	  default:
		return E_PAR;
	}

	return E_OK;
}

/*
 *	Set GPIO register to select I/O Multi-function.
 */
IMPORT void set_port_func(INT port, INT no, UW func, UW inout );
EXPORT ER initUartGpio( W port )
{
	switch( port ){
	  case	0:	/* UART0 */
		set_port_func( 'A', 1, PA1_UT0TXDA,	PORT_OUT );
		set_port_func( 'A', 2, PA2_UT0RXD,	PORT_IN );
		break;
	  case	1:	/* UART1 */
	  case	2:	/* UART2 */
	  case	3:	/* UART3 */
	  case	4:	/* UART4 */
	  case	5:	/* UART5 */
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
	UB	ken;		/*   --> [UARTxBRD]<ken[23:23]> */
	UB	brk;		/* K --> [UARTxBRD]<brk[21:16]> */
	UH	brn;		/* N --> [UARTxBRD]<brn[15:0]> */
} BRDTBL;
EXPORT void initUartParam( W port, UW wl, UW par, UW sb, UW baud, UW flow )
{
const	BRDTBL	brdTbl[] = {
	{115200, 1, 0x2A, 0x2B  }, {57600, 1, 0x15,   0x56 }, {38400, 1, 0x3F,  0x82 },
	{ 19200, 1, 0x3E, 0x104 }, { 9600, 1, 0x3D,  0x208 }, { 4800, 1, 0x39, 0x410 },
	{  2400, 1, 0x33, 0x820 }, { 1200, 1, 0x25, 0x1040 }, {0,0,0,0}  };
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
	while( (*UARTxSWRST(port) & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );
	*UARTxSWRST(port) = UART_SWRST_SWRST_1ST;
	*UARTxSWRST(port) = UART_SWRST_SWRST_2ND;
	while( (*UARTxSWRST(port) & UART_SWRST_SWRSTF_STAT) == UART_SWRST_SWRSTF_RUN );

	*UARTxFIFOCLR(port) = UART_FIFOCLR_TFCLR | UART_FIFOCLR_RFCLR;

	/*------------------------------*/
	/*	BAUDRATE		*/
	/*------------------------------*/
	*UARTxCLK(port) = UART_CLK_PRSEL_D1;	/* FTx = FT0/1 */
	*UARTxBRD(port) = ((UW)brdTbl[idx].ken << 23)
			| ((UW)brdTbl[idx].brk << 16)
			|  (UW)brdTbl[idx].brn;

	/*------------------------------*/
	/*	PARAMETERS		*/
	/*------------------------------*/
	*UARTxCR0(port) = UART_CR0_HBSEN_DISABLE | UART_CR0_LPB_DISABLE | UART_CR0_NF_NONE
			| UART_CR0_CTSE_DISABLE | UART_CR0_RTSE_DISABLE | UART_CR0_WU_DISABLE
			| UART_CR0_IV_DISABLE | UART_CR0_DIR_LSB
			| sb | par | wl ;

	/*------------------------------*/
	/*	INTERRUPTS		*/
	/*------------------------------*/
	*UARTxCR1(port) = UART_CR1_TIL_0 | UART_CR1_RIL_1
			| UART_CR1_INTTXFE_DISABLE | UART_CR1_INTTXWE_DISABLE
			| UART_CR1_INTRXFE_DISABLE | UART_CR1_INTRXWE_DISABLE
			| UART_CR1_INTERR_DISABLE
			| UART_CR1_DMATE_DISABLE | UART_CR1_DMARE_DISABLE;
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* ADBUN_TMPM3HQF10B */
#endif /* USE_TMONITOR */
