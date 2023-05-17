/*
 *----------------------------------------------------------------------
 *    UCT micro T-Kernel Version 2.00.43
 *
 *    Copyright (c) 2013-2019 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Released by UC Technology (http://www.uctec.com/) at 2013/06/10.
 *    Modified by UC Technology at 2015/06/04.
 *    Modified by UC Technology at 2016/10/24.
 *    Modified by UC Technology at 2018/09/13.
 *    Modified by UC Technology at 2019/10/15.
 *
 *----------------------------------------------------------------------
 */

#ifndef __NET_ETHER_H__
#define __NET_ETHER_H__

#include <tk/tkernel.h>
/* !Trick: uTK3.0's /include/sys/cpu/stm32l4/sysdef.h defines 
 * same macros as the STMCubeL4 library, undefine them here. 
 */
/*
#undef	EXTI_BASE
#undef	FLASH_ACR_DCEN
#undef	FLASH_ACR_DCRST
#undef	FLASH_ACR_ICEN
#undef	FLASH_ACR_ICRST
#undef	FLASH_ACR_LATENCY
#undef	FLASH_ACR_PRFTEN
#undef	FLASH_ACR_RUN_PD
#undef	FLASH_ACR_SLEEP_PD
#undef	FLASH_BASE
#undef	GPIOA_BASE
#undef	GPIOB_BASE
#undef	GPIOC_BASE
#undef	GPIOD_BASE
#undef	GPIOE_BASE
#undef	GPIOF_BASE
#undef	GPIOG_BASE
#undef	GPIOH_BASE
#undef	PWR_BASE
#undef	PWR_CR1_DBP
#undef	PWR_CR1_LPMS
#undef	PWR_CR1_LPMS_SHUTDOWN
#undef	PWR_CR1_LPMS_STANDBY
#undef	PWR_CR1_LPMS_STOP0
#undef	PWR_CR1_LPMS_STOP1
#undef	PWR_CR1_LPMS_STOP2
#undef	PWR_CR1_LPR
#undef	PWR_CR1_VOS
#undef	RCC_AHB2ENR_ADCEN
#undef	RCC_AHB2ENR_ADCEN
#undef	RCC_AHB2ENR_GPIOAEN
#undef	RCC_AHB2ENR_GPIOAEN
#undef	RCC_AHB2ENR_GPIOBEN
#undef	RCC_AHB2ENR_GPIOBEN
#undef	RCC_AHB2ENR_GPIOCEN
#undef	RCC_AHB2ENR_GPIOCEN
#undef	RCC_AHB2ENR_GPIODEN
#undef	RCC_AHB2ENR_GPIODEN
#undef	RCC_AHB2ENR_GPIOEEN
#undef	RCC_AHB2ENR_GPIOEEN
#undef	RCC_AHB2ENR_GPIOFEN
#undef	RCC_AHB2ENR_GPIOFEN
#undef	RCC_AHB2ENR_GPIOGEN
#undef	RCC_AHB2ENR_GPIOGEN
#undef	RCC_AHB2ENR_GPIOHEN
#undef	RCC_AHB2ENR_GPIOHEN
#undef	RCC_APB1ENR1_CAN1EN
#undef	RCC_APB1ENR1_DAC1EN
#undef	RCC_APB1ENR1_I2C1EN
#undef	RCC_APB1ENR1_I2C2EN
#undef	RCC_APB1ENR1_I2C3EN
#undef	RCC_APB1ENR1_LPTIM1EN
#undef	RCC_APB1ENR1_OPAMPEN
#undef	RCC_APB1ENR1_PWREN
#undef	RCC_APB1ENR1_SPI2EN
#undef	RCC_APB1ENR1_SPI3EN
#undef	RCC_APB1ENR1_TIM2EN
#undef	RCC_APB1ENR1_TIM2EN
#undef	RCC_APB1ENR1_TIM3EN
#undef	RCC_APB1ENR1_TIM3EN
#undef	RCC_APB1ENR1_TIM4EN
#undef	RCC_APB1ENR1_TIM4EN
#undef	RCC_APB1ENR1_TIM5EN
#undef	RCC_APB1ENR1_TIM5EN
#undef	RCC_APB1ENR1_TIM6EN
#undef	RCC_APB1ENR1_TIM6EN
#undef	RCC_APB1ENR1_TIM7EN
#undef	RCC_APB1ENR1_TIM7EN
#undef	RCC_APB1ENR1_UART4EN
#undef	RCC_APB1ENR1_UART5EN
#undef	RCC_APB1ENR1_USART2EN
#undef	RCC_APB1ENR1_USART3EN
#undef	RCC_APB1ENR1_WWDGEN
#undef	RCC_APB1ENR2_LPTIM2EN
#undef	RCC_APB1ENR2_LPUART1EN
#undef	RCC_APB1ENR2_SWPMI1EN
#undef	RCC_APB2ENR_DFSDM1EN
#undef	RCC_APB2ENR_FWEN
#undef	RCC_APB2ENR_SAI1EN
#undef	RCC_APB2ENR_SAI2EN
#undef	RCC_APB2ENR_SDMMC1EN
#undef	RCC_APB2ENR_SPI1EN
#undef	RCC_APB2ENR_SYSCFGEN
#undef	RCC_APB2ENR_TIM15EN
#undef	RCC_APB2ENR_TIM16EN
#undef	RCC_APB2ENR_TIM17EN
#undef	RCC_APB2ENR_TIM1EN
#undef	RCC_APB2ENR_TIM8EN
#undef	RCC_APB2ENR_USART1EN
#undef	RCC_BASE
#undef	RCC_CCIPR_ADCSEL
#undef	RCC_CCIPR_CLK48SEL
#undef	RCC_CCIPR_DFSDM1SEL
#undef	RCC_CCIPR_LPUART1SEL
#undef	RCC_CCIPR_SWPMI1SEL
#undef	RCC_CFGR_HPRE
#undef	RCC_CFGR_MCOPRE
#undef	RCC_CFGR_MCOSEL
#undef	RCC_CFGR_PPRE1
#undef	RCC_CFGR_PPRE2
#undef	RCC_CFGR_STOPWUCK
#undef	RCC_CFGR_SW
#undef	RCC_CFGR_SW_HSE
#undef	RCC_CFGR_SW_MSI
#undef	RCC_CFGR_SW_PLL
#undef	RCC_CFGR_SWS
#undef	RCC_CFGR_SWS_HSE
#undef	RCC_CFGR_SWS_MSI
#undef	RCC_CFGR_SWS_PLL
#undef	RCC_CR_CSSON
#undef	RCC_CR_HSEBYP
#undef	RCC_CR_HSEON
#undef	RCC_CR_HSERDY
#undef	RCC_CR_HSIASFS
#undef	RCC_CR_HSIKERON
#undef	RCC_CR_HSION
#undef	RCC_CR_HSIRDY
#undef	RCC_CR_MSION
#undef	RCC_CR_MSIPLLEN
#undef	RCC_CR_MSIRANGE
#undef	RCC_CR_MSIRDY
#undef	RCC_CR_MSIRGSEL
#undef	RCC_CR_PLLON
#undef	RCC_CR_PLLRDY
#undef	RCC_CR_PLLSAI1ON
#undef	RCC_CR_PLLSAI1RDY
#undef	RCC_CR_PLLSAI2ON
#undef	RCC_CR_PLLSAI2RDY
#undef	RCC_PLLCFGR_PLLM
#undef	RCC_PLLCFGR_PLLM
#undef	RCC_PLLCFGR_PLLN
#undef	RCC_PLLCFGR_PLLN
#undef	RCC_PLLCFGR_PLLP
#undef	RCC_PLLCFGR_PLLP
#undef	RCC_PLLCFGR_PLLPEN
#undef	RCC_PLLCFGR_PLLPEN
#undef	RCC_PLLCFGR_PLLQ
#undef	RCC_PLLCFGR_PLLQ
#undef	RCC_PLLCFGR_PLLQEN
#undef	RCC_PLLCFGR_PLLQEN
#undef	RCC_PLLCFGR_PLLR
#undef	RCC_PLLCFGR_PLLR
#undef	RCC_PLLCFGR_PLLREN
#undef	RCC_PLLCFGR_PLLREN
#undef	RCC_PLLCFGR_PLLSRC
#undef	RCC_PLLCFGR_PLLSRC_HSE
#undef	RCC_PLLCFGR_PLLSRC_HSE
#undef	RCC_PLLCFGR_PLLSRC_HSI
#undef	RCC_PLLCFGR_PLLSRC_MSI
#undef	SYSCFG_BASE
#undef	TIM2_BASE
#undef	TIM3_BASE
#undef	TIM4_BASE
#undef	TIM5_BASE
#undef	TIM6_BASE
#undef	TIM7_BASE

#include <stm32l4xx_hal.h>
#include <stm32l4xx_ll_spi.h>
#include <stm32l4xx_ll_dma.h>
*/

#include "../../../include/dev_net.h"

/*
 * The channel number of ethernet controller.
 */
#define ETHERC_MAX_CHANNEL         (1)

/*
 * The receive task priority.
 */
#define ETHER_RECV_TASK_PRIORITY   (10)
/*
 * The stack size of the receive task.
 */
#define ETHER_RECV_TASK_STASK_SIZE (2048)

#define ETHER_SPI_DMA_TMOUT			(1000)

#define ETHER_POLLING_INTERVAL		(10)

/*
 * Flag patterns
 */
#define ETHER_FLGPTN_RECV		(0x01)
#define ETHER_FLGPTN_SEND		(0x02)
#define ETHER_FLGPTN_ERROR		(0x04)
#define ETHER_FLGPTN_DMA_RX		(0x10)
#define ETHER_FLGPTN_DMA_TX		(0x20)
#define ETHER_FLGPTN_DMA_ERROR	(0x40)

#define ETHER_FLGPTN_FATAL_ERR (0x00010000)

/*
 * Constants for device information
 */
#define ETHER_DEVINFO_DEVNAME "NUCLEO-L476-W5500"
#define ETHER_DEVINFO_IOBASE (0x00000000UL)
#define ETHER_DEVINFO_IOSIZE (512)
#define ETHER_DEVINFO_INTNO  (32)		/* Interrupt vector.*/
#define ETHER_DEVINFO_IFCONN (IFC_AUTO)

/*
 * Initializes.
 *
 * @param ch               ETHERC channel (0 or 1)
 * @param macaddr          MAC Address (48-bits)
 * @param mii_mode         Use MII if TRUE, otherwise use RMII.
 * @param promiscuous_mode Use promisucuous mode if TRUE.
 *
 * @retval E_OK 
 * @retval <0   Error
 */
ER ether_init( INT ch, const NetAddr * macaddr, BOOL mii_mode,
	       BOOL promiscuous_mode );

/*
 * Gets the ethernet physical address.
 */
ER ether_get_phyaddr( INT ch, NetAddr * addr );

/*
 * Sends a packet.
 */
ER ether_send( INT ch, const void * buf, INT len, TMO tmout );

/*
 * Check if the link is up.
 */
BOOL ether_check_link( INT ch );

/*
 * Sets event notification message buffer ID.
 */
ER ether_set_msgbuf( INT ch, ID id );
/*
 * Gets event notification message buffer ID.
 */
ER ether_get_msgbuf( INT ch, ID * id );

/*
 * Resets the network adaptor.
 */
ER ether_reset( INT ch, W data, ID mbfid );

/*
 * Gets the ethernet physical address.
 */
ER ether_get_phyaddr( INT ch, NetAddr * addr );

/*
 * Gets device informatio non the network adaptor.
 */
ER ether_get_devinfo( INT ch, NetDevInfo * devinfo );

/*
 * Gets network adaptor statistics information.
 */
ER ether_get_stinfo( INT ch, NetStInfo * stinfo );

/*
 * Clears network adaptor statistics information to zero.
 */
ER ether_clear_stinfo( INT ch );

/*
 * Sets the receive buffer.
 *
 * If buf is NULL, then this function unsets all buffers.
 */
ER ether_set_rxbuf( INT ch, void *buf );

/*
 * Gets the maximum and minimum sizes of apckets to be received.
 */
ER ether_get_rxbuf_size( INT ch, NetRxBufSz * bufsz );

/*
 * Sets the maximum and minimum sizes of apckets to be received.
 */
ER ether_set_rxbuf_size( INT ch, const NetRxBufSz * bufsz );

/*
 * Enable/Disable Multicast receive.
 */
void ether_set_multicast(INT ch, BOOL enable);

#define ETH_FIFO_ALIGN( buf, n ) \
	( (((UW)(buf)) + ((1 << (n)) - 1)) & ~((1 << (n)) - 1) )
#define ETH_MAX_DATA_LENGTH	(1518)
#define ETH_MAX_BUFSIZE		(ETH_FIFO_ALIGN(ETH_MAX_DATA_LENGTH, 4))
#define ETH_RXBUF_MAX_SIZE	(ETH_MAX_BUFSIZE)
#define ETH_RXBUF_MIN_SIZE	(60)

/*
 * Default values for the maximum size and the minimum size for receiving
 * buffers, which are defined in 'T-Engine Standard Device Driver Specifications.'
 */
#define ETHER_RXBUF_MAX_SIZE (ETH_MAX_BUFSIZE)
#define ETHER_RXBUF_MIN_SIZE (60)

#endif /* __NET_ETHER_H__ */
