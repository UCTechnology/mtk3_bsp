/*
 *----------------------------------------------------------------------
 *    UCT micro T-Kernel Version 2.00.12
 *
 *    Copyright (c) 2013-2015 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 *
 *    Released by UC Technology (http://www.uctec.com/) at 2013/06/10.
 *    Modified by UC Technology at 2015/06/04.
 *
 *----------------------------------------------------------------------
 */

#ifndef __NETDRV_UTIL_H__
#define __NETDRV_UTIL_H__

#include <tk/tkernel.h>
#include <../include/dev_net.h>

/*
 * Converts a string to a MAC address.
 */
ER netdrv_str2addr( const char *str_addr, NetAddr *addr );

#endif /* __NETDRV_UTIL_H__ */
