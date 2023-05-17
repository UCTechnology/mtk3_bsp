/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 *----------------------------------------------------------------------
 *    Modifications: Porting to micro T-Kernel 2.0
 *    Modified by UC Technology at 2022/06/15.
 *
 *    UCT micro T-Kernel Version 2.00.58
 *    Copyright (c) 2011-2022 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include <tk/tkernel.h>
#include <sys/sysdef.h>
#include <tk/syslib.h>
#include <dev_net.h>

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include <string.h>

#include "lwip/etharp.h"
#include "lwip/ethip6.h"

/* Define those to better describe your network interface. */
#define IFNAME0 '0'
#define IFNAME1 '\0'

#define ETHER_DRV_MAX_RBUFF    4
#define ETHER_DRV_BUFF_SIZE    1520

#define ROUNDUP(x)	(((x) + (ETHER_BUF_ALIGNMENT-1)) & ~(ETHER_BUF_ALIGNMENT-1))

#define DMAC_MPL_SIZE	(32 * 1024)

struct mac_filter_item{
  struct mac_filter_item *next;
  ip_addr_t grp_addr;
};

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct tknetif {
  ID ethdevid;
  ID rxmbfid;
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
  UW multicast_group_cnt;
  struct mac_filter_item *multicast_list;
  ID mplid;
  UB *drv_buf[ETHER_DRV_MAX_RBUFF];
  UB *output_buf;
};

#if LWIP_IPV6
/* The allnodes (ff01::1, ff02::1) group. */
const static ip6_addr_t allnodes[2] = {
	{ { PP_HTONL(0xff010000), PP_HTONL(0), PP_HTONL(0), PP_HTONL(1) } IPADDR6_ZONE_INIT }, 
	{ { PP_HTONL(0xff020000), PP_HTONL(0), PP_HTONL(0), PP_HTONL(1) } IPADDR6_ZONE_INIT }};
#endif

/* Forward declarations. */
static void rx_task(UINT stacd, void *exinf);

static ER low_level_set_multicast_address(struct tknetif *ethernetif)
{
  NetAddr *faddr = mem_malloc(sizeof(NetAddr) * ethernetif->multicast_group_cnt);
  struct mac_filter_item *p = ethernetif->multicast_list;
  ER ercd = E_OK;
  W i, asize;
  	
  if(faddr != NULL){
    for(i = 0; i < ethernetif->multicast_group_cnt; i++){
      if(p == NULL){
        break;
      }
#if LWIP_IPV4
      if(IP_IS_V4(&p->grp_addr)){
        faddr[i].c[0] = 0x01;
        faddr[i].c[1] = 0x00;
        faddr[i].c[2] = 0x5E;
        faddr[i].c[3] = (ip_2_ip4(&p->grp_addr)->addr & 0x00007F00) >> 8;
        faddr[i].c[4] = (ip_2_ip4(&p->grp_addr)->addr & 0x00FF0000) >> 16;
        faddr[i].c[5] = (ip_2_ip4(&p->grp_addr)->addr & 0xFF000000) >> 24;
      }
#endif
#if LWIP_IPV6
      if(IP_IS_V6(&p->grp_addr)){
        faddr[i].c[0] = 0x33;
        faddr[i].c[1] = 0x33;
        faddr[i].c[2] = (ip_2_ip6(&p->grp_addr)->addr[3] & 0x000000FF);
        faddr[i].c[3] = (ip_2_ip6(&p->grp_addr)->addr[3] & 0x0000FF00) >> 8;
        faddr[i].c[4] = (ip_2_ip6(&p->grp_addr)->addr[3] & 0x00FF0000) >> 16;
        faddr[i].c[5] = (ip_2_ip6(&p->grp_addr)->addr[3] & 0xFF000000) >> 24;
      }
#endif
      p = p->next;
    }

    ercd = tk_swri_dev(ethernetif->ethdevid, DN_SET_MCAST_LIST, faddr, i, &asize);
    mem_free(faddr);
  }
  else{
    return ERR_MEM;
  }
  
  return ercd;
}

#if LWIP_IPV4 && LWIP_IGMP
static err_t 
tknetif_igmp_mac_filter(struct netif *netif,
                        const ip4_addr_t *group, 
                        enum netif_mac_filter_action action)
{
  struct tknetif *ethernetif = netif->state;
  struct mac_filter_item *p, *item;
  ER ercd = E_OK;
  W asize;
	
  // Check parameter.
  if(netif == NULL || group == NULL){
    return ERR_ARG;
  }
  
  // Add or Delete ipv4 group address entry.
  if(action == NETIF_DEL_MAC_FILTER){
    p = ethernetif->multicast_list;
    asize = ethernetif->multicast_group_cnt;

    if( p != NULL && IP_IS_V4(&p->grp_addr) && (memcmp(ip_2_ip4(&p->grp_addr), group, sizeof(ip4_addr_t)) == 0) ){
      ethernetif->multicast_list = p->next;
      mem_free(p);
      ethernetif->multicast_group_cnt --;
    }
    else if(p != NULL){
      while(p->next != NULL){
        if(IP_IS_V4(&p->next->grp_addr) && memcmp(ip_2_ip4(&p->next->grp_addr), group, sizeof(ip4_addr_t)) == 0){
          item = p->next;
          p->next = p->next->next;
          mem_free(item);
          ethernetif->multicast_group_cnt --;
          break;
        }
        p = p->next;
      }
    }

    if(asize == ethernetif->multicast_group_cnt){
      return ERR_ARG;
    }
  }
  else if(action == NETIF_ADD_MAC_FILTER){
    item = mem_malloc(sizeof(struct mac_filter_item));
    if(item != NULL){
      memcpy( ip_2_ip4(&item->grp_addr), group, sizeof(ip4_addr_t) );
      IP_SET_TYPE(&item->grp_addr, IPADDR_TYPE_V4);
      item->next = NULL;
      if(ethernetif->multicast_list == NULL){
        ethernetif->multicast_list = item;
      }
      else{
        p = ethernetif->multicast_list;
        while(p->next != NULL){
          p = p->next;
        }
        p->next = item;
      }
      ethernetif->multicast_group_cnt ++;
    }
    else{
      return ERR_MEM;
    }
  }
  else{
    return ERR_ARG;
  }
  
  // Modify MAC filter setting.
  if(ethernetif->multicast_list == NULL){
    ercd = tk_swri_dev(ethernetif->ethdevid, DN_SET_MCAST_LIST, NULL, 0, &asize);
  }
  else{
    ercd = low_level_set_multicast_address(ethernetif);
  }
  
  if(ercd < E_OK){
    return ERR_IF;
  }
  else{
    return ERR_OK;
  }
}
#endif // LWIP_IPV4 && LWIP_IGMP

#if LWIP_IPV6 && LWIP_IPV6_MLD
static 
err_t tknetif_mld_mac_filter(struct netif *netif,
                             const ip6_addr_t *group, 
                             enum netif_mac_filter_action action)
{
  struct tknetif *ethernetif = netif->state;
  struct mac_filter_item *p, *item;
  ER ercd = E_OK;
  W asize;
	
  // Check parameter.
  if(netif == NULL || group == NULL){
    return ERR_ARG;
  }
  
  // Add or Delete ipv4 group address entry.
  if(action == NETIF_DEL_MAC_FILTER){
    p = ethernetif->multicast_list;
    asize = ethernetif->multicast_group_cnt;

    if( p != NULL && IP_IS_V6(&p->grp_addr) && (memcmp(ip_2_ip6(&p->grp_addr), group, sizeof(ip6_addr_t)) == 0) ){
      ethernetif->multicast_list = p->next;
      mem_free(p);
      ethernetif->multicast_group_cnt --;
    }
    else if(p != NULL){
      while(p->next != NULL){
        if(IP_IS_V6(&p->next->grp_addr) && memcmp(ip_2_ip6(&p->next->grp_addr), group, sizeof(ip6_addr_t)) == 0){
          item = p->next;
          p->next = p->next->next;
          mem_free(item);
          ethernetif->multicast_group_cnt --;
          break;
        }
        p = p->next;
      }
    }

    if(asize == ethernetif->multicast_group_cnt){
      return ERR_ARG;
    }
  }
  else if(action == NETIF_ADD_MAC_FILTER){
    item = mem_malloc(sizeof(struct mac_filter_item));
    if(item != NULL){
      memcpy( ip_2_ip6(&item->grp_addr), group, sizeof(ip6_addr_t) );
      IP_SET_TYPE(&item->grp_addr, IPADDR_TYPE_V6);
      item->next = NULL;
      if(ethernetif->multicast_list == NULL){
        ethernetif->multicast_list = item;
      }
      else{
        p = ethernetif->multicast_list;
        while(p->next != NULL){
          p = p->next;
        }
        p->next = item;
      }
      ethernetif->multicast_group_cnt ++;
    }
    else{
      return ERR_MEM;
    }
  }
  else{
    return ERR_ARG;
  }
  
  // Modify MAC filter setting.
  if(ethernetif->multicast_list == NULL){
    ercd = tk_swri_dev(ethernetif->ethdevid, DN_SET_MCAST_LIST, NULL, 0, &asize);
  }
  else{
    ercd = low_level_set_multicast_address(ethernetif);
  }
  
  if(ercd < E_OK){
    return ERR_IF;
  }
  else{
    return ERR_OK;
  }
}
#endif //LWIP_IPV6 && LWIP_IPV6_MLD

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param ch     the net interface channel to be initialized.
 * @param netif  the already initialized lwip network interface structure
 *               for this ethernetif
 */
static void
low_level_init(int ch, struct netif *netif)
{
  NetAddr macaddr;
  NetRxBufSz bufsz;
  T_CMBF cmbf;
  struct tknetif *ethernetif = netif->state;
  W i, asize;
  void *ptr;
  ER ercd;
  T_CMPL cmpl;
  UB devnm[] = {'n', 'e', 't', 'a' + ch, 0};

  /* Create memory pool for receive buffer. */
  cmpl.exinf = 0;
  cmpl.mplatr = TA_TFIFO;
  cmpl.mplsz = DMAC_MPL_SIZE;
#if defined(EVAL_STM32H743I)
  /* WARNING! STM32H7's ETHDMA module CAN NOT access the DTCM & ITCM. 
     So receive Buffer MUST be allocated from other internal memories. */
  cmpl.mplatr |= TA_USERBUF;
  /* SRAM3: 0x30044000 - 0x3004BFFF (0x30040000 - 0x30043FFF Used by the net driver.)*/
  cmpl.bufptr = (void *) 0x30044000;
#elif defined(TWR_K60D100M) || defined(TWR_K60F120M) || defined(TWR_K70F120M)
  /* For Kinetis CPUs, use IMEM (0x1FFF0000 - 0x1FFF7FFF) as memory pool for receive buffer. */
  cmpl.mplatr |= TA_USERBUF;
  cmpl.bufptr = (void *) 0x1FFF0000;
#elif defined(NUCLEO_STM32H723ZGT6)
  /* WARNING! STM32H7's ETHDMA module CAN NOT access the DTCM & ITCM. 
     So receive Buffer MUST be allocated from other internal memories. */
  cmpl.mplatr |= TA_USERBUF;
  /* AXI SRAM: 0x24004000 - 0x2400BFFF(0x24000000 - 24003FFF Used by the net driver.)  */
  cmpl.bufptr = (void *) 0x24004000;
 #elif defined(NUCLEO_STM32H742ZIT6) || defined(NUCLEO_STM32H743ZIT6)
  /* WARNING! STM32H7's ETHDMA module CAN NOT access the DTCM & ITCM. 
     So receive Buffer MUST be allocated from other internal memories. */
  cmpl.mplatr |= TA_USERBUF;
  /* SRAM1: 0x30000000 - 0x30007FFF */
  cmpl.bufptr = (void *) 0x30000000;
#endif
  ethernetif->mplid = tk_cre_mpl(&cmpl);
  LWIP_ASSERT("Error: cannot create memory pool.\n", ethernetif->mplid >= E_OK );
  
  ercd = tk_get_mpl(ethernetif->mplid, ETHER_DRV_BUFF_SIZE + ETHER_BUF_ALIGNMENT, (void **) &ptr, TMO_POL);
  LWIP_ASSERT("Error: cannot allocate memory from memory pool.\n", ercd >= E_OK );
  ethernetif->output_buf = (void *) ROUNDUP((UW) ptr);
  
  /* set MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;

  /* maximum transfer unit */
  netif->mtu = 1500;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;
 
  /* Do whatever else is needed to initialize interface. */ 
  ethernetif->ethdevid = tk_opn_dev(devnm, TD_UPDATE);
  LWIP_ASSERT("Error: cannot open net device.\n", ethernetif->ethdevid > 0);
  
  ethernetif->multicast_group_cnt = 0;
  ethernetif->multicast_list = NULL;

#if LWIP_IPV6
  /* Trick: lwIP assumed the allnodes (ff01::1, ff02::1) group is always received 
	 by the netif layer.*/
  tknetif_mld_mac_filter(netif, &allnodes[0], NETIF_ADD_MAC_FILTER), 
  tknetif_mld_mac_filter(netif, &allnodes[1], NETIF_ADD_MAC_FILTER), 
#endif

#if LWIP_IPV4 && LWIP_IGMP
  netif->igmp_mac_filter = tknetif_igmp_mac_filter;
#endif //LWIP_IPV4 && LWIP_IGMP
  
#if LWIP_IPV6 && LWIP_IPV6_MLD
  netif->mld_mac_filter = tknetif_mld_mac_filter;
#endif //LWIP_IPV6 && LWIP_IPV6_MLD
  
  tk_srea_dev(ethernetif->ethdevid, DN_NETADDR, &macaddr, sizeof(NetAddr), &asize);
  memcpy(netif->hwaddr, macaddr.c, netif->hwaddr_len);
  
  cmbf.exinf = NULL;
  cmbf.mbfatr = TA_TFIFO;
  cmbf.bufsz = sizeof( NetEvent ) * 8;
  cmbf.maxmsz = sizeof( NetEvent );
  ethernetif->rxmbfid = tk_cre_mbf(&cmbf);
  LWIP_ASSERT("Error: cannot create message buffer for ethernet driver.\n", ethernetif->rxmbfid > 0);
  ercd = tk_swri_dev(ethernetif->ethdevid, DN_NETEVENT, &ethernetif->rxmbfid, sizeof(ID), &asize);
  LWIP_ASSERT("Error: cannot set message buffer id.\n", ercd >= E_OK);
  
  
  bufsz.minsz = 60;
  bufsz.maxsz = ETHER_DRV_BUFF_SIZE;
  ercd = tk_swri_dev(ethernetif->ethdevid, DN_NETRXBUFSZ, &bufsz, sizeof( bufsz ), &asize);
  LWIP_ASSERT("Error: cannot set net receive buffer size.\n", ercd >= E_OK);
  
  /* Set receive buffer to net driver. */
  for(i = 0; i < ETHER_DRV_MAX_RBUFF; i++){
    ercd = tk_get_mpl(ethernetif->mplid, ETHER_DRV_BUFF_SIZE + ETHER_BUF_ALIGNMENT, (void **) &ethernetif->drv_buf[i], TMO_POL);
    if(ercd >= E_OK){
      ptr = (void *) ROUNDUP((UW) ethernetif->drv_buf[i]);
      ercd = tk_swri_dev(ethernetif->ethdevid, DN_NETRXBUF, &ptr, sizeof( void * ), &asize);
      if(ercd < E_OK){
        LWIP_DEBUGF(LWIP_DBG_LEVEL_WARNING | LWIP_DBG_ON, ((UB *)"Net receive buffer set error.\n"));
      }
    }
  }
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  struct tknetif *ethernetif = netif->state;
  struct pbuf *q;
  W asize, dlen = 0;
  ER ercd;
  
#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    memcpy(ethernetif->output_buf + dlen, q->payload, q->len);
    dlen += q->len;
  }

  ercd = tk_swri_dev(ethernetif->ethdevid, 0, ethernetif->output_buf, dlen, &asize);
  if(ercd < E_OK){
    LWIP_DEBUGF(LWIP_DBG_LEVEL_WARNING | LWIP_DBG_ON, ((UB *)"Data transmission error.\n"));
  }
  
#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  
  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  struct tknetif *ethernetif = netif->state;
  struct pbuf *p;
  NetEvent event;
  u16_t len;
  ER ercd;
  W asize;

  /* Obtain the size of the packet and put it into the "len"
     variable. */
  ercd = tk_rcv_mbf( ethernetif->rxmbfid, &event, TMO_FEVR);
  if(ercd < E_OK || event.len <= 0){
    return NULL;
  }
  
  len = event.len;

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* Read the entire packet into the pbuf. */
    pbuf_take(p, event.buf, event.len);

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.recv);
  } else {
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }
  tk_swri_dev(ethernetif->ethdevid, DN_NETRXBUF, &event.buf, sizeof( void * ), &asize);

  return p;  
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void
tknetif_input(struct netif *netif)
{
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
  case ETHTYPE_IPV6:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ((UB *)"ethernetif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }
}

static void 
rx_task(UINT stacd, void *exinf)
{ 
  do{
    tknetif_input(exinf);
  }while(1);
}

static err_t 
tknetif_init_impl(int ch, struct netif *netif)
{
  struct tknetif *ethernetif;
  T_CTSK ctsk;
  ER ercd;

  LWIP_ASSERT("netif != NULL", (netif != NULL));
    
  ethernetif = mem_malloc(sizeof(struct tknetif));
  if (ethernetif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ((UB *) "ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, NETIF_LINK_SPEED);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0 + ch;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
#if LWIP_IPV4
  netif->output = etharp_output;
#endif //LWIP_IPV4
#if LWIP_IPV6
  netif->output_ip6 = ethip6_output;
#endif //LWIP_IPV6
  netif->linkoutput = low_level_output;
  
  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
  
  /* initialize the hardware */
  low_level_init(ch, netif);
  
  ctsk.exinf = netif;
  ctsk.tskatr = TA_HLNG | TA_RNG0;
  ctsk.task = rx_task;
  ctsk.itskpri = NETIF_THREAD_PRIO;
  ctsk.stksz = DEFAULT_THREAD_STACKSIZE;
  ercd = tk_cre_tsk(&ctsk);
  LWIP_ASSERT("Error: Cannot create ethernet receive task.\n", ercd >= E_OK);
  
  tk_sta_tsk(ercd, 0);

  return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
tknetif_init(struct netif *netif)
{
  return tknetif_init_impl(0, netif);
}

err_t
tknetif_netb_init(struct netif *netif)
{
  return tknetif_init_impl(1, netif);
}
	


