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

#define USE_NETA        1

#define HWADDR "00-00-00-00-00-02"

IMPORT ER NetDrvEntry( INT ac, UB *av[] );

#define LED_1		(0x00000010)			// ADBUN-M4NR (PORT W4 )
#define LED_2		(0x00000020)			// ADBUN-M4NR (PORT W5 )
#define LED_3		(0x00000040)			// ADBUN-M4NR (PORT W6 )
#define LED_4		(0x00000080)			// ADBUN-M4NR (PORT W7 )

LOCAL ER InitializeLAN( void )
{
    ER ercd;
    UB *args[] = {
        (UB *) HWADDR
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
    return ercd;
//    return E_OK;
}

#if USE_NETA
struct netif tknetif;
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

    // Enable Clock for PORT W
    *CGPROTECT = 0xC1;
    *CGFSYSMENB |= CGFSYSMENB_PW;			// LED1,2,3,4
    *CGPROTECT = 0x00;

    // Setup PORT W for LED1,2,3,4
    *GPIO_FR3(W) &= ~(LED_1|LED_2|LED_3|LED_4);         // DISABLE Function3
    *GPIO_FR5(W) &= ~(LED_1|LED_4);                     // DISABLE Function5
    *GPIO_FR7(W) &= ~(LED_1|LED_2|LED_3|LED_4);         // DISABLE Function7
    *GPIO_IE(W)  &= ~(LED_1|LED_2|LED_3|LED_4);         // DISABLE Input
    *GPIO_CR(W)  |=  (LED_1|LED_2|LED_3|LED_4);         // ENABLE Output
    // LED Control
    *GPIO_DATA(W) &= ~(LED_1|LED_2|LED_3|LED_4);        // LED1,2,3,4 all off
    *GPIO_DATA(W) |=  LED_1;

    /* Initialize the LAN driver. */
    ercd = InitializeLAN();
    if( ercd < 0 ) {
        return ercd;
    }
    link_stat = (W)ercd;

    *GPIO_DATA(W) &= ~LED_1;
    *GPIO_DATA(W) |= LED_2;

    tcpip_init(tcpip_init_done, (void *) tk_get_tid());
    tk_slp_tsk(TMO_FEVR);

    *GPIO_DATA(W) &= ~LED_2;
    *GPIO_DATA(W) |= LED_3;

    mscnt = 0;
    if((link_stat & 0x01) == 0){
        IP4_ADDR(&gw, 0, 0, 0, 0);
    	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
    	IP4_ADDR(&netmask, 0, 0, 0, 0);
    	
    	netif_add(&tknetif, &ipaddr, &netmask, &gw, NULL, tknetif_init, tcpip_input);
    	netif_set_default(&tknetif);
    	netif_set_up(&tknetif);
    	tm_printf((UB *) "dhcp_start(&tknetif)\n");
    	dhcp_start(&tknetif);
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

            *GPIO_DATA(W) &= ~LED_3;
            *GPIO_DATA(W) |= LED_4;
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

    do{
        tk_dly_tsk(1000);
    } while(1);
}
