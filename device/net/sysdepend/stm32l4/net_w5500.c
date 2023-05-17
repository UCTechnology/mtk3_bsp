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
 *	net_spi_w5500.c
 *	Ethernet function for W5500 with SPI
 *		for STM32L4
 */
#include <stm32l4xx_hal.h>
#include <stm32l4xx_ll_spi.h>
#include <stm32l4xx_ll_dma.h>
#include "net_w5500.h"

#define W5500_ACCESS_TMOUT			(500)

#define initSS()	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define setSS()		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define resetSS()	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)

/*
 * SPI handler For w5500;
 */
static SPI_HandleTypeDef hspi = {0};
/*
 * DMA handler For w5500;
 */
static DMA_HandleTypeDef spidma_tx = {0};
static DMA_HandleTypeDef spidma_rx = {0};

void dma_tx_int_handler(int intno)
{
	HAL_DMA_IRQHandler(&spidma_tx);
}

void dma_rx_int_handler(int intno)
{
	HAL_DMA_IRQHandler(&spidma_rx);
}

/*
 * Functions for STM32CubeL4 HAL library SPI module.
 *  - HAL_SPI_MspInit
 *  - HAL_SPI_MspDeInit
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef igpio = {0};
	
	// Initialize GPIO
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	// SPI1_SCK = PA5
	igpio.Pin        = GPIO_PIN_5;
	igpio.Mode       = GPIO_MODE_AF_PP;
	igpio.Pull       = GPIO_PULLUP;
	igpio.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
	igpio.Alternate  = GPIO_AF5_SPI1;
	HAL_GPIO_Init(GPIOA, &igpio);
	
	// SPI1_MISO = PA6
	igpio.Pin        = GPIO_PIN_6;
	igpio.Pull       = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &igpio);
	
	// SPI1_MOSI = PA7
	igpio.Pin        = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOA, &igpio);
	
	// CS = PB6
	igpio.Pin        = GPIO_PIN_6;
	igpio.Mode       = GPIO_MODE_OUTPUT_OD;
	igpio.Pull       = GPIO_NOPULL;
	igpio.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &igpio);
	
	// Enable SPI1 module.
	__HAL_RCC_SPI1_CLK_ENABLE();

#if SPI_USE_DMA	
	// Enable DMA1
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	// Configure the DMA handler for TX process
    spidma_tx.Init.Request             = DMA_REQUEST_1;
    spidma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    spidma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
    spidma_tx.Init.MemInc              = DMA_MINC_ENABLE;
    spidma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    spidma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    spidma_tx.Init.Mode                = DMA_NORMAL;
    spidma_tx.Init.Priority            = DMA_PRIORITY_HIGH;
    spidma_tx.Instance                 = DMA1_Channel3;

    HAL_DMA_Init(&spidma_tx);
	
    /* Associate the initialized DMA handle to the SPI handle */
    __HAL_LINKDMA(hspi, hdmatx, spidma_tx);
	
    // Configure the DMA handler for RX process
    spidma_rx.Init.Request             = DMA_REQUEST_1;
    spidma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    spidma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
    spidma_rx.Init.MemInc              = DMA_MINC_ENABLE;
    spidma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    spidma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    spidma_rx.Init.Mode                = DMA_NORMAL;
    spidma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
    spidma_rx.Instance                 = DMA1_Channel2;

    HAL_DMA_Init(&spidma_rx);
	
    // Associate the initialized DMA handle to the SPI handle
    __HAL_LINKDMA(hspi, hdmarx, spidma_rx);
	
#endif // SPI_USE_DMA
	
	// Set CS(PB6) to High.
	initSS();

	return;
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	UNUSED(hspi);
	
	// Deselect W5500.
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	
	// Disable SPI1 module.
	__HAL_RCC_SPI1_CLK_DISABLE();
	
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	dma_notify_tx_end();
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	dma_notify_rx_end();
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	dma_notify_error();
}

static HAL_StatusTypeDef w5500_read(uint16_t addr, uint8_t block, uint8_t *val, uint16_t n)
{
	uint8_t cmd[3];
	HAL_StatusTypeDef err;
	
	cmd[0] = (uint8_t) ((addr >> 8) & 0x00FF);
	cmd[1] = (uint8_t) (addr & 0x00FF);
	cmd[2] = (block & 0x1F) << 3;			// RWB = 0, OM = 00 N-Bytes Data Phase
	
	setSS();
	
	err = HAL_SPI_Transmit(&hspi, cmd, 3, W5500_ACCESS_TMOUT);
	if (err == HAL_OK) {
		err = HAL_SPI_Receive(&hspi, val, n, W5500_ACCESS_TMOUT);
	}
	
	resetSS();
	
	return err;
}

static HAL_StatusTypeDef w5500_write(uint16_t addr, uint8_t block, uint8_t *val, uint16_t n)
{
	uint8_t cmd[3];
	HAL_StatusTypeDef err;
	
	cmd[0] = (uint8_t) ((addr >> 8) & 0x00FF);
	cmd[1] = (uint8_t) (addr & 0x00FF);
	cmd[2] = ((block & 0x1F) << 3) | (1 << 2);	// RWB = 1, OM = 00 N-Bytes Data Phase
	
	setSS();
	
	err = HAL_SPI_Transmit(&hspi, cmd, 3, W5500_ACCESS_TMOUT);
	if (err == HAL_OK) {
		err = HAL_SPI_Transmit(&hspi, val, n, W5500_ACCESS_TMOUT);
	}
	
	resetSS();
	
	return err;
}

int w5500_send(const void *buf, unsigned int len) 
{
	uint8_t val[3];
	uint16_t addr;
	int err;
	
	err = w5500_read(W5500_Sn_TX_WR0, W5500_BLK_SOCK_REG(0), val, 2);
	if (err != HAL_OK) {
		return err;
	}
	
	addr = ((uint16_t) val[0] << 8) | val[1];
	
	// tm_printf("SEND: Sn_TX_WR = 0x%04X, size = %d\n", addr, len);
	
#if SPI_USE_DMA
	val[2] = ((W5500_BLK_SOCK_TXBUF(0) & 0x1F) << 3) | (1 << 2);
	
	setSS();
	
	err = HAL_SPI_Transmit(&hspi, val, 3, W5500_ACCESS_TMOUT);
	if (err == HAL_OK) {
		err = HAL_SPI_Transmit_DMA(&hspi, (uint8_t *) buf, len);
		
		if (err == HAL_OK) {
			if (dma_wait_tx() != 0) {
				err = HAL_TIMEOUT;
			}
		}
	}
	
	resetSS();
#else
	err = w5500_write(addr, W5500_BLK_SOCK_TXBUF(0), (uint8_t *) buf, len);
#endif
	
	// Update Sn_WR && Do send.
	if (err == HAL_OK) {
		addr += len;
		val[0] = (uint8_t) (addr >> 8);
		val[1] = (uint8_t) (addr & 0x00FF);
		err = w5500_write(W5500_Sn_TX_WR0, W5500_BLK_SOCK_REG(0), val, 2);
		if (err == HAL_OK) {
			val[0] = W5500_Sn_CR_SEND;
			err = w5500_write(W5500_Sn_CR, W5500_BLK_SOCK_REG(0), val, 1);
		}
	}
	
	return err;
}

int w5500_recv(void *buf, int bufsz, unsigned short *len)
{
	uint8_t val[3];
	uint16_t addr, size, last;
	int err;
	
	err = w5500_read(W5500_Sn_RX_WR0, W5500_BLK_SOCK_REG(0), val, 2);
	if (err != HAL_OK) {
		return -1;
	}
	
	// Sn_RX_WR needs double check.
	do {
		last = ((uint16_t) val[0] << 8) | val[1];
		err = w5500_read(W5500_Sn_RX_WR0, W5500_BLK_SOCK_REG(0), val, 2);
		if (err != HAL_OK) {
			return -1;
		}
	} while(val[0] != ((uint8_t) (last >> 8)) || val[1] != ((uint8_t) (last & 0x00FF)));
	
	err = w5500_read(W5500_Sn_RX_RD0, W5500_BLK_SOCK_REG(0), val, 2);
	if (err != HAL_OK) {
		return -1;
	}
	
	addr = ((uint16_t) val[0] << 8) | val[1];
	
	// Trick: Sn_IR may report wrong RECV status.
	if (addr == last) {
		return -1;
	}
	
	err = w5500_read(addr, W5500_BLK_SOCK_RXBUF(0), val, 2);
	if (err != HAL_OK) {
		return -1;
	}
	
	size = (((uint16_t) val[0] << 8) | val[1]) - 2; // The original size include the first two bytes of length.
	if (size > bufsz) {
		return -1;
	}
	addr += 2;

	// tm_printf("RECV: Sn_RX_RD = 0x%04X, size = %d, Sn_RX_WR = 0x%04X\n", addr, size, last);

#if SPI_USE_DMA
	val[0] = (uint8_t) (addr >> 8);
	val[1] = (uint8_t) (addr & 0x00FF);
	val[2] = ((W5500_BLK_SOCK_RXBUF(0) & 0x1F) << 3);
	
	setSS();
		
	err = HAL_SPI_Transmit(&hspi, val, 3, W5500_ACCESS_TMOUT);
	if (err == HAL_OK) {
		err = HAL_SPI_Receive_DMA(&hspi, (uint8_t *) buf, size);
		
		if (err == HAL_OK) {
			if (dma_wait_rx() != 0) {
				err = HAL_TIMEOUT;
			}
		}
	}
	
	resetSS();
#else
	err = w5500_read(addr, W5500_BLK_SOCK_RXBUF(0), buf, size);
#endif
	
	if (err == HAL_OK) {
		*len = size;
		addr += size;
		val[0] = (uint8_t) (addr >> 8);
		val[1] = (uint8_t) (addr & 0x00FF);
		err = w5500_write(W5500_Sn_RX_RD0, W5500_BLK_SOCK_REG(0), val, 2);
		if (err == HAL_OK) {
			val[0] = W5500_Sn_CR_RECV;
			err = w5500_write(W5500_Sn_CR, W5500_BLK_SOCK_REG(0), val, 1);
		}
	}

	return err;
}

int w5500_get_status(unsigned char *status)
{
	return w5500_read(W5500_Sn_IR, W5500_BLK_SOCK_REG(0), status, 1);
}

int w5500_check_link(void)
{
	uint8_t val;
	int err;
	
	err = w5500_read(W5500_COMMON_PHYCFGR, W5500_BLK_COMMON, &val, 1);
	if (err != HAL_OK || !(val & W5500_COMMON_PHYCFGR_LNK)) {
		return 0;
	}
	
	return 1;
}

void w5500_set_multicast(int enable)
{
	int err;
	uint8_t val;
	
	err = w5500_read(W5500_Sn_MR, W5500_BLK_SOCK_REG(0), &val, 1);
	if (err == HAL_OK) {
		if (enable) {
			val &= ~W5500_Sn_MR_MFEN;
		}
		else {
			val |= W5500_Sn_MR_MFEN;
		}
		w5500_write(W5500_Sn_MR, W5500_BLK_SOCK_REG(0), &val, 1);
	}
}

int w5500_init(const unsigned char *macaddr, int promiscuous_mode)
{
	HAL_StatusTypeDef err;
	uint8_t tmp_val;
	int i;
	
	// Update SystemCoreClock.
	HAL_RCC_GetHCLKFreq();
	
	/* SPI Config */
    hspi.Instance = SPI1;
	
    /* SPI baudrate is set to 20 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 80/4 = 20 MHz)
     to verify the constraint:
        - W5500 SPI interface max baudrate is 80/33.3MHz for write/read,
          but the document said 'high speed may be distorted becase of the 
		  circuit crosstalk and the length of the signal line.' too, so 
		  20Mhz is a safty choice.
     */
    hspi.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_4;
    hspi.Init.Direction          = SPI_DIRECTION_2LINES;
    hspi.Init.CLKPhase           = SPI_PHASE_2EDGE;
    hspi.Init.CLKPolarity        = SPI_POLARITY_HIGH;
    hspi.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
    hspi.Init.CRCPolynomial      = 7;
    hspi.Init.CRCLength          = SPI_CRC_LENGTH_DATASIZE;
    hspi.Init.DataSize           = SPI_DATASIZE_8BIT;
    hspi.Init.FirstBit           = SPI_FIRSTBIT_MSB;
    hspi.Init.NSS                = SPI_NSS_SOFT;
    hspi.Init.NSSPMode           = SPI_NSS_PULSE_DISABLE;
    hspi.Init.TIMode             = SPI_TIMODE_DISABLE;
    hspi.Init.Mode               = SPI_MODE_MASTER;

    err = HAL_SPI_Init(&hspi);
    if (err != HAL_OK) {
    	return err;
    }

	// Trick: Wait for W5500 
	do {
		err = w5500_read(W5500_COMMON_VERSIONR, W5500_BLK_COMMON, &tmp_val, 1);
		if (err != HAL_OK) {
			return err;
		}
	} while(tmp_val != W5500_COMMON_VERSIONR_VISION);
	
	// --- Initialze W5500. ---
	// Config MAC address.
	err = w5500_write(W5500_COMMON_SHAR0, W5500_BLK_COMMON, (uint8_t *) macaddr, 6);
	if (err != HAL_OK) {
		return err;
	}

	// Set socket0 MR (MAC RAW mode)
	tmp_val = W5500_Sn_MR_MACRAW;
	if (!promiscuous_mode) {
		tmp_val |= W5500_Sn_MR_MFEN;
	}
	err = w5500_write(W5500_Sn_MR, W5500_BLK_SOCK_REG(0), &tmp_val, 1);
	if (err != HAL_OK) {
		return err;
	}

	// Set socket1 ~ 7　TX, RXBUF_SIZE = 0
	tmp_val = 0;
	for (i = 1; i <= 7; i++) {
		err = w5500_write(W5500_Sn_RXBUF_SIZE, W5500_BLK_SOCK_REG(i), &tmp_val, 1);
		if (err != HAL_OK) {
			return err;
		}
		
		err = w5500_write(W5500_Sn_TXBUF_SIZE, W5500_BLK_SOCK_REG(i), &tmp_val, 1);
		if (err != HAL_OK) {
			return err;
		}
	}

	// socket0 use all 16KB buffer.
	tmp_val = 16;
	err = w5500_write(W5500_Sn_RXBUF_SIZE, W5500_BLK_SOCK_REG(0), &tmp_val, 1);
	if (err != HAL_OK) {
		return err;
	}
	
	err = w5500_write(W5500_Sn_TXBUF_SIZE, W5500_BLK_SOCK_REG(0), &tmp_val, 1);
	if (err != HAL_OK) {
		return err;
	}

	// Start socket 0
	tmp_val = W5500_Sn_CR_OPEN;
	err = w5500_write(W5500_Sn_CR, W5500_BLK_SOCK_REG(0), &tmp_val, 1);
	if (err != HAL_OK) {
		return err;
	}

	do{
		err = w5500_read(W5500_Sn_SR, W5500_BLK_SOCK_REG(0), &tmp_val, 1);
		if (err != HAL_OK) {
			return err;
		}
	} while(tmp_val != W5500_Sn_SR_SOCK_MACRAW);

	return 0;
}

