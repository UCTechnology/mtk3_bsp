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

#include "net_util.h"

LOCAL INT netdrv_str2int( const char *str, INT n )
{
	INT ret, i;

	ret = 0;
	for( i=0; i<n; i++ ) {
		ret *= 16;
		if( *str >= '0' && *str <= '9' ) {
			ret += *str - '0';
		}
		else if( *str >= 'A' && *str <= 'F' ) {
			ret += *str - 'A' + 10;
		}
		else if( *str >= 'a' && *str <= 'f' ) {
			ret += *str - 'a' + 10;
		}
		else {
			return -1;
		}
		str++;
	}

	return ret;
}

ER netdrv_str2addr( const char *str_addr, NetAddr *addr )
{
	INT i, n;

	for( i=0; i<6; i++ ) {
		n = netdrv_str2int( str_addr, 2 );
		if( n < 0 ) {
			return E_PAR;
		}
		addr->c[i] = n;
		str_addr += 2;
		if( i < 5 && (*str_addr != ':' && *str_addr != '-') ) {
			return E_PAR;
		}
		str_addr++;
	}

	return E_OK;
}
