/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the ADBUN-M4GR.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_M4GR

#include <tk/tkernel.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"
#if DEV_SER_ENABLE
/*
 *	ser_rxbuf.c
 *	Serial communication device driver
 *	Interrupt processing for TMPM4GR
 */

IMPORT	void* knl_memset( void *s, int c, SZ n );
IMPORT	void* knl_memcpy( void *dst, const void *src, SZ n );

/*----------------------------------------------------------------------*/
/*	The structure of the receive buffer is as follows.		*/
/*									*/
/*	|<---------------------size-------------------->|		*/
/*	|						|		*/
/*	+-----------------------------------------------+		*/
/*	|	ABCDEFGHIJKLMNOPQRSTUVWX		|		*/
/*	+^------^-----------------------^---------------+^		*/
/*	 |	|			|		 |		*/
/*	top	rp			wp		end		*/
/*----------------------------------------------------------------------*/
typedef	struct RxBufferInfomation {
	W	size;		/* Buffer size (bytes)	*/
	W	free;		/* free size in buf	*/
	UB	*top;		/* buffer top		*/
	UB	*end;		/* buffer end + 1	*/
	UB	*wp;		/* write pointer	*/
	UB	*rp;		/* read pointer		*/
} RXBINFO;

LOCAL	RXBINFO	rxBuf[DEV_SER_UNITNM];			/* Rx Buffer Infomation	*/
LOCAL	UB	rxBuffer[DEV_SER_UNITNM][RX_BUFFER_SIZE];/* Entity of Rx Buffer */
EXPORT	ER	initRxBuf(W port)
{
	knl_memset( rxBuffer[port], 0, RX_BUFFER_SIZE );

	rxBuf[port].size = RX_BUFFER_SIZE;
	rxBuf[port].free = RX_BUFFER_SIZE;
	rxBuf[port].top  = rxBuffer[port];
	rxBuf[port].end	 = rxBuf[port].top + RX_BUFFER_SIZE;
	rxBuf[port].wp	 = rxBuf[port].top;
	rxBuf[port].rp	 = rxBuf[port].top;

	return E_OK;
}

EXPORT	ER	deleteRxBuf(W port)
{
	knl_memset( &rxBuf[port], 0, sizeof(RXBINFO) );
	return E_OK;
}

EXPORT	W	resizeRxBuf(W port, W size)
{
	return initRxBuf(port);
}

EXPORT	W	getRxBufFreeSize(W port)
{
	return rxBuf[port].free;
}

EXPORT	W	getDataSizeInRxBuf(W port)
{
	return rxBuf[port].size - rxBuf[port].free;
}


/*----------------------------------------------------------------------*/
/*	SAVE RX DATA TO RX BUFFER					*/
/*	--------------------------------------------------------	*/
/*	* Save received data directly from BRR to RxBuf.		*/
/*		UARTx_BRR --> RxBuf[]					*/
/*	* It is possible to call this function only from the interrupt	*/
/*	  handler (uartInthdr).						*/
/*	* It is necessary to have free space at the time of calling 	*/
/*	  this function.						*/
/*----------------------------------------------------------------------*/
EXPORT	W	setRxBufFromDR(W port, _UW *data, W sz)
{		/* data = UARTx_RBR : The address is fixed! */
	UB	*wp;
	UB	*rp;
	UB	*end;
	W	free;
	INT	cnt;		/* Data size written in rx buffer */

	rp   = rxBuf[port].rp;
	wp   = rxBuf[port].wp;
	end  = rxBuf[port].end;
	free = rxBuf[port].free;
	cnt = 0;

	while( (cnt < sz)&&(cnt < free) ){
		cnt++;
		*wp++ = (UB)*data;
		if( end <= wp ){
			wp = rxBuf[port].top;	/* Back to the top */
		}else if( wp == rp ){
			break;			/* buffer is full */
		}
	}

	rxBuf[port].wp	  = wp;
	rxBuf[port].free -= cnt;

	return cnt;
}

/*----------------------------------------------------------------------*/
/*	GET RX DATA FROM RX BUFFER					*/
/*	--------------------------------------------------------	*/
/*	* Get received data from RxBuf.					*/
/*		RxBuf[] --> data					*/
/*	* Call this function from the task.				*/
/*----------------------------------------------------------------------*/
EXPORT	W	getDataFromRxBuf(W port, UB *data, W maxSize)
{
	UB	*wp;
	UB	*rp;
	W	sz;		/* Size to process at once		*/
	W	cnt;		/* Number of data read from Rx Buffer.	*/
	UINT	intsts;

	if( rxBuf[port].free == RX_BUFFER_SIZE ){
		return 0;		/* NO DATA */
	}

	DI(intsts);
	rp = rxBuf[port].rp;
	wp = rxBuf[port].wp;
	EI(intsts);

	/*--------------------------------------------------------------*/
	/*	>>> INVERT <<< --> This is pre process to make NORMAL.	*/
	/*	+-------------------------------+			*/
	/*	|HIJKLMN		ABCDEFGH|			*/
	/*	+^------^---------------^-------+^			*/
	/*	top	wp		rp	 end			*/
	/*--------------------------------------------------------------*/
	if( wp <= rp ){			/* wp == rp : Buffer Full	*/
		sz = rxBuf[port].end - rp;
		if( maxSize < sz ){
			knl_memcpy( data, rp, sz = maxSize );
			rp += sz;
			/* all copied! */
		}else{
			knl_memcpy( data, rp, sz );
			data	+= sz;
			maxSize	-= sz;
			rp = rxBuf[port].top;
			/* some data are remaind */
		}
		cnt = sz;
	}else{
		cnt = 0;
	}

	/*--------------------------------------------------------------*/
	/*	>>> NORMAL <<<						*/
	/*	+-------------------------------+			*/
	/*	|	ABCDEFGHIJKLMNOP	|			*/
	/*	+^------^---------------^-------+^			*/
	/*	top	rp		wp	 end			*/
	/*--------------------------------------------------------------*/
	if( rp < wp ){
		sz = wp - rp;
		if( sz > maxSize ){
			sz = maxSize;
		}
		knl_memcpy( data, rp, sz );
		rp  += sz;
		cnt += sz;
	}

	DI(intsts);
	rxBuf[port].rp	  = rp;
	rxBuf[port].free += cnt;
	sz = rxBuf[port].free;
	EI(intsts);

	if( sz > 0 ){
		enableRxRelatedInterrupt(port);
	}

	return cnt;
}

#endif	/* DEV_SER_ENABLE */
#endif	/* CPU_M4GR */
