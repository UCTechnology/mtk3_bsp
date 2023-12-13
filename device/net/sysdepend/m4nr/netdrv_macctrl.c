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

#include "netdrv_macctrl.h"
#include "../../net_stat.h"
#include "net_ether.h"
#include <lwipopts.h>
#include <string.h>

#define PHY_NEGOTIATE_TMOUT		10000

LOCAL void netdrv_macctrl_init_gpio( void );
LOCAL ER netdrv_macctrl_init_macdma( const NetAddr *addr, BOOL promiscuous_mode );
LOCAL ER netdrv_macctrl_init_eth( const NetAddr *addr, BOOL promiscuous_mode );
ER netdrv_macctrl_read_phy_reg( UH phyaddr, UH phyreg, UH *phyval );
ER netdrv_macctrl_write_phy_reg( UH phyaddr, UH phyreg, UH phyval );
LOCAL void netdrv_macctrl_init_txdesc( ETH_DMA_TX_DESC *txdesc );
LOCAL void netdrv_macctrl_init_rxdesc( INT n, ETH_DMA_RX_DESC *rxdesc );
LOCAL void netdrv_macctrl_clear_interrupts( UW status );
LOCAL void netdrv_macctrl_enable_interrupts( void );
LOCAL void netdrv_macctrl_disable_interrupts( void );

LOCAL ER netdrv_macctrl_define_inthdr( void );
LOCAL void netdrv_macctrl_inthdr( UINT intno );
LOCAL void netdrv_macctrl_txint_inthdr( void );
LOCAL void netdrv_macctrl_rxint_inthdr( void );
LOCAL void netdrv_macctrl_incr_next_rxbd( void );

LOCAL void netdrv_macctrl_reset_rxdesc( ETH_DMA_RX_DESC * desc );

#if defined(__ICCARM__)

#pragma data_alignment=ETHER_BUF_ALIGNMENT
LOCAL ETH_DMA_TX_DESC txdesc;
#pragma data_alignment=ETHER_BUF_ALIGNMENT
LOCAL ETH_DMA_RX_DESC rxdesc[ ETH_DMA_RX_DESC_NUM ];

#elif defined(__CC_ARM)

__align(ETHER_BUF_ALIGNMENT)
LOCAL ETH_DMA_TX_DESC txdesc;
__align(ETHER_BUF_ALIGNMENT)
LOCAL ETH_DMA_RX_DESC rxdesc[ ETH_DMA_RX_DESC_NUM ];

#elif defined(__GNUC__)
LOCAL ETH_DMA_TX_DESC txdesc __attribute__ ((aligned(ETHER_BUF_ALIGNMENT)));
LOCAL ETH_DMA_RX_DESC rxdesc[ ETH_DMA_RX_DESC_NUM ] __attribute__ ((aligned(ETHER_BUF_ALIGNMENT)));
#endif



LOCAL NETDRV_MACCTRL_CONTROL_BLOCK netmaccb;

ER netdrv_macctrl_init( ID flgid, const NetAddr *addr, BOOL promiscuous_mode )
{
	memset( &netmaccb, 0, sizeof( NETDRV_MACCTRL_CONTROL_BLOCK ) );
	netmaccb.flgid = flgid;
	QueInit( &netmaccb.freeq );
	netmaccb.bufnum = 0;
	netmaccb.max_bufsz = ETHER_RXBUF_MAX_SIZE;

	/* Configure the GPIO ports for ethernet pins */
	netdrv_macctrl_init_gpio();
  
	/* Configure the Ethernet MAC/DMA */
	netdrv_macctrl_init_macdma(addr, promiscuous_mode);
  
	return E_OK;
}

IMPORT	void	set_port_func(INT port, INT no, UW func, UW inout );
LOCAL void netdrv_macctrl_init_gpio()
{
	/* Ethernet pins configuration ************************************************/
	/*
	  ETH_MDIO -------------------------> PH0
	  ETH_MDC --------------------------> PG7
	  ETH_MII_COL ----------------------> PG3
	  ETH_MII_RX_CLK -------------------> PL1
	  ETH_MII_CRS ----------------------> PG2
	  ETH_MII_RX_DV --------------------> PG0
	  ETH_MII_RX_ER --------------------> PG1
	  ETH_MII_RXD0 ---------------------> PL2
	  ETH_MII_RXD1 ---------------------> PL3
	  ETH_MII_RXD2 ---------------------> PG4
	  ETH_MII_RXD3 ---------------------> PG5
	  ETH_MII_TXD0 ---------------------> PM3
	  ETH_MII_TXD1 ---------------------> PM2
	  ETH_MII_TXD2 ---------------------> PM1
	  ETH_MII_TXD3 ---------------------> PM0
	  ETH_MII_TX_EN --------------------> PK0
	  ETH_MII_TX_CLK -------------------> PT4
	*/
	// Enable GPIO G, H, K, L, M, T
	*CGPROTECT = 0xC1;
	*CGFSYSMENB |= (CGFSYSMENB_PG | CGFSYSMENB_PH | CGFSYSMENB_PK | 
				    CGFSYSMENB_PL | CGFSYSMENB_PM | CGFSYSMENB_PT);
	*CGPROTECT = 0x00;
	
	// PG0(RX_DV/IN), PG1(RX_ER/IN), PG3(COL/IN), PG4(RXD2/IN), PG5(RXD3/IN), PG7(MDC/OUT)
	*PxCR('G') = (*PxCR('G') & ~0x3B) | 0x80;
	*PxFRn('G', 8) |= 0xBB;
	*PxIE('G') = (*PxIE('G') | 0x3B) & ~0x80;
	
	// PH0(MDC/IN/OUT)
	*PxCR('H') |= 0x01;
	*PxFRn('H', 8) |= 0x01;
	*PxIE('H') |= 0x01;
	
	// PK0(TX_EN/OUT)
	*PxCR('K') |= 0x01;
	*PxFRn('K', 8) |= 0x01;
	*PxIE('K') &= ~0x01;
	
	// PL1(RX_CLK/IN) PL2(RXD0/IN) PL3(RXD1/IN)
	*PxCR('L') &= ~0x0E;
	*PxFRn('L', 8) |= 0x0E;
	*PxIE('L') |= 0x0E;
	
	// PM0(TXD3/OUT), PM1(TXD2/OUT), PM2(TXD1/OUT), PM3(TXD0/OUT)
	*PxCR('M') |= 0x0F;
	*PxFRn('M', 8) |= 0x0F;
	*PxIE('M') &= ~0x0F;
	
	// PT4(TX_CLK/IN)
	*PxCR('T') &= ~0x10;
	*PxFRn('T', 8) |= 0x10;
	*PxIE('T') |= 0x10;
}

LOCAL ER netdrv_macctrl_init_macdma( const NetAddr *addr, BOOL promiscuous_mode )
{
	ER ercd;

	/* Enable ETHERNET clock  */
	*CGPROTECT = 0xC1;
	*CGFSYSMENC |= CGFSYSMENC_ETHMA;
	*CGPROTECT = 0x00;
	
	/* Reset EtherMAC Unit A */
	*EMA_MAC_CTRL2 = EMA_MAC_CTRL2_SRST_ENABLE;
	/* NORMAL mode & MII Interface */
	*EMA_MAC_CTRL0 = (EMA_MAC_CTRL0_CLK_LM_NORMAL | EMA_MAC_CTRL0_PHY_INTF_SEL_MII);
	/* Enable RX, TX clock */
	*EMA_MAC_CTRL1 = (EMA_MAC_CTRL1_RXCLKEN | EMA_MAC_CTRL1_TXCLKEN);
	/* Start EtherMAC Unit A */
	*EMA_MAC_CTRL2 = EMA_MAC_CTRL2_SRST_DISABLE;

	/* Configure Ethernet */
	ercd = netdrv_macctrl_init_eth(addr, promiscuous_mode);

	return ercd;
}

/*
 * Initialize the Ethernet module in the CPU
 */
LOCAL ER netdrv_macctrl_init_eth( const NetAddr *addr, BOOL promiscuous_mode )
{
	UH val;
	ER ercd;
	UW duplex;
	UW speed;
	UW start;
	SYSTIM	tim;

	/*********************************************************
	 * MAC configuration
	 *********************************************************/

	/*
	 * TDMA configuration.
	 *  - 
	 */
	/* Software reset */
	*EMA_BUS_MODE = EMA_BUS_MODE_SWR; 
	/* Wait Reset complete. */
	while((*EMA_BUS_MODE) & EMA_BUS_MODE_SWR);
	
	/* Wait for AHB transaction complete */
	while((*ET_AHB_STATUS) & ET_AHB_STATUS_AHSTS);
	
	/* Set Bus mode */
	*EMA_BUS_MODE = (EMA_BUS_MODE_AAL |  EMA_BUS_MODE_PBLx8 | EMA_BUS_MODE_FB |
					 EMA_BUS_MODE_PR_1 | EMA_BUS_MODE_PBL_16 | EMA_BUS_MODE_DSL(0));
	
	/* Initialize Tx Descriptors list */
	netdrv_macctrl_init_txdesc( &txdesc );
	/* Initialize Rx Descriptors list */
	netdrv_macctrl_init_rxdesc( ETH_DMA_RX_DESC_NUM, &rxdesc[0] );
	
	/* Initialize operation mode. */
	*EMA_OPERATION_MODE = (EMA_OPERATION_MODE_TTC_128 | EMA_OPERATION_MODE_RTC_128);
	
	// SetPriorityInt(ETH_VECTOR, ETH_IRQ_PRI, ETH_IRQ_SUBPRI);
	
	netdrv_macctrl_disable_interrupts();
	netdrv_macctrl_clear_interrupts(0xFFFFFFFF);
	netdrv_macctrl_define_inthdr();
	
	*EMA_INTERRUPT_ENABLE = (EMA_INTERRUPT_ENABLE_FBE | EMA_INTERRUPT_ENABLE_RSE |
							 EMA_INTERRUPT_ENABLE_RIE |
							 EMA_INTERRUPT_ENABLE_UNE | EMA_INTERRUPT_ENABLE_OVE |
							 EMA_INTERRUPT_ENABLE_TJE | EMA_INTERRUPT_ENABLE_TSE |
							 EMA_INTERRUPT_ENABLE_TIE);
	netdrv_macctrl_enable_interrupts();
	
	/* Enable DMA */
	*EMA_OPERATION_MODE |= (EMA_OPERATION_MODE_ST | EMA_OPERATION_MODE_SR);

	/*---------------- PHY initialization and configuration -------*/
	/*
	 * Reset the PHY chip.
	 */
	ercd = netdrv_macctrl_write_phy_reg( ETH_PHY_ADDRESS, PHY_MII_REG_CR, 
					     PHY_MII_REG_CR_RESET);
	if( ercd != E_OK ) {
		return ercd;
	}

	/* Wait for the completion of the reset procedure. */
	do {
		ercd = netdrv_macctrl_read_phy_reg( ETH_PHY_ADDRESS, 
						    PHY_MII_REG_CR, &val );
	} while( (val & PHY_MII_REG_CR_RESET) != 0 );

	/* Enable Auto-Negotiation */
	ercd = netdrv_macctrl_write_phy_reg( ETH_PHY_ADDRESS, PHY_MII_REG_CR,
					     PHY_MII_REG_CR_AN_ENABLE );
	if( ercd != E_OK ) {
		return ercd;
	}

	/* Wait until the auto-negotiation will be completed */
	tk_get_tim(&tim);
	start = tim.lo;
	do {
		ercd = netdrv_macctrl_read_phy_reg( ETH_PHY_ADDRESS,
						    PHY_MII_REG_SCSR, &val );
		tk_get_tim(&tim);
	} while( ((val & PHY_MII_REG_PHYSCSR_AN_COMPLETE) == 0) &&
			 (tim.lo - start < PHY_NEGOTIATE_TMOUT) );

	if( tim.lo - start >= PHY_NEGOTIATE_TMOUT ) {
		return E_TMOUT;
	}
	
	if((val & PHY_MII_REG_PHYSCSR_DUPLEX_MASK) == PHY_MII_REG_PHYSCSR_DUPLEX_HALF_10T) {
		duplex = EMA_MAC_CONFIGURATION_DM_HALF;
		speed = EMA_MAC_CONFIGURATION_FES_10M;
	}
	else if((val & PHY_MII_REG_PHYSCSR_DUPLEX_MASK) == PHY_MII_REG_PHYSCSR_DUPLEX_HALF_100T){
		duplex = EMA_MAC_CONFIGURATION_DM_HALF;
		speed = EMA_MAC_CONFIGURATION_FES_100M;
	}
	else if ((val & PHY_MII_REG_PHYSCSR_DUPLEX_MASK) == PHY_MII_REG_PHYSCSR_DUPLEX_FULL_10T){
		duplex = EMA_MAC_CONFIGURATION_DM_FULL;
		speed = EMA_MAC_CONFIGURATION_FES_10M;
	}
	else{
		duplex = EMA_MAC_CONFIGURATION_DM_FULL;
		speed = EMA_MAC_CONFIGURATION_FES_100M;
	}
	
	ercd = netdrv_macctrl_read_phy_reg( ETH_PHY_ADDRESS, PHY_MII_REG_SR, &val );
	
	/* initialize MAC address in ethernet MAC */
	netdrv_macctrl_set_address( addr );
	
	/* Set Frame filter */
	*EMA_MAC_FRAME_FILTER = EMA_MAC_FRAME_FILTER_PCF_0;
	if( promiscuous_mode ){
		*EMA_MAC_FRAME_FILTER |= EMA_MAC_FRAME_FILTER_PR;
	}
	
	/* Set Flow Control */
	while((*EMA_MFLOW_CONTROL) & EMA_MFLOW_CONTROL_FCA);
	*EMA_MFLOW_CONTROL = (EMA_MFLOW_CONTROL_PT(100) | EMA_MFLOW_CONTROL_PLT_28 |
						  EMA_MFLOW_CONTROL_RFE | EMA_MFLOW_CONTROL_TFE);
	
	/* Set Interrupt Mask */
	*EMA_INTERRUPT_MASK = (EMA_INTERRUPT_MASK_TSIM | EMA_INTERRUPT_MASK_PMTIM);
	
	/* Set EtherMAC configuration. */
	*EMA_MAC_CONFIGURATION = (EMA_MAC_CONFIGURATION_SARC_3 | EMA_MAC_CONFIGURATION_IFG_96 | 
							  speed | duplex | EMA_MAC_CONFIGURATION_BL_10 |
							  EMA_MAC_CONFIGURATION_PRELEN_7);
							  
	*EMA_MAC_CONFIGURATION |= (EMA_MAC_CONFIGURATION_RE | EMA_MAC_CONFIGURATION_TE);
	                  
	return E_OK;
}

LOCAL void _mii_mdc_wait(void){
	while((*EMA_MII_ADDRESS) & EMA_MII_ADDRESS_GB);
}

ER netdrv_macctrl_read_phy_reg( UH phyaddr, UH phyreg, UH *phyval )
{
	_mii_mdc_wait();
	
	*EMA_MII_ADDRESS = (EMA_MII_ADDRESS_PA(phyaddr) | EMA_MII_ADDRESS_GR(phyreg) |
						EMA_MII_ADDRESS_CR_DIV_42 | EMA_MII_ADDRESS_GW_READ |
						EMA_MII_ADDRESS_GB);
	_mii_mdc_wait();
	
	*phyval = (UH)(*EMA_MII_DATA);
	
	return E_OK;
}

ER netdrv_macctrl_write_phy_reg( UH phyaddr, UH phyreg, UH phyval )
{
	_mii_mdc_wait();
	
	*EMA_MII_DATA = phyval;
	*EMA_MII_ADDRESS = (EMA_MII_ADDRESS_PA(phyaddr) | EMA_MII_ADDRESS_GR(phyreg) |
						EMA_MII_ADDRESS_CR_DIV_42 | EMA_MII_ADDRESS_GW_WRITE |
						EMA_MII_ADDRESS_GB);
	_mii_mdc_wait();

	return E_OK;
}

void netdrv_macctrl_set_address( const NetAddr *addr )
{
	*EMA_MAC_ADDRESS0_HIGH = (((UW)addr->c[5] << 8) | 
							  (UW)addr->c[4]);
	*EMA_MAC_ADDRESS0_LOW = (((UW)addr->c[3] << 24) |
							 ((UW)addr->c[2] << 16) |
							 ((UW)addr->c[1] << 8)  |
							 (UW)addr->c[0]);
}

LOCAL void netdrv_macctrl_init_txdesc( ETH_DMA_TX_DESC *txdesc )
{
	memset( txdesc, 0, sizeof( ETH_DMA_TX_DESC ) * 4 );
	txdesc->ter = 1;
	
	*EMA_TRANSMIT_DESCRIPTOR_LIST_ADDRESS = (UW) txdesc;
}

LOCAL void netdrv_macctrl_init_rxdesc( INT n, ETH_DMA_RX_DESC *rxdesc )
{	
	memset( rxdesc, 0, sizeof( ETH_DMA_RX_DESC ) * n );
	rxdesc[n - 1].rer = 1;

	*EMA_RECEIVE_DESCRIPTOR_LIST_ADDRESS = (UW)rxdesc;
}


void netdrv_macctrl_start_rx( void )
{
	*EMA_RECEIVE_POLL_DEMAND = 0;
}

void netdrv_macctrl_start_tx(void)
{
	*EMA_TRANSMIT_POLL_DEMAND = 0;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

LOCAL void netdrv_macctrl_clear_interrupts( UW status )
{
	status &= (EMA_STATUS_NIS | EMA_STATUS_AIS | EMA_STATUS_ERI |
				   EMA_STATUS_FBI | EMA_STATUS_ETI | EMA_STATUS_RWT |
				   EMA_STATUS_RPS |  EMA_STATUS_RI |
				   EMA_STATUS_UNF | EMA_STATUS_OVF | EMA_STATUS_TJT |
				   EMA_STATUS_TPS | EMA_STATUS_TI);
	*EMA_STATUS = status;
}

LOCAL void netdrv_macctrl_enable_interrupts( void )
{
	EnableInt(ETH_VECTOR, INTPRI_GROUP(4, 0) );
	
	*EMA_INTERRUPT_ENABLE |= (EMA_INTERRUPT_ENABLE_NIS | EMA_INTERRUPT_ENABLE_AIS);
}

LOCAL void netdrv_macctrl_disable_interrupts( void )
{
	*EMA_INTERRUPT_ENABLE &= ~(EMA_INTERRUPT_ENABLE_NIS | EMA_INTERRUPT_ENABLE_AIS);
	
	DisableInt(ETH_VECTOR);
}

LOCAL ER netdrv_macctrl_define_inthdr( void )
{
	T_DINT dint;
	ER ercd;

	dint.intatr = TA_HLNG;
	dint.inthdr = netdrv_macctrl_inthdr;
	ercd = tk_def_int( ETH_VECTOR, &dint );
	if( ercd < 0 ) {
		return ercd;
	}

	return E_OK;
}

LOCAL void netdrv_macctrl_inthdr( UINT intno )
{
	UW emastatus;
        int ch = 0;

	emastatus = *EMA_STATUS;
	if( (emastatus & EMA_STATUS_AIS) != 0) {
		if( emastatus & EMA_STATUS_OVF ) {
			netdrv_stat_inc_overrun(ch);
		}
	}
	
	if( (emastatus & EMA_STATUS_TI ) != 0 ) {
		netdrv_macctrl_txint_inthdr();
	}
	if( (emastatus & EMA_STATUS_RI ) != 0 ) {
		netdrv_macctrl_rxint_inthdr();
	}
	
	netdrv_macctrl_clear_interrupts(emastatus);
}

LOCAL void netdrv_macctrl_txint_inthdr( void )
{
        int ch = 0;
	/* Increment a counter. */
	netdrv_stat_inc_txint(ch);

	/* Clears */
	txdesc.buffer1 = (UW)NULL;
	txdesc.tbs1 = 0;

	/* Notifies completing a transmission. */
	tk_set_flg( netmaccb.flgid, ETHER_FLGPTN_SEND );
}

LOCAL void netdrv_macctrl_rxint_inthdr( void )
{
        int ch = 0;
	/* Increment a counter. */
	netdrv_stat_inc_rxint(ch);

	/* Notifies a packet reception. */
	tk_set_flg( netmaccb.flgid, ETHER_FLGPTN_RECV );
}

#if 0
void netdrv_macctrl_errint_inthdr( void )
{
	UW bit;
	bit = in_w( TK_ENET_EIR );
	out_w( TK_ENET_EIR,  ~(TK_ENET_EIR_TXF_BIT | TK_ENET_EIR_RXF_BIT) );

	if( bit & TK_ENET_EIR_MII_BIT ) {
		/* MII interrupt */
		tk_set_flg( enetcb.flgid, ETHER_FLGPTN_MII );
	}
}
#endif

ER netdrv_macctrl_get_recv_event( NetEvent * event )
{
	INT i;
        int ch = 0;
	/* Check a parameter. */
	if( !event ) {
		return E_PAR;
	}
	
	if(netmaccb.bufnum < 1){
		return E_NOEXS;
	}

	i = netmaccb.next_rxbd;
	if( rxdesc[i].own != 0 ) {
		return E_NOEXS;
	}

	if( rxdesc[i].oe != 0 ) {
		netdrv_stat_inc_overrun(ch);
		event->len = 0;
	}
	else if( rxdesc[i].es != 0 ) {
		event->len = 0;
	}
	else {
		event->len = rxdesc[i].fl;
	}
	event->buf = (VP) rxdesc[i].buffer1;

	netdrv_macctrl_reset_rxdesc( &rxdesc[i] );
	netdrv_macctrl_incr_next_rxbd();

	return E_OK;
}

LOCAL void netdrv_macctrl_reset_rxdesc( ETH_DMA_RX_DESC * desc )
{
	INT intsts;
	
	if( desc == NULL ) {
		return;
	}

	if( isQueEmpty( &netmaccb.freeq ) == FALSE ) {
		/* Assign a buffer from the free queue to the descriptor. */
		desc->own = 1;
		desc->buffer1 = (UW)QueRemoveNext( &netmaccb.freeq );
		desc->rbs1 = netmaccb.max_bufsz;
		if( (*EMA_STATUS) & EMA_STATUS_RU ) {
			// Restart Receive polling.
			*EMA_STATUS = EMA_STATUS_RU;
			netdrv_macctrl_start_rx();
		}
	}
	else {
		/* There is no receive buffer. */
		desc->own = 0;
		desc->buffer1 = (UW)NULL;
		desc->rbs1 = 0ul;
		
		DI(intsts);
		netmaccb.bufnum--;
		EI(intsts);
	}
}

LOCAL void netdrv_macctrl_incr_next_rxbd( void )
{
	netmaccb.next_rxbd++;
	if( netmaccb.next_rxbd >= ETH_DMA_RX_DESC_NUM ) {
		netmaccb.next_rxbd = 0;
	}
}

ER netdrv_macctrl_set_rxbuf_size( INT size )
{
	netmaccb.max_bufsz = size;

	return E_OK;
}

ER netdrv_macctrl_enqueue_rxbuf( VP ptr )
{
	QUEUE * que;
	UW ptr_addr;
	INT i;
	INT intsts;

	ptr_addr = (UW)ptr;
	if( ptr_addr & 0x0000000F ) {
		return E_PAR;
	}

	if( netmaccb.bufnum < ETH_DMA_RX_DESC_NUM ) {
		/* Set ptr into a receive buffer descriptor. */
		i = netmaccb.next_rxbd + netmaccb.bufnum;
		if( i >= ETH_DMA_RX_DESC_NUM ) {
			i -= ETH_DMA_RX_DESC_NUM;
		}
		rxdesc[i].buffer1 = (UW)ptr;
		rxdesc[i].rbs1 = netmaccb.max_bufsz;
		rxdesc[i].own = 1;
		
		if( (*EMA_STATUS) & EMA_STATUS_RU ) {
			// Restart Receive polling.
			*EMA_STATUS = EMA_STATUS_RU;
			netdrv_macctrl_start_rx();
		}
		
		DI(intsts);
		netmaccb.bufnum++;
		EI(intsts);
	}
	else {
		/* Enqueue ptr into a free queue. */
		que = (QUEUE *)ptr;
		QueInsert( que, &netmaccb.freeq );
	}

	return E_OK;
}

ER netdrv_macctrl_request_send( const void * buf, INT len )
{       
        int ch = 0;
	/* Check parameters. */
	if( !buf || len < 0 ) {
		return E_PAR;
	}

	/* Check a link status. */
	if( !netdrv_macctrl_check_link() ) {
		return E_IO;
	}

	/* Check a buffer size.*/
	if( len > ENET_MAX_DATA_LENGTH ) {
		netdrv_stat_inc_txerr(ch);
		return E_PAR;
	}

	if( txdesc.own != 0 ) {
		return E_BUSY;
	}

	txdesc.ic = 1;
	txdesc.ls = 1;
	txdesc.fs = 1;
	txdesc.cic = 0;
	txdesc.dc = 0;
	txdesc.ter = 1;
	txdesc.tch = 0;
	txdesc.dp = 0;
	txdesc.ttse = 0;
	txdesc.tbs2 = 0;
	txdesc.tbs1 = len;
	txdesc.buffer1 = (UW)buf;
	txdesc.buffer2 = (UW)NULL;
	txdesc.own = 1;
	
	if( (*EMA_STATUS) & EMA_STATUS_TU) {
		*EMA_STATUS = EMA_STATUS_TU;
		netdrv_macctrl_start_tx();
	}
	
	return E_OK;
}

BOOL netdrv_macctrl_check_link( void )
{
	ER ercd;
	UH val;
	
	ercd = netdrv_macctrl_read_phy_reg( ETH_PHY_ADDRESS, PHY_MII_REG_SR,
					    &val );
	if( ercd < 0 ) {
		return FALSE;
	}
  
	if( (val & PHY_MII_REG_SR_LINK_STATUS) != 0 ) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

ER netdrv_macctrl_clear_rxbuf( void )
{
	NetEvent event;
	INT i;
	ER ercd;

	/* Discard all received data. */
	do {
		ercd = netdrv_macctrl_get_recv_event( &event );
	} while( ercd == E_OK );
	
	if(netmaccb.bufnum > 0){
		*EMA_MAC_CONFIGURATION &= ~EMA_MAC_CONFIGURATION_RE;
		/* Wait for Reveive stop. */
		while((*EMA_STATUS & EMA_STATUS_RS_MASK) != EMA_STATUS_RS_STOP);
		netdrv_macctrl_incr_next_rxbd();
	}

	/* Unregister buffers. */
	for( i=0; i<ETH_DMA_RX_DESC_NUM; i++ ) {
		rxdesc[i].own = 0;
		rxdesc[i].buffer1 = 0;
		rxdesc[i].rbs1 = 0;
	}

	/* Release all buffers. */
	QueInit( &netmaccb.freeq );
	netmaccb.bufnum = 0;

	return E_OK;
}
