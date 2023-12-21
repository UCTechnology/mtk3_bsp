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
 *    Modified by UC Technology at 2023/11/22
 * 
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */


/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for XMC7200
 */

#ifndef	__DEV_SER_XMC7200_H__
#define	__DEV_SER_XMC7200_H__

#define DEV_SER_UNITNM		(11)	/* Number of devive channels */
#define DEV_SER_UNIT0		(0)	/* SCB0 */
#define DEV_SER_UNIT1		(1)	/* SCB1 */
#define DEV_SER_UNIT2		(2)	/* SCB2 */
#define DEV_SER_UNIT3		(3)	/* SCB3 */
#define DEV_SER_UNIT4		(4)	/* SCB4 */
#define DEV_SER_UNIT5		(5)	/* SCB5 */
#define DEV_SER_UNIT6		(6)	/* SCB6 */
#define DEV_SER_UNIT7		(7)	/* SCB7 */
#define DEV_SER_UNIT8		(8)	/* SCB8 */
#define DEV_SER_UNIT9		(9)	/* SCB9 */
#define DEV_SER_UNIT10		(10)	/* SCB10		*/

/* Base address for each channel */
/*
 * UARTn : n = 0 - 10
 */
#define UART_BASE	(0x40600000ul)
#define UART0_BASE	(0x40600000ul)
#define UART1_BASE	(0x40610000ul)
#define UART2_BASE	(0x40620000ul)
#define UART3_BASE	(0x40630000ul)
#define UART4_BASE	(0x40640000ul)
#define UART5_BASE	(0x40650000ul)
#define UART6_BASE	(0x40660000ul)
#define UART7_BASE	(0x40670000ul)
#define UART8_BASE	(0x40680000ul)
#define UART9_BASE	(0x40690000ul)
#define UART10_BASE	(0x406A0000ul)

/*
 * UART registers
 */
#define SCBxCTRL			(0x0000ul)	/* Generic control register */
#define SCBxUART_CTRL		(0x0040ul)	/* UART control register */
#define SCBxUART_TX_CTRL	(0x0044ul)	/* UART transmitter control register */
#define SCBxUART_RX_CTRL	(0x0048ul)	/* UART receiver control register */
#define SCBxUART_RX_STATUS	(0x004Cul)	/* UART receiver status register */
#define SCBxUART_FLOW_CTRL	(0x0050ul)	/* UART flow control register */
#define SCBxTX_CTRL			(0x00200ul)	/* transmitter control register */
#define SCBxTX_FIFO_CTRL	(0x00204ul)	/* transmitter FIFO control register */
#define SCBxTX_FIFO_STATUS	(0x00208ul)	/* transmitter FIFO status register */
#define SCBxTX_FIFO_WR		(0x00240ul)	/* transmitter FIFO write register */
#define SCBxRX_CTRL			(0x00300ul)	/* Receiver control register */
#define SCBxRX_FIFO_CTRL	(0x00304ul)	/* Receiver FIFO control register */
#define SCBxRX_FIFO_STATUS	(0x00308ul)	/* Receiver FIFO status register */
#define SCBxRX_FIFO_RD		(0x00340ul)	/* Receiver FIFO read register */
#define SCBxINTR_CAUSE		(0x00E00ul)	/* Active clocked interrupt signal register */
#define SCBxINTR_TX			(0x00F80ul)	/* Transmitter interrupt request register */
#define SCBxINTR_TX_MASK	(0x00F88ul)	/* Transmitter interrupt mask register */
#define SCBxINTR_RX			(0x00FC0ul)	/* Receiver interrupt request register */
#define SCBxINTR_RX_MASK	(0x00FC8ul)	/* Receiver interrupt mask register */

/* CTRL */
#define CTRL_OVS(x)	((x) << 0)
#define CTRL_WIDTH_BYTE	(0U << 14)	/* 8-bit FIFO data elements. */
#define CTRL_WIDTH_HFWORD	(1U << 14)	/* 16-bit FIFO data elements. */
#define CTRL_WIDTH_WORD	(2U << 14)	/* 32-bit FIFO data elements. */
#define CTRL_MODE_UART	(2U << 24)	/* UART mode. */
#define CTRL_ENABLED	(1U << 31)	/* IP enabled */
#define CTRL_WIDTH_MASK	(0x0000C000)

/* UART_RX_CTRL */
#define UART_RX_CTRL_STOP_BIT(x)	((x) << 0)	/* stop bits. */
												/* STOP_BITS + 1 is the duration of the stopperiod in terms of halve bit periods. */
#define UART_RX_CTRL_STOP_BIT_MASK	(0x00000007)
#define UART_RX_CTRL_EVEN_PARITY	(0U << 4)	/* even parity */
#define UART_RX_CTRL_ODD_PARITY		(1U << 4)	/* odd parity */
#define UART_RX_CTRL_PARITY_DISABLE	(0U << 5)	/* Parity checking disabled */
#define UART_RX_CTRL_PARITY_ENABLE	(1U << 5)	/* Parity checking enabled */
#define UART_RX_CTRL_PARITY_MASK	(0x00000030)
#define UART_RX_CTRL_BREAK_WIDTH(x)	((x) << 16)	/* Break width.  */

/* RX_CTRL */
#define RX_CTRL_DATA_WIDTH(x)	((x) << 0)	/* Dataframe width. DATA_WIDTH + 1. the valid range is [3, 8]. */
#define RX_CTRL_LSB_FIRST		(0U << 7)	/* LSB first */
#define RX_CTRL_MSB_FIRST		(1U << 7)	/* MSB first */

/* TX_CTRL */
#define TX_CTRL_DATA_WIDTH(x)	((x) << 0)	/* Dataframe width. DATA_WIDTH + 1. the valid range is [3, 8]. */
#define TX_CTRL_LSB_FIRST		(0U << 7)	/* LSB first */
#define TX_CTRL_MSB_FIRST		(1U << 7)	/* MSB first */

/* RX_FIFO_CTRL */
#define RX_FIFO_CTRL_LEVEL(x)	((x) << 0)	/* Trigger level. */
#define RX_FIFO_CTRL_CLEAR		(1U << 16)	/* reciver FIFO and reciver shift register are cleared/invalidated. */

#define SCB_RX_FIFO_STATUS_USED	(0x000000FF)

/* TX_FIFO_CTRL */
#define TX_FIFO_CTRL_LEVEL(x)	((x) << 0)	/* Trigger level. */
#define TX_FIFO_CTRL_CLEAR		(1U << 16)	/* transmitter FIFO and transmitter shift register are cleared/invalidated. */

#define SCB_TX_FIFO_STATUS_USED	(0x000001FF)

/* UART_FLOW_CTRL */
#define UART_FLOW_CTRL_TRIGGER_LEVEL(x)	((x) << 0)	/* Trigger level.  */
#define UART_FLOW_CTRL_TRIGGER_LEVEL_MASK	(0x000000FF)
#define UART_FLOW_CTRL_RTS_LOW	(0U << 16)	/* Polarity of the RTS output signal 'uart_rts_out' 0:Low Active */
#define UART_FLOW_CTRL_RTS_HIGH	(1U << 16)	/* Polarity of the RTS output signal 'uart_rts_out' 1:High Active */
#define UART_FLOW_CTRL_CTS_LOW	(0U << 24)	/* Polarity of the CTS input signal 'uart_cts_in' 0:Low Active */
#define UART_FLOW_CTRL_CTS_HIGH	(1U << 24)	/* Polarity of the RTS output signal 'uart_rts_out' 1:High Active */
#define UART_FLOW_CTRL_CTS_DISABLE	(0U << 25)	/* Disable use of CTS input signal 'uart_cts_in' by the UART transmitter: */
#define UART_FLOW_CTRL_CTS_ENABLE	(1U << 25)	/* Enable use of CTS input signal 'uart_cts_in' by the UART transmitter: */

/* INTR */
#define SCB_INTR_TX_TRIGGER	(1U << 0)
#define SCB_INTR_TX_NOT_FULL	(1U << 1)
#define SCB_INTR_TX_EMPTY	(1U << 4)
#define SCB_INTR_TX_OVERFLOW	(1U << 5)
#define SCB_INTR_TX_UNDERFLOW	(1U << 6)
#define SCB_INTR_TX_BLOCKED	(1U << 7)
#define SCB_INTR_TX_NACK	(1U << 8)	/* SmartCard mode */
#define SCB_INTR_TX_DONE	(1U << 9)
#define SCB_INTR_TX_ARB_LOST	(1U << 10)	/* LIN or SmartCard mode */

#define SCB_INTR_RX_TRIGGER	(1U << 0)
#define SCB_INTR_RX_NOT_EMPTY	(1U << 2)
#define SCB_INTR_RX_FULL	(1U << 3)
#define SCB_INTR_RX_OVERFLOW	(1U << 5)
#define SCB_INTR_RX_UNDERFLOW	(1U << 6)
#define SCB_INTR_RX_BLOCKED	(1U << 7)
#define SCB_INTR_RX_FRAME_ERROR	(1U << 8)
#define SCB_INTR_RX_PARITY_ERROR	(1U << 9)
#define SCB_INTR_RX_BAUD_DETECT	(1U << 10)
#define SCB_INTR_RX_BREAK_DETECT	(1U << 11)

#define SCB_INTR_CAUSE_TX	(1U << 2)	/* Transmitter interrupt active ('interrupt_tx') */
#define SCB_INTR_CAUSE_RX	(1U << 3)	/* Receiver interrupt active ('interrupt_rx') */

#define RX_INTR_ERR_MASK	(SCB_INTR_RX_OVERFLOW | SCB_INTR_RX_FRAME_ERROR | SCB_INTR_RX_PARITY_ERROR | SCB_INTR_RX_BREAK_DETECT)

#define SCB_TX_INTR_ALL_MASK	(SCB_INTR_TX_TRIGGER | SCB_INTR_TX_NOT_FULL | SCB_INTR_TX_EMPTY | SCB_INTR_TX_OVERFLOW | SCB_INTR_TX_UNDERFLOW | \
								SCB_INTR_TX_BLOCKED | SCB_INTR_TX_NACK | SCB_INTR_TX_DONE | SCB_INTR_TX_ARB_LOST)

#define SCB_RX_INTR_ALL_MASK	(SCB_INTR_RX_TRIGGER | SCB_INTR_RX_NOT_EMPTY | SCB_INTR_RX_FULL | SCB_INTR_RX_OVERFLOW | SCB_INTR_RX_UNDERFLOW | \
								SCB_INTR_RX_BLOCKED | SCB_INTR_RX_FRAME_ERROR | SCB_INTR_RX_PARITY_ERROR | SCB_INTR_RX_BAUD_DETECT | SCB_INTR_RX_BREAK_DETECT)

/* UART interrupt number */
#define INTNO_UART0		(18)	/* SCB0 */
#define INTNO_UART1		(113)	/* SCB1 */
#define INTNO_UART2		(114)	/* SCB2 */
#define INTNO_UART3		(115)	/* SCB3 */
#define INTNO_UART4		(116)	/* SCB4 */
#define INTNO_UART5		(117)	/* SCB5 */
#define INTNO_UART6		(118)	/* SCB6 */
#define INTNO_UART7		(119)	/* SCB7 */
#define INTNO_UART8		(120)	/* SCB8 */
#define INTNO_UART9		(121)	/* SCB9 */
#define INTNO_UART10	(122)	/* SCB10 */

#endif		/* __DEV_SER_XMC7200_H__ */
