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
 *	net_spi_w5500.h 
 *	Ethernet function for W5500 with SPI
 *		for STM32L4
 */
#ifndef __NET_SPI_W5500_STM32L4_H__
#define __NET_SPI_W5500_STM32L4_H__

#define W5500_BLK_COMMON		(0x00)
#define W5500_BLK_SOCK_REG(x)	(((x) << 2) | 1)
#define W5500_BLK_SOCK_TXBUF(x)	(((x) << 2) | 2)
#define W5500_BLK_SOCK_RXBUF(x)	(((x) << 2) | 3)

#define W5500_COMMON_MR			(0x0000)
#define W5500_COMMON_SHAR0		(0x0009)
#define W5500_COMMON_IR			(0x0015)
#define W5500_COMMON_IMR		(0x0016)			
#define W5500_COMMON_SIR		(0x0017)
#define W5500_COMMON_SIMR		(0x0018)
#define W5500_COMMON_PHYCFGR	(0x002E)
#define W5500_COMMON_VERSIONR	(0x0039)

#define W5500_COMMON_PHYCFGR_LNK	(0x01)

#define W5500_COMMON_VERSIONR_VISION	(0x04)

#define W5500_Sn_MR				(0x0000)
#define W5500_Sn_CR				(0x0001)
#define W5500_Sn_IR				(0x0002)
#define W5500_Sn_SR				(0x0003)
#define W5500_Sn_MSSR0			(0x0012)
#define W5500_Sn_RXBUF_SIZE		(0x001E)
#define W5500_Sn_TXBUF_SIZE		(0x001F)
#define W5500_Sn_TX_FSR0		(0x0020)
#define W5500_Sn_TX_RD0			(0x0022)
#define W5500_Sn_TX_WR0			(0x0024)
#define W5500_Sn_RX_RSR0		(0x0026)
#define W5500_Sn_RX_RD0			(0x0028)
#define W5500_Sn_RX_WR0			(0x002A)
#define W5500_Sn_RX_IMR			(0x002C)

#define W5500_Sn_MR_MACRAW		(0x04)
#define W5500_Sn_MR_MFEN		(0x80)

#define W5500_Sn_CR_OPEN		(0x01)
#define W5500_Sn_CR_SEND		(0x20)
#define W5500_Sn_CR_RECV		(0x40)

#define W5500_Sn_IR_SEND_OK		(0x10)
#define W5500_Sn_IR_RECV		(0x04)

#define W5500_Sn_SR_SOCK_MACRAW	(0x42)

#define SPI_USE_DMA				(1)

#define INTNO_DMA1_CHANNEL2		(12)
#define INTNO_DMA1_CHANNEL3		(13)

/* The below functions must be implemented in the net_ether.c because they used utkernel APIs. */
void dma_notify_tx_end(void);
void dma_notify_rx_end(void);
void dma_notify_error(void);
int dma_wait_tx(void);
int dma_wait_rx(void);

void dma_tx_int_handler(int intno);
void dma_rx_int_handler(int intno);
int w5500_init(const unsigned char *macaddr, int promiscuous_mode);
int w5500_send(const void *buf, unsigned int len);
int w5500_recv(void *buf, int bufsz, unsigned short *len);

int w5500_get_status(unsigned char *status);
int w5500_check_link(void);
void w5500_set_multicast(int enable);


#endif // __NET_SPI_W5500_STM32L4_H__
