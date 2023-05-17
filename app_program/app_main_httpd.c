/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 * Sample User Program
 * ---------------------------------------------------------
 * 
 * Entry routine for the user application.
 * At this point, Initialize and start the user application.
 *
 * Entry routine is called from the initial task for Kernel,
 * so system call for stopping the task should not be issued 
 * from the contexts of entry routine.
 * We recommend that:
 * (1)'usermain()' only generates the user initial task.
 * (2)initialize and start the user application by the user
 * initial task.
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "lwip/api.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "netif/etharp.h"
#include "netif/tknetif.h"
#include "lwip/apps/httpd.h"

#define USE_NETA        0

#ifdef USE_ETHER_SHIELD
#define USE_NETB        1
#else
#undef USE_NETA
#define USE_NETA        1
#define USE_NETB        0
#endif 

#define HWADDR "00-00-00-00-00-02"
#define HWADDR2 "00-00-00-00-00-03"

IMPORT ER NetDrvEntry( INT ac, UB *av[] );

LOCAL ER InitializeLAN( void )
{
    ER ercd;
    UB *args[] = {
        (UB *) HWADDR,
#if USE_NETB
        (UB *) HWADDR2
#endif
    };

    tm_putstring( (UB*)"\nInitializing network devices...\n" );
    /* Register the device driver. */
    ercd = NetDrvEntry( sizeof(args) / sizeof(UB *), args );
    if( ercd < 0 ) {
        tm_putstring( (UB*)"Initialization failed.\n" );
        tm_putstring( (UB*)"Cannot start the application.\n" );
        return -1;
    }
    
#if USE_NETA
    if( args[0] != NULL) {
        if ((ercd & (1 << 0)) != 0) {
            tm_putstring( (UB*)"Neta NIC Link is not ready. " );
            tm_putstring( (UB*)"Please check the cable is connected.\n" );
        }
        else {
            tm_putstring( (UB*)"Neta NIC Link is UP.\n" );
        }
    }
#endif
#if USE_NETB    
    if( args[1] != NULL) {
        if ((ercd & (1 << 1)) != 0) {
            tm_putstring( (UB*)"Netb NIC Link is not ready. " );
            tm_putstring( (UB*)"Please check the cable is connected.\n" );
        }
        else {
            tm_putstring( (UB*)"Netb NIC Link is UP.\n" );
        }
    }
#endif

    return ercd;
//    return E_OK;
}

#if USE_NETA
struct netif tknetif;
#endif
#if USE_NETB
struct netif tknetif_b;
#endif

static void tcpip_init_done(void *arg)
{
    tk_wup_tsk((ID) arg);
}

/*
 * Entry routine for the user application.
 * At this point, Initialize and start the user application.
 *
 * Entry routine is called from the initial task for Kernel,
 * so system call for stopping the task should not be issued 
 * from the contexts of entry routine.
 * We recommend that:
 * (1)'usermain()' only generates the user initial task.
 * (2)initialize and start the user application by the user
 * initial task.
 */
EXPORT  INT usermain( void )
{
    ER ercd;
#if USE_NETA
    ip_addr_t ipaddr, netmask, gw;
#endif
#if USE_NETB
    ip_addr_t ipaddr_b, netmask_b, gw_b;
#endif
    W mscnt, link_stat, dhcp_stat = 0;
    
    /* Initialize the LAN driver. */
    ercd = InitializeLAN();
    if( ercd < 0 ) {
        return ercd;
    }
    link_stat = (W)ercd;

    tcpip_init(tcpip_init_done, (void *) tk_get_tid());
    tk_slp_tsk(TMO_FEVR);

#if USE_NETA
    mscnt = 0;
    if((link_stat & 0x01) == 0){
	    IP4_ADDR(&gw, 0, 0, 0, 0);
    	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
    	IP4_ADDR(&netmask, 0, 0, 0, 0);
    	
    	netif_add(&tknetif, &ipaddr, &netmask, &gw, NULL, tknetif_init, tcpip_input);
    	netif_set_default(&tknetif);
    	netif_set_up(&tknetif);
    	dhcp_start(&tknetif);
    	tm_printf((UB *) "dhcp_start(&tknetif)\n");
    }
    while((link_stat & 0x01) == 0){
        tk_dly_tsk(DHCP_FINE_TIMER_MSECS);
        //dhcp_fine_tmr();
        if(tknetif.ip_addr.addr && (tknetif.ip_addr.addr != ipaddr.addr)){
            tm_printf((UB *) "Neta DHCP result:\n");
            tm_printf((UB *) "        IP Address: %d.%d.%d.%d\n", 
                      ip4_addr1(&tknetif.ip_addr), 
                      ip4_addr2(&tknetif.ip_addr), 
                      ip4_addr3(&tknetif.ip_addr), 
                      ip4_addr4(&tknetif.ip_addr));
            ipaddr.addr = tknetif.ip_addr.addr;
            tm_printf((UB *) "       Subnet Mask: %d.%d.%d.%d\n", 
                      ip4_addr1(&tknetif.netmask), 
                      ip4_addr2(&tknetif.netmask), 
                      ip4_addr3(&tknetif.netmask), 
                      ip4_addr4(&tknetif.netmask));
            tm_printf((UB *) "   Default Gateway: %d.%d.%d.%d\n", 
                      ip4_addr1(&tknetif.gw), 
                      ip4_addr2(&tknetif.gw), 
                      ip4_addr3(&tknetif.gw), 
                      ip4_addr4(&tknetif.gw));
            dhcp_stat |= 0x01;
            break;
        }
        mscnt += DHCP_FINE_TIMER_MSECS;
        if (mscnt >= DHCP_COARSE_TIMER_SECS*1000) {
            //dhcp_coarse_tmr();
            tm_printf((UB *) "Neta DHCP timeout.\n");
            break;
            //mscnt = 0;
        }
    }
#endif
#if USE_NETB
    mscnt = 0;
    if((link_stat & 0x02) == 0){
	    IP4_ADDR(&gw_b, 0, 0, 0, 0);
    	IP4_ADDR(&ipaddr_b, 0, 0, 0, 0);
    	IP4_ADDR(&netmask_b, 0, 0, 0, 0);
    	
    	netif_add(&tknetif_b, &ipaddr_b, &netmask_b, &gw_b, NULL, tknetif_netb_init, tcpip_input);
    	netif_set_up(&tknetif_b);
    
    	dhcp_start(&tknetif_b);
    	tm_printf((UB *) "dhcp_start(&tknetif_b)\n");
    }
    while((link_stat & 0x02) == 0){
        tk_dly_tsk(DHCP_FINE_TIMER_MSECS);
        //dhcp_fine_tmr();
        if(tknetif_b.ip_addr.addr && (tknetif_b.ip_addr.addr != ipaddr_b.addr)){
            tm_printf((UB *) "Netb DHCP result:\n");
            tm_printf((UB *) "        IP Address: %d.%d.%d.%d\n", 
                      ip4_addr1(&tknetif_b.ip_addr), 
                      ip4_addr2(&tknetif_b.ip_addr), 
                      ip4_addr3(&tknetif_b.ip_addr), 
                      ip4_addr4(&tknetif_b.ip_addr));
            ipaddr_b.addr = tknetif_b.ip_addr.addr;
            tm_printf((UB *) "       Subnet Mask: %d.%d.%d.%d\n", 
                      ip4_addr1(&tknetif_b.netmask), 
                      ip4_addr2(&tknetif_b.netmask), 
                      ip4_addr3(&tknetif_b.netmask), 
                      ip4_addr4(&tknetif_b.netmask));
            tm_printf((UB *) "   Default Gateway: %d.%d.%d.%d\n", 
                      ip4_addr1(&tknetif_b.gw), 
                      ip4_addr2(&tknetif_b.gw), 
                      ip4_addr3(&tknetif_b.gw), 
                      ip4_addr4(&tknetif_b.gw));
        	out_w(GPIO_ODR(B), (in_w(GPIO_ODR(B)))|(1<<0));		// green on
            dhcp_stat |= 0x02;
            break;
        }
        mscnt += DHCP_FINE_TIMER_MSECS;
        if (mscnt >= DHCP_COARSE_TIMER_SECS*1000) {
            //dhcp_coarse_tmr();
            tm_printf((UB *) "Netb DHCP timeout.\n");
            break;
            //mscnt = 0;
        }
    }
#endif /* USE_NETB */

    /* Initialize httpserver */
    if(dhcp_stat & 0x03){
    	tm_printf((UB *) "httpd_init() Start.\n");
    	httpd_init();
    }

    do{
        tk_dly_tsk(1000);
    } while(1);
}
