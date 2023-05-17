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
#include "lwip/apps/tftp_server.h"

#define USE_NETA        1

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

#define MAX_DUMMY_FILE		2
#define MAX_FNAME_LEN		32
#define MAX_FILE_SIZE		256

struct dummy_file{
	int is_free;
	char fname[MAX_FNAME_LEN];
	int rw_pos;
	int size;
	char contents[MAX_FILE_SIZE];
} ftp_files[MAX_DUMMY_FILE];

static void* dummy_open(const char* fname, const char* mode, u8_t write){
	int i;
	struct dummy_file *p = NULL;

	if(fname == NULL || strlen(fname) > (MAX_FNAME_LEN -1)){
		return NULL;
	}

	for(i = 0; i < MAX_DUMMY_FILE; i++){
		if(ftp_files[i].is_free == 0 &&
		   memcmp(fname, ftp_files[i].fname, strlen(ftp_files[i].fname) + 1) == 0){
			p = &ftp_files[i];
		   	p->rw_pos = 0;
		}
		else if(ftp_files[i].is_free != 0 && p == NULL && write != 0){
			p = &ftp_files[i];
		}
	}

	if(p!= NULL && p->is_free != 0){
		memcpy(p->fname, fname, strlen(fname) + 1);
		p->is_free = 0;
		p->size = 0;
		p->rw_pos = 0;
	}

	return p;
}

static void dummy_close(void* handle){
	return;
}

static int dummy_read(void* handle, void* buf, int bytes){
	struct dummy_file *fp = (struct dummy_file *) handle;
	int len;

	if(fp == NULL){
		return 0;
	}

	len = fp->size - fp->rw_pos;
	if(len > bytes){
		len = bytes;
	}

	if(len > 0 && buf != NULL){
		memcpy(buf, fp->contents + fp->rw_pos, len);
		fp->rw_pos += len;
	}

	return len;
}

static int dummy_write(void* handle, struct pbuf* p){
	struct dummy_file *fp = (struct dummy_file *) handle;
	int len;

	if(fp == NULL){
		return 0;
	}

	len = p->tot_len;
	len = (MAX_FILE_SIZE - fp->rw_pos) > len ? len : (MAX_FILE_SIZE - fp->rw_pos);

	if(len > 0){
		pbuf_copy_partial(p, fp->contents + fp->rw_pos, len, 0);
		fp->rw_pos += len;
		fp->size = fp->rw_pos;
	}

	return len;
}

static struct tftp_context sample_server = {
	dummy_open,
	dummy_close,
	dummy_read,
	dummy_write
};

LOCAL ER InitializeLAN( void )
{
	ER ercd;
	UB *args[] = {
		(UB *) HWADDR
#if USE_NETB
		, (UB *) HWADDR2
#endif
	};

	tm_putstring( (UB*)"\nInitializing network devices...\n" );
	/* Register the device driver. */
	ercd = NetDrvEntry( sizeof(args) / sizeof(UB *), args );
	tm_printf((UB *) "NetDrvEntry() end. ercd=%d\n", ercd);
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
EXPORT	INT	usermain( void )
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

	// Initialize ftp files
#define FILE1_NAME			"memo.txt"
#define FILE1_CONTENTS			"A simple text file for tftp server test.\n"
	memcpy(ftp_files[0].fname, FILE1_NAME, sizeof(FILE1_NAME));
	memcpy(ftp_files[0].contents, FILE1_CONTENTS, sizeof(FILE1_CONTENTS));
	ftp_files[0].rw_pos = 0;
	ftp_files[0].size = sizeof(FILE1_CONTENTS) - 1;
	ftp_files[0].is_free = 0;

	ftp_files[1].is_free = 1;

	ercd = tftp_init(&sample_server);
	tm_printf((UB *) "tftp_init_server() end. rc = %d\n", ercd);

    do{
        tk_dly_tsk(1000);
    } while(1);

}
