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
 *    Modifications: Adapted to the ADBUN-M4NR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#ifndef __NETDRV_MACCTRL_H__
#define __NETDRV_MACCTRL_H__

//#include <basic.h>
#include <tk/tkernel.h>
#include <dev_net.h>
#include <sys/queue.h>

/*--------- EtherMAC Unit A Registers ---------*/
#define ETHMA_BASE			(0x4000A000ul)		/* ETHM-A */
#define EMA_MAC_CTRL0		(_UW*)(ETHMA_BASE+0x1800ul)
#define EMA_MAC_CTRL1		(_UW*)(ETHMA_BASE+0x1804ul)
#define EMA_MAC_CTRL2		(_UW*)(ETHMA_BASE+0x1808ul)

#define EMA_MAC_CONFIGURATION				(_UW*)(ETHMA_BASE+0x0000ul)
#define EMA_MAC_FRAME_FILTER				(_UW*)(ETHMA_BASE+0x0004ul)
#define EMA_HASH_TABLE_HIGH					(_UW*)(ETHMA_BASE+0x0008ul)
#define EMA_HASH_TABLE_LOW					(_UW*)(ETHMA_BASE+0x000Cul)
#define EMA_MII_ADDRESS						(_UW*)(ETHMA_BASE+0x0010ul)
#define EMA_MII_DATA						(_UW*)(ETHMA_BASE+0x0014ul)
#define EMA_MFLOW_CONTROL					(_UW*)(ETHMA_BASE+0x0018ul)
#define EMA_VLAN_TAG						(_UW*)(ETHMA_BASE+0x001Cul)
#define EMA_DEBUG							(_UW*)(ETHMA_BASE+0x0024ul)
#define EMA_REMOTE_WAKE_UP_FRAME_FILTER		(_UW*)(ETHMA_BASE+0x0028ul)
#define EMA_PMT_CONTROL_STATUS				(_UW*)(ETHMA_BASE+0x002Cul)
#define EMA_INTERRUPT_STATUS				(_UW*)(ETHMA_BASE+0x0038ul)
#define EMA_INTERRUPT_MASK					(_UW*)(ETHMA_BASE+0x003Cul)
#define EMA_MAC_ADDRESS0_HIGH				(_UW*)(ETHMA_BASE+0x0040ul)
#define EMA_MAC_ADDRESS0_LOW				(_UW*)(ETHMA_BASE+0x0044ul)
#define EMA_MAC_ADDRESS1_HIGH				(_UW*)(ETHMA_BASE+0x0048ul)
#define EMA_MAC_ADDRESS1_LOW				(_UW*)(ETHMA_BASE+0x004Cul)
#define EMA_MAC_ADDRESS2_HIGH				(_UW*)(ETHMA_BASE+0x0050ul)
#define EMA_MAC_ADDRESS2_LOW				(_UW*)(ETHMA_BASE+0x0054ul)
#define EMA_MAC_ADDRESS3_HIGH				(_UW*)(ETHMA_BASE+0x0058ul)
#define EMA_MAC_ADDRESS3_LOW				(_UW*)(ETHMA_BASE+0x005Cul)
#define EMA_WDOG_TIMEOUT					(_UW*)(ETHMA_BASE+0x00DCul)
#define EMA_MMC_CONTROL						(_UW*)(ETHMA_BASE+0x0100ul)
#define EMA_MMC_RECEIVE_INTERRUPT			(_UW*)(ETHMA_BASE+0x0104ul)
#define EMA_MMC_TRANSMIT_INTERRUPT			(_UW*)(ETHMA_BASE+0x0108ul)
#define EMA_MMC_RECEIVE_INTERRUPT_MASK		(_UW*)(ETHMA_BASE+0x010Cul)
#define EMA_MMC_TRANSMIT_INTERRUPT_MASK		(_UW*)(ETHMA_BASE+0x0110ul)

#define EMA_TX_FRAME_COUNT_GOOD_BAD				(_UW*)(ETHMA_BASE+0x0118ul)
#define EMA_TX_UNDERFLOW_ERROR_FRAMES			(_UW*)(ETHMA_BASE+0x0148ul)
#define EMA_TX_SINGLE_COLLISION_GOOD_FRAMES		(_UW*)(ETHMA_BASE+0x014Cul)
#define EMA_TX_MULTIPLE_COLLISION_GOOD_FRAMES	(_UW*)(ETHMA_BASE+0x0150ul)
#define EMA_TX_FRAME_COUNT_GOOD					(_UW*)(ETHMA_BASE+0x0168ul)

#define EMA_RX_FRAMES_COUNT_GOOD_BAD			(_UW*)(ETHMA_BASE+0x0180ul)
#define EMA_RX_CRC_ERROR_FRAMES					(_UW*)(ETHMA_BASE+0x0194ul)
#define EMA_RX_ALIGNMENT_ERROR_FRAMES			(_UW*)(ETHMA_BASE+0x0198ul)
#define EMA_RX_UNICAST_FRAMES_GOOD				(_UW*)(ETHMA_BASE+0x01C4ul)
#define EMA_RX_FIFO_OVERFLOW_FRAMES				(_UW*)(ETHMA_BASE+0x01D4ul)

#define EMA_MMC_IPC_RECEIVE_INTERRUPT_MASK		(_UW*)(ETHMA_BASE+0x0200ul)
#define EMA_MMC_IPC_RECEIVE_INTERRUPT			(_UW*)(ETHMA_BASE+0x0208ul)
#define EMA_RXICMP_GOOD_FRAMES					(_UW*)(ETHMA_BASE+0x0240ul)
#define EMA_RXICMP_ERROR_FRAMES					(_UW*)(ETHMA_BASE+0x0244ul)
#define EMA_VLAN_INCL_REG						(_UW*)(ETHMA_BASE+0x0584ul)
#define EMA_VLAN_HASH_TABLE_REG					(_UW*)(ETHMA_BASE+0x0588ul)
#define EMA_TIMESTAMP_CONTROL					(_UW*)(ETHMA_BASE+0x0700ul)
#define EMA_SUB_SECOND_INCREMENT				(_UW*)(ETHMA_BASE+0x0704ul)
#define EMA_SYSTEM_TIME_SECONDS					(_UW*)(ETHMA_BASE+0x0708ul)
#define EMA_SYSTEM_TIME_NANOSECONDS				(_UW*)(ETHMA_BASE+0x070Cul)
#define EMA_SYSTEM_TIME_SECONDS_UPDATE			(_UW*)(ETHMA_BASE+0x0710ul)
#define EMA_SYSTEM_TIME_NANOSECONDS_UPDATE		(_UW*)(ETHMA_BASE+0x0714ul)
#define EMA_TIMESTAMP_ADDEND					(_UW*)(ETHMA_BASE+0x0718ul)
#define EMA_TARGET_TIME_SECONDS					(_UW*)(ETHMA_BASE+0x071Cul)
#define EMA_TARGET_TIME_NANOSECONDS				(_UW*)(ETHMA_BASE+0x0720ul)
#define EMA_SYSTEM_TIME_HIGHER_WORD_SECONDS		(_UW*)(ETHMA_BASE+0x0724ul)
#define EMA_TIMESTAMP_STATUS					(_UW*)(ETHMA_BASE+0x0728ul)
#define EMA_PPS_CONTROL							(_UW*)(ETHMA_BASE+0x072Cul)
#define EMA_PPS0_INTERVAL						(_UW*)(ETHMA_BASE+0x0760ul)
#define EMA_PPS0_WIDTH							(_UW*)(ETHMA_BASE+0x0764ul)
#define EMA_PPS1_TARGET_TIME_SECONDS			(_UW*)(ETHMA_BASE+0x0780ul)
#define EMA_PPS1_TARGET_TIME_NANOSECONDS		(_UW*)(ETHMA_BASE+0x0784ul)
#define EMA_PPS1_INTERVAL						(_UW*)(ETHMA_BASE+0x0788ul)
#define EMA_PPS1_WIDTH							(_UW*)(ETHMA_BASE+0x078Cul)

/* EMA_MAC_CTRL0 */
#define EMA_MAC_CTRL0_CLK_LM_NORMAL			(0x00000000ul)
#define EMA_MAC_CTRL0_CLK_LM_TMAC_LOOPBACK	(0x00000010ul)
#define EMA_MAC_CTRL0_PHY_INTF_SEL_MII		(0x00000000ul)
#define EMA_MAC_CTRL0_PHY_INTF_SEL_RMII		(0x00000004ul)

/* EMA_MAC_CTRL1 */
#define EMA_MAC_CTRL1_RXCLKEN				(0x00000002ul)
#define EMA_MAC_CTRL1_TXCLKEN				(0x00000001ul)

/* EMA_MAC_CTRL2 */
#define EMA_MAC_CTRL2_SRST_ENABLE			(0x00000000ul)
#define EMA_MAC_CTRL2_SRST_DISABLE			(0x00000001ul)

/* EMA_MAC_CONFIGURATION */
#define EMA_MAC_CONFIGURATION_SARC_2		(0x20000000ul)
#define EMA_MAC_CONFIGURATION_SARC_3		(0x30000000ul)
#define EMA_MAC_CONFIGURATION_SARC_6		(0x60000000ul)
#define EMA_MAC_CONFIGURATION_SARC_7		(0x70000000ul)
#define EMA_MAC_CONFIGURATION_TWOKPE		(0x08000000ul)
#define EMA_MAC_CONFIGURATION_CST			(0x02000000ul)
#define EMA_MAC_CONFIGURATION_WD			(0x00800000ul)
#define EMA_MAC_CONFIGURATION_JD			(0x00400000ul)
#define EMA_MAC_CONFIGURATION_JE			(0x00100000ul)
#define EMA_MAC_CONFIGURATION_IFG_96		(0x00000000ul)
#define EMA_MAC_CONFIGURATION_IFG_88		(0x00020000ul)
#define EMA_MAC_CONFIGURATION_IFG_80		(0x00040000ul)
#define EMA_MAC_CONFIGURATION_IFG_72		(0x00060000ul)
#define EMA_MAC_CONFIGURATION_IFG_64		(0x00080000ul)
#define EMA_MAC_CONFIGURATION_IFG_56		(0x000A0000ul)
#define EMA_MAC_CONFIGURATION_IFG_48		(0x000C0000ul)
#define EMA_MAC_CONFIGURATION_IFG_40		(0x000E0000ul)
#define EMA_MAC_CONFIGURATION_DCRS			(0x00010000ul)
#define EMA_MAC_CONFIGURATION_FES_10M		(0x00000000ul)
#define EMA_MAC_CONFIGURATION_FES_100M		(0x00004000ul)
#define EMA_MAC_CONFIGURATION_DO			(0x00002000ul)
#define EMA_MAC_CONFIGURATION_LM			(0x00001000ul)
#define EMA_MAC_CONFIGURATION_DM_HALF		(0x00000000ul)
#define EMA_MAC_CONFIGURATION_DM_FULL		(0x00000800ul)
#define EMA_MAC_CONFIGURATION_IPC			(0x00000400ul)
#define EMA_MAC_CONFIGURATION_DR			(0x00000200ul)
#define EMA_MAC_CONFIGURATION_ACS			(0x00000080ul)
#define EMA_MAC_CONFIGURATION_BL_10			(0x00000000ul)
#define EMA_MAC_CONFIGURATION_BL_8			(0x00000020ul)
#define EMA_MAC_CONFIGURATION_BL_4			(0x00000040ul)
#define EMA_MAC_CONFIGURATION_BL_1			(0x00000060ul)
#define EMA_MAC_CONFIGURATION_DC			(0x00000010ul)
#define EMA_MAC_CONFIGURATION_TE			(0x00000008ul)
#define EMA_MAC_CONFIGURATION_RE			(0x00000004ul)
#define EMA_MAC_CONFIGURATION_PRELEN_7		(0x00000000ul)
#define EMA_MAC_CONFIGURATION_PRELEN_5		(0x00000001ul)
#define EMA_MAC_CONFIGURATION_PRELEN_3		(0x00000002ul)

/* EMA_MAC_FRAME_FILTER */
#define EMA_MAC_FRAME_FILTER_RA				(0x80000000ul)
#define EMA_MAC_FRAME_FILTER_VTFE			(0x00010000ul)
#define EMA_MAC_FRAME_FILTER_HPF			(0x00000400ul)
#define EMA_MAC_FRAME_FILTER_SAF			(0x00000200ul)
#define EMA_MAC_FRAME_FILTER_SAIF			(0x00000100ul)
#define EMA_MAC_FRAME_FILTER_PCF_0			(0x00000000ul)
#define EMA_MAC_FRAME_FILTER_PCF_1			(0x00000040ul)
#define EMA_MAC_FRAME_FILTER_PCF_2			(0x00000080ul)
#define EMA_MAC_FRAME_FILTER_PCF_3			(0x000000C0ul)
#define EMA_MAC_FRAME_FILTER_DBF			(0x00000020ul)
#define EMA_MAC_FRAME_FILTER_PM				(0x00000010ul)
#define EMA_MAC_FRAME_FILTER_DAIF			(0x00000008ul)
#define EMA_MAC_FRAME_FILTER_HMC			(0x00000004ul)
#define EMA_MAC_FRAME_FILTER_HUC			(0x00000002ul)
#define EMA_MAC_FRAME_FILTER_PR				(0x00000001ul)

/* EMA_MII_ADDRESS */
#define EMA_MII_ADDRESS_PA(x)				(((x)&0x1F)<<11)
#define EMA_MII_ADDRESS_GR(x)				(((x)&0x1F)<<6)
#define EMA_MII_ADDRESS_CR_DIV_42			(0x00000000ul)
#define EMA_MII_ADDRESS_CR_DIV_62			(0x00000004ul)
#define EMA_MII_ADDRESS_CR_DIV_16			(0x00000008ul)
#define EMA_MII_ADDRESS_CR_DIV_26			(0x0000000Cul)
#define EMA_MII_ADDRESS_CR_DIV_102			(0x00000010ul)
#define EMA_MII_ADDRESS_GW_WRITE			(0x00000002ul)
#define EMA_MII_ADDRESS_GW_READ				(0x00000000ul)
#define EMA_MII_ADDRESS_GB					(0x00000001ul)

/* EMA_MFLOW_CONTROL */
#define EMA_MFLOW_CONTROL_PT(x)				(((x)&0xFFFF)<<16)
#define EMA_MFLOW_CONTROL_DZPQ				(0x00000080ul)
#define EMA_MFLOW_CONTROL_PLT_4				(0x00000000ul)
#define EMA_MFLOW_CONTROL_PLT_28			(0x00000010ul)
#define EMA_MFLOW_CONTROL_PLT_144			(0x00000020ul)
#define EMA_MFLOW_CONTROL_PLT_256			(0x00000030ul)
#define EMA_MFLOW_CONTROL_UP				(0x00000008ul)
#define EMA_MFLOW_CONTROL_RFE				(0x00000004ul)
#define EMA_MFLOW_CONTROL_TFE				(0x00000002ul)
#define EMA_MFLOW_CONTROL_FCA				(0x00000001ul)

/* EMA_INTERRUPT_MASK */
#define EMA_INTERRUPT_MASK_TSIM				(0x00000200ul)
#define EMA_INTERRUPT_MASK_PMTIM			(0x00000008ul)


/* TDMA Registers */
#define EMA_BUS_MODE									(_UW*)(ETHMA_BASE+0x1000ul)
#define EMA_TRANSMIT_POLL_DEMAND						(_UW*)(ETHMA_BASE+0x1004ul)
#define EMA_RECEIVE_POLL_DEMAND							(_UW*)(ETHMA_BASE+0x1008ul)
#define EMA_RECEIVE_DESCRIPTOR_LIST_ADDRESS				(_UW*)(ETHMA_BASE+0x100Cul)
#define EMA_TRANSMIT_DESCRIPTOR_LIST_ADDRESS			(_UW*)(ETHMA_BASE+0x1010ul)
#define EMA_STATUS										(_UW*)(ETHMA_BASE+0x1014ul)
#define EMA_OPERATION_MODE								(_UW*)(ETHMA_BASE+0x1018ul)
#define EMA_INTERRUPT_ENABLE							(_UW*)(ETHMA_BASE+0x101Cul)
#define EMA_MISSED_FRAME_AND_BUFFER_OVERFLOW_COUNTER	(_UW*)(ETHMA_BASE+0x1020ul)
#define EMA_RECEIVE_INTERRUPT_WATCHDOG_TIMER			(_UW*)(ETHMA_BASE+0x1024ul)
#define ET_AHB_STATUS									(_UW*)(ETHMA_BASE+0x102Cul)
#define EMA_CURRENT_HOST_TRANSMIT_DESCRIPTOR			(_UW*)(ETHMA_BASE+0x1048ul)
#define EMA_CURRENT_HOST_RECEIVE_DESCRIPTOR				(_UW*)(ETHMA_BASE+0x104Cul)
#define EMA_CURRENT_HOST_TRANSMIT_BUFFER_ADDRESS		(_UW*)(ETHMA_BASE+0x1050ul)
#define EMA_CURRENT_HOST_RECEIVE_BUFFER_ADDRESS			(_UW*)(ETHMA_BASE+0x1054ul)

/* EMA_BUS_MODE */
#define EMA_BUS_MODE_TXPR					(0x08000000ul)
#define EMA_BUS_MODE_MB						(0x04000000ul)
#define EMA_BUS_MODE_AAL					(0x02000000ul)
#define EMA_BUS_MODE_PBLx8					(0x01000000ul)
#define EMA_BUS_MODE_USP					(0x00800000ul)
#define EMA_BUS_MODE_RPBL_32				(0x00400000ul)
#define EMA_BUS_MODE_RPBL_16				(0x00200000ul)
#define EMA_BUS_MODE_RPBL_8					(0x00100000ul)
#define EMA_BUS_MODE_RPBL_4					(0x00080000ul)
#define EMA_BUS_MODE_RPBL_2					(0x00040000ul)
#define EMA_BUS_MODE_RPBL_1					(0x00020000ul)
#define EMA_BUS_MODE_FB						(0x00010000ul)
#define EMA_BUS_MODE_PR_4					(0x0000C000ul)
#define EMA_BUS_MODE_PR_3					(0x00008000ul)
#define EMA_BUS_MODE_PR_2					(0x00004000ul)
#define EMA_BUS_MODE_PR_1					(0x00000000ul)
#define EMA_BUS_MODE_PBL_32					(0x00002000ul)
#define EMA_BUS_MODE_PBL_16					(0x00001000ul)
#define EMA_BUS_MODE_PBL_8					(0x00000800ul)
#define EMA_BUS_MODE_PBL_4					(0x00000400ul)
#define EMA_BUS_MODE_PBL_2					(0x00000200ul)
#define EMA_BUS_MODE_PBL_1					(0x00000100ul)
#define EMA_BUS_MODE_ATDS					(0x00000080ul)
#define EMA_BUS_MODE_DSL(x)					(((x)&0x1F)<<2)
#define EMA_BUS_MODE_SWR					(0x00000001ul)

/* EMA_STATUS */
#define EMA_STATUS_TTI						(0x20000000ul)
#define EMA_STATUS_GPI						(0x10000000ul)
#define EMA_STATUS_GMI						(0x08000000ul)
#define EMA_STATUS_EB_MASK					(0x03800000ul)
#define EMA_STATUS_EB_RX_DMA_WRITE_DATA		(0x00000000ul)
#define EMA_STATUS_EB_TX_DMA_READ_DATA		(0x01800000ul)
#define EMA_STATUS_EB_RX_DMA_WRITE_DES		(0x02000000ul)
#define EMA_STATUS_EB_TX_DMA_WRITE_DES		(0x02800000ul)
#define EMA_STATUS_EB_RX_DMA_READ_DES		(0x03000000ul)
#define EMA_STATUS_EB_TX_DMA_READ_DES		(0x03800000ul)
#define EMA_STATUS_TS_MASK					(0x00700000ul)
#define EMA_STATUS_TS_STOP					(0x00000000ul)
#define EMA_STATUS_TS_RUNNING_FETCH_DES		(0x00100000ul)
#define EMA_STATUS_TS_RUNNING_WAIT_STATUS	(0x00200000ul)
#define EMA_STATUS_TS_RUNNING_WRITE_FIFO	(0x00300000ul)
#define EMA_STATUS_TS_WRITE_TIMESTAMP		(0x00400000ul)
#define EMA_STATUS_TS_STOPPING_UNDERFLOW	(0x00600000ul)
#define EMA_STATUS_TS_CLOSE_DES				(0x00700000ul)
#define EMA_STATUS_RS_MASK					(0x000E0000ul)
#define EMA_STATUS_RS_STOP					(0x00000000ul)
#define EMA_STATUS_RS_RUNNING_FETCH_DES		(0x00020000ul)
#define EMA_STATUS_RS_RUNNING_WAIT_PKT		(0x00060000ul)
#define EMA_STATUS_RS_PAUSE_NO_DES			(0x00080000ul)
#define EMA_STATUS_RS_RUNNING_CLOSE_DES		(0x000A0000ul)
#define EMA_STATUS_RS_WRITE_TIMESTAMP		(0x000C0000ul)
#define EMA_STATUS_RS_RUNNING_WRITE_BUF		(0x000E0000ul)
#define EMA_STATUS_NIS						(0x00010000ul)
#define EMA_STATUS_AIS						(0x00008000ul)
#define EMA_STATUS_ERI						(0x00004000ul)
#define EMA_STATUS_FBI						(0x00002000ul)
#define EMA_STATUS_ETI						(0x00000400ul)
#define EMA_STATUS_RWT						(0x00000200ul)
#define EMA_STATUS_RPS						(0x00000100ul)
#define EMA_STATUS_RU						(0x00000080ul)
#define EMA_STATUS_RI						(0x00000040ul)
#define EMA_STATUS_UNF						(0x00000020ul)
#define EMA_STATUS_OVF						(0x00000010ul)
#define EMA_STATUS_TJT						(0x00000008ul)
#define EMA_STATUS_TU						(0x00000004ul)
#define EMA_STATUS_TPS						(0x00000002ul)
#define EMA_STATUS_TI						(0x00000001ul)

/* EMA_OPERATION_MODE */
#define EMA_OPERATION_MODE_DT				(0x04000000ul)
#define EMA_OPERATION_MODE_RSF				(0x02000000ul)
#define EMA_OPERATION_MODE_DFF				(0x01000000ul)
#define EMA_OPERATION_MODE_FTF				(0x00100000ul)
#define EMA_OPERATION_MODE_TTC_64			(0x00000000ul)
#define EMA_OPERATION_MODE_TTC_128			(0x00004000ul)
#define EMA_OPERATION_MODE_TTC_192			(0x00008000ul)
#define EMA_OPERATION_MODE_TTC_256			(0x0000C000ul)
#define EMA_OPERATION_MODE_TTC_40			(0x00010000ul)
#define EMA_OPERATION_MODE_TTC_32			(0x00014000ul)
#define EMA_OPERATION_MODE_TTC_24			(0x00018000ul)
#define EMA_OPERATION_MODE_TTC_16			(0x0001C000ul)
#define EMA_OPERATION_MODE_ST				(0x00002000ul)
#define EMA_OPERATION_MODE_FEF				(0x00000080ul)
#define EMA_OPERATION_MODE_FUF				(0x00000040ul)
#define EMA_OPERATION_MODE_DGF				(0x00000020ul)
#define EMA_OPERATION_MODE_RTC_64			(0x00000000ul)
#define EMA_OPERATION_MODE_RTC_32			(0x00000008ul)
#define EMA_OPERATION_MODE_RTC_96			(0x00000010ul)
#define EMA_OPERATION_MODE_RTC_128			(0x00000018ul)
#define EMA_OPERATION_MODE_OSF				(0x00000004ul)
#define EMA_OPERATION_MODE_SR				(0x00000002ul)


/* EMA_INTERRUPT_ENABLE */
#define EMA_INTERRUPT_ENABLE_NIS			(0x00010000ul)
#define EMA_INTERRUPT_ENABLE_AIS			(0x00008000ul)
#define EMA_INTERRUPT_ENABLE_ERE			(0x00004000ul)
#define EMA_INTERRUPT_ENABLE_FBE			(0x00002000ul)
#define EMA_INTERRUPT_ENABLE_ETE			(0x00000400ul)
#define EMA_INTERRUPT_ENABLE_RWE			(0x00000200ul)
#define EMA_INTERRUPT_ENABLE_RSE			(0x00000100ul)
#define EMA_INTERRUPT_ENABLE_RUE			(0x00000080ul)
#define EMA_INTERRUPT_ENABLE_RIE			(0x00000040ul)
#define EMA_INTERRUPT_ENABLE_UNE			(0x00000020ul)
#define EMA_INTERRUPT_ENABLE_OVE			(0x00000010ul)
#define EMA_INTERRUPT_ENABLE_TJE			(0x00000008ul)
#define EMA_INTERRUPT_ENABLE_TUE			(0x00000004ul)
#define EMA_INTERRUPT_ENABLE_TSE			(0x00000002ul)
#define EMA_INTERRUPT_ENABLE_TIE			(0x00000001ul)

/* ET_AHB_STATUS */
#define ET_AHB_STATUS_AHSTS					(0x00000001ul)


/*--------- PHY Registers ---------*/
#define PHY_MII_REG_CR		(0)
#define PHY_MII_REG_SR		(1)
#define PHY_MII_REG_SCSR	(0x1F)

#define PHY_MII_REG_CR_RESET		(0x01 << 15)
#define PHY_MII_REG_CR_AN_ENABLE	(0x01 << 12)
#define PHY_MII_REG_SR_LINK_STATUS	(0x01 << 2)

#define PHY_MII_REG_PHYSCSR_DUPLEX_MASK			(0x07 << 2)
#define PHY_MII_REG_PHYSCSR_DUPLEX_HALF_10T		(0x01 << 2)
#define PHY_MII_REG_PHYSCSR_DUPLEX_FULL_10T		(0x05 << 2)
#define PHY_MII_REG_PHYSCSR_DUPLEX_HALF_100T	(0x02 << 2)
#define PHY_MII_REG_PHYSCSR_DUPLEX_FULL_100T	(0x06 << 2)
#define PHY_MII_REG_PHYSCSR_AN_COMPLETE			(0x01 << 12)

#define ETH_PHY_READ_TMOUT	(0x4fffful)
#define ETH_PHY_WRITE_TMOUT	(0x4fffful)

#define ETH_PHY_ADDRESS	(0x00)

typedef struct {
	// TDES0
	UW	db:1;
	UW	uf:1;
	UW	ed:1;
	UW	cc:4;
	UW	vf:1;
	UW	ec:1;
	UW	lc:1;
	UW	nc:1;
	UW	loc:1;
	UW	pce:1;
	UW	ff:1;
	UW	jt:1;
	UW	es:1;
	UW	ihe:1;
	UW	ttss:1;
	UW	rsv1:2;
	UW	tch:1;
	UW	ter:1;
	UW	rsv2:6;
	UW	fs:1;
	UW	ls:1;
	UW	ic:1;
	UW	own:1;
	
	// TDES1
	UW	tbs1:11;
	UW	tbs2:11;
	UW	ttse:1;
	UW	dp:1;
	UW	_tch:1;
	UW	_ter:1;
	UW	dc:1;
	UW	cic:2;
	UW	_fs:1;
	UW	_ls:1;
	UW	_ic:1;
	
	// TDES2
	UW	buffer1;
	
	// TDES3
	UW	buffer2;
} ETH_DMA_TX_DESC;

typedef struct {
	// RDES0
	UW	pce:1;
	UW	ce:1;
	UW	de:1;
	UW	re:1;
	UW	rwt:1;
	UW	ft:1;
	UW	lc:1;
	UW	ice:1;
	UW	ls:1;
	UW	fs:1;
	UW	vlan:1;
	UW	oe:1;
	UW	le:1;
	UW	saf:1;
	UW	dbe:1;
	UW	es:1;
	UW	fl:14;
	UW	afm:1;
	UW	own:1;
	
	// RDES1
	UW	rbs1:11;
	// UW	rbs2:11;
	UW	rsv3:3;
	UW	rch:1;
	UW	rer:1;
	UW	rsv4:6;
	UW	rsv1:2;
	UW	_rch:1;
	UW	_rer:1;
	UW	rsv2:5;
	UW	dic:1;
	
	// RDES2
	UW	buffer1;
	
	// RDES3
	UW	buffer2;
} ETH_DMA_RX_DESC;

#define ETH_DMA_RX_DESC_NUM	(8)
#define ETH_VECTOR			(156)
#define ETH_IRQ_PRI			(4)
#define ETH_IRQ_SUBPRI		(0)

#define ENET_FIFO_ALIGN( buf, n ) \
	( (((UW)(buf)) + ((1 << (n)) - 1)) & ~((1 << (n)) - 1) )
#define ENET_MAX_DATA_LENGTH (1518)
#define ENET_MAX_BUFSIZE ( ENET_FIFO_ALIGN(ENET_MAX_DATA_LENGTH, 4) )

typedef void		*VP;
typedef struct {
	INT next_rxbd;	/* Indicator to a next receive buffer descriptor. */
	ID flgid;	/* Eventflag ID for notifying events. */
	QUEUE freeq;	/* Free queue for bufferb . */
	INT bufnum;	/* The number of buffer assigned to BDs. */
	INT max_bufsz;
} NETDRV_MACCTRL_CONTROL_BLOCK;

void netdrv_macctrl_start_rx( void );
ER netdrv_macctrl_init( ID flgid, const NetAddr *addr, BOOL promiscuous_mode );
void netdrv_macctrl_set_address( const NetAddr *addr );
void netdrv_macctrl_start_tx(void);
ER netdrv_macctrl_get_recv_event( NetEvent * event );
ER netdrv_macctrl_set_rxbuf_size( INT size );
ER netdrv_macctrl_request_send( const void * buf, INT len );
ER netdrv_macctrl_enqueue_rxbuf( VP ptr );
BOOL netdrv_macctrl_check_link( void );
ER netdrv_macctrl_request_send( const void * buf, INT len );
ER netdrv_macctrl_clear_rxbuf( void );

#endif /* __NETDRV_MACCTRL_H__ */
