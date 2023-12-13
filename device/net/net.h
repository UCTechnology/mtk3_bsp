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
 *	net.h
 *	Ethernet device driver
 *	Driver local definition
 */

#ifndef __DEV_NET_H__
#define __DEV_NET_H__

#include <tk/tkernel.h>
#include "../common/drvif/mgdrvif.h"
#include "../include/dev_net.h"
#include "net_cnf.h"

#define DEVDEF_NET_ETHER_PATH_(a)	#a
#define DEVDEF_NET_ETHER_PATH(a)	DEVDEF_NET_ETHER_PATH_(a)
#define DEVDEF_NET_ETHER_SYSDEP()	DEVDEF_NET_ETHER_PATH(sysdepend/TARGET_CPU_DIR/net_ether.h)
#include DEVDEF_NET_ETHER_SYSDEP()

/*
 * Device name. "Neta" "Netb" ...
 */
#define NET_DEVNM DEVCNF_NET_DEVNAME

/*
 * The maximum number of requests.
 */
#ifndef NET_MAXREQQ
#define NET_MAXREQQ (4)
#endif

/*
 * The driver task priority.
 */
#ifndef NET_TASKPRI
#define NET_TASKPRI (10)
#endif

/*
 * The stack size of the driver task.
 */
#ifndef NET_TASKSTKSZ
#define NET_TASKSTKSZ (2048)
#endif

/*
 * MII/RMII mode.
 *
 * TRUE and FALSE coresponds to MII mode and RMII mode respectively.
 */
#ifndef NETDRV_MII_MODE
#define NETDRV_MII_MODE (FALSE)
#endif

/*
 * Promiscuous mode.
 */
#ifndef NETDRV_PROMISCUOUS_MODE
#define NETDRV_PROMISCUOUS_MODE (FALSE)
#endif

/*
 * Time-out value for auto-negotiation in milliseconds.
 */
#ifndef NETDRV_AN_TMOUT
#define NETDRV_AN_TMOUT (5000)
#endif 

#define NETDRV_USERCMD_STOP (23)

/*
 * Main function for registering/unregistering the network device driver.
 *
 * @param ac  The number of arguments.
 * @param av  Array of arguments.
 *
 * This function takes MAC address as an argument.
 *
 * @code
 *   UB av[] = { "00-11-22-33-44-55", "00-11-22-33-44-56" };
 *   ER ercd;
 *
 *   ercd = NetDrvEntry( 2, av );
 *   if( ercd < 0 ) {
 *      // Error
 *   }
 * @endcode
 */
EXPORT ER NetDrvEntry( INT ac, UB *av[] );

#endif /* __DEV_NET_H__ */
