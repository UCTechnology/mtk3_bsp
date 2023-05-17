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
 *----------------------------------------------------------------------
 *    Modifications: Porting to micro T-Kernel 2.0
 *    Modified by UC Technology at 2014/5/10.
 *
 *    UCT micro T-Kernel DevKit tuned for Kinetis Version 2.00.02
 *    Copyright (c) 2011-2014 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

#include <tk/tkernel.h>
#include <tk/syslib.h>

#include "lwip/sys.h"
#include "lwip/def.h"

/**
 * Opens a serial device for communication.
 * 
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
	ER ercd;
	sio_fd_t siofd = Kmalloc(sizeof(struct _sio_fd_t));
	
	ercd = CreateLock(&siofd->lock, NULL);
	
	if(ercd >= E_OK){
		siofd->devid = tk_opn_dev((UB*) PPPOS_DEVNM, (TD_READ|TD_WRITE));
	}
	else{
		Kfree(siofd);
		siofd = NULL;
	}
	
	return siofd;
}

/**
 * Sends a single character to the serial device.
 * 
 * @param c character to send
 * @param fd serial device handle
 * 
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
	SZ asize;
	
	if(fd != NULL){
		if(fd->devid < E_OK){
			Lock(&fd->lock);
			fd->devid = tk_opn_dev((UB*)PPPOS_DEVNM, (TD_READ|TD_WRITE));
			Unlock(&fd->lock);
		}

		tk_swri_dev(fd->devid, 0, &c, 1, &asize);
	}
}

/**
 * Receives a single character from the serial device.
 * 
 * @param fd serial device handle
 * 
 * @note This function will block until a character is received.
 */
u8_t sio_recv(sio_fd_t fd)
{
	u8_t	c = 0;
	SZ asize;
	
	if(fd != NULL){
		if(fd->devid < E_OK){
			Lock(&fd->lock);
			fd->devid = tk_opn_dev((UB*)PPPOS_DEVNM, (TD_READ|TD_WRITE));
			Unlock(&fd->lock);
		}
		
		tk_srea_dev(fd->devid, 0, &c, 1, &asize);
	}
	
	return c;
}

/**
 * Reads from the serial device.
 * 
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 * 
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
	SZ asize = 0;
	ER ercd;
	
	if(fd != NULL){
		if(fd->devid < E_OK){
			Lock(&fd->lock);
			fd->devid = tk_opn_dev((UB*)PPPOS_DEVNM, (TD_READ|TD_WRITE));
			Unlock(&fd->lock);
		}

		/* Non-Blocking read */
		ercd = tk_srea_dev(fd->devid, 1, data, len, &asize);
		if ( ercd >= 0 && asize <= 0 ) {
			/* Blocking read 1 bytes */
			ercd = tk_srea_dev(fd->devid, 0, data, 1, &asize);
			if ( ercd >= E_OK && len > 1 ) {
				/* Non-Blocking read more bytes */
				tk_srea_dev(fd->devid, 1, data + 1,
							len - 1, &asize);
				asize += 1;
			}
		}

		if(ercd < E_OK){
			asize = 0;
		}
	}
	
	return asize;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
	SZ asize = 0;
	ER ercd;
	
	if(fd != NULL){
		if(fd->devid < E_OK){
			Lock(&fd->lock);
			fd->devid = tk_opn_dev((UB*)PPPOS_DEVNM, (TD_READ|TD_WRITE));
			Unlock(&fd->lock);
		}

		/* Non-Blocking read */
		ercd = tk_srea_dev(fd->devid, 1, data, len, &asize);

		if(ercd < E_OK){
			asize = 0;
		}
	}
	
	return asize;
}

/**
 * Writes to the serial device.
 * 
 * @param fd serial device handle
 * @param data pointer to data to send
 * @param len length (in bytes) of data to send
 * @return number of bytes actually sent
 * 
 * @note This function will block until all data can be sent.
 */
u32_t sio_write(sio_fd_t fd, u8_t *data, u32_t len)
{
	SZ asize = 0;
	ER ercd;
	
	if(fd != NULL){
		if(fd->devid < E_OK){
			Lock(&fd->lock);
			fd->devid = tk_opn_dev((UB*)PPPOS_DEVNM, (TD_READ|TD_WRITE));
			Unlock(&fd->lock);
		}
	
		ercd = tk_swri_dev(fd->devid, 0, data, len, &asize);
		
		if(ercd < E_OK){
			asize = 0;
		}
	}
	
	return asize;
}

/**
 * Aborts a blocking sio_read() call.
 * 
 * @param fd serial device handle
 */
void sio_read_abort(sio_fd_t fd)
{
	if(fd != NULL){
		Lock(&fd->lock);
		tk_cls_dev(fd->devid, 0);
		fd->devid = -1;
		Unlock(&fd->lock);
	}
}
