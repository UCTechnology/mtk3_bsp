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


#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#include <basic.h>
#include <tk/tkernel.h>

Inline UH swap_2byte( UH val )
{
	return ((val & 0xff) << 8) | ((val >> 8) & 0xff);
}

Inline UW swap_4byte( UW val )
{
	return ( ((val & 0x000000ffUL) << 24) | ((val & 0x0000ff00UL) << 8) |
		 ((val & 0xff000000UL) >> 24) | ((val & 0x00ff0000UL) >> 8) );
}


#if BIGENDIAN

/*
 * When the byte order is big-endian.
 */

Inline UH htobe16( UH val )
{
	return val;
}

Inline UH htole16( UH val )
{
	return swap_2byte( val );
}

Inline UH be16toh( UH val )
{
	return val;
}

Inline UH le16toh( UH val )
{
	return swap_2byte( val );
}

Inline UW htobe32( UW val )
{
	return val;
}

Inline UW htole32( UW val )
{
	return swap_4byte( val );
}

Inline UW be32toh( UW val )
{
	return val;
}

Inline UW le32toh( UW val )
{
	return swap_4byte( val );
}

#else /* BIGENDIAN */

/*
 * Whan the byte order is little endian.
 */

Inline UH htobe16( UH val )
{
	return swap_2byte( val );
}

Inline UH htole16( UH val )
{
	return val;
}

Inline UH be16toh( UH val )
{
	return swap_2byte( val );
}

Inline UH le16toh( UH val )
{
	return val;
}

Inline UW htobe32( UW val )
{
	return swap_4byte( val );
}

Inline UW htole32( UW val )
{
	return val;
}

Inline UW be32toh( UW val )
{
	return swap_4byte( val );
}

Inline UW le32toh( UW val )
{
	return val;
}

#endif /* BIGENDIAN */

#endif /* __ENDIAN_H__ */
