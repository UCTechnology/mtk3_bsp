/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
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
 * Author: Simon Goldschmidt
 *
 *----------------------------------------------------------------------
 *    Modifications: Porting to micro T-Kernel 2.0
 *    Modified by UC Technology at 2013/3/25.
 *
 *    UCT micro T-Kernel DevKit tuned for Kinetis Version 2.00.02
 *    Copyright (c) 2011-2014 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */
#ifndef __TKLWIP_OPTS_H__
#define __TKLWIP_OPTS_H__

#define SYS_LIGHTWEIGHT_PROT            1
#define NO_SYS                          0

/**
 * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
 */
#define LWIP_RAW                        1

/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    1

/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET                     1

#if LWIP_SOCKET
#define LWIP_PROVIDE_ERRNO
#endif // LWIP_SOCKET

/**
 * LWIP_SO_SNDRCVTIMEO_NONSTANDARD==1: SO_RCVTIMEO/SO_SNDTIMEO take an int
 * (milliseconds, much like winsock does) instead of a struct timeval (default).
 */
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 1

/**
 * LWIP_SO_SNDTIMEO==1: Enable send timeout for sockets/netconns and
 * SO_SNDTIMEO processing.
 */
#define LWIP_SO_SNDTIMEO                1

/**
 * LWIP_SO_RCVTIMEO==1: Enable receive timeout for sockets/netconns and
 * SO_RCVTIMEO processing.
 */
#define LWIP_SO_RCVTIMEO                1

/**
 * LWIP_IPV4==1: Enable IPv4
 */
#define LWIP_IPV4                       1

/**
 * LWIP_IPV6==1: Enable IPv6
 */
#define LWIP_IPV6                       0

/**
 * LWIP_DHCP==1: Enable DHCP module.
 */
#define LWIP_DHCP                       1

#define DHCP_DEBUG                      LWIP_DBG_OFF

#define LWIP_AUTOIP                     1

/**
 * LWIP_TCP==1: Turn on TCP.
 */
#define LWIP_TCP                        1

/**
 * LWIP_UDP==1: Turn on UDP.
 */
#define LWIP_UDP                        1

/**
 * LWIP_DNS==1: Turn on DNS module. UDP must be available for DNS
 * transport.
 */
#define LWIP_DNS                        1

/**
 * LWIP_ARP==1: Enable ARP functionality.
 */
#define LWIP_ARP                        1

/**
 * LWIP_ICMP==1: Enable ICMP module inside the IP stack.
 * Be careful, disable that make your product non-compliant to RFC1122
 */
#define LWIP_ICMP                       1

#define NO_STDCLIB                      1

#define ERRNO                           1

#define LWIP_IGMP						1

/**
 * PPP_SUPPORT==1: Enable PPP.
 */
#define PPP_SUPPORT                     1

/**
 * PPPOE_SUPPORT==1: Enable PPP Over Ethernet
 */
#define PPPOE_SUPPORT                   1

/**
 * PPPOS_SUPPORT==1: Enable PPP Over Serial
 */
#define PPPOS_SUPPORT                   1

#define PAP_SUPPORT                     1
#define CHAP_SUPPORT                    1
//#define MSCHAP_SUPPORT                  1

#define PPP_DEBUG                       LWIP_DBG_OFF

#define LWIP_SNMP                       1

#if LWIP_SNMP
#define MIB2_STATS                      1
#endif

/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 */
#define MEM_ALIGNMENT                   4


#define ETH_PAD_SIZE                    2

#define LWIP_DEBUG                      LWIP_DBG_ON

#define TCPIP_DEBUG                     LWIP_DBG_OFF

#define NETIF_THREAD_PRIO               12

#define DEFAULT_THREAD_STACKSIZE        2048
#define DEFAULT_THREAD_PRIO             15

#define TCPIP_THREAD_STACKSIZE          2048
#define TCPIP_THREAD_PRIO               11

#define SLIPIF_THREAD_STACKSIZE         2048
#define SLIPIF_THREAD_PRIO              NETIF_THREAD_PRIO

#define PPP_THREAD_STACKSIZE            2048
#define PPP_THREAD_PRIO                 NETIF_THREAD_PRIO

#define TCP_MSS                         536

#define MEM_SIZE                        16000
#define TCP_SND_QUEUELEN                40
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN
#define TCP_SND_BUF                     (12 * TCP_MSS)
#define TCP_WND                         (10 * TCP_MSS)

/* ---------- Statistics options ---------- */

#define LWIP_STATS              1
#define LWIP_STATS_DISPLAY      1

#if LWIP_STATS
#define LINK_STATS              1
#define IP_STATS                1
#define ICMP_STATS              1
#define IGMP_STATS              1
#define IPFRAG_STATS            1
#define UDP_STATS               1
#define TCP_STATS               1
#define MEM_STATS               1
#define MEMP_STATS              1
#define PBUF_STATS              1
#define SYS_STATS               1
#endif /* LWIP_STATS */

/* Minimal changes to opt.h required for etharp unit tests: */
#define ETHARP_SUPPORT_STATIC_ENTRIES   1

#define ETHER_BUF_ALIGNMENT		(16)

#endif /* __LWIPOPTS_H__ */
