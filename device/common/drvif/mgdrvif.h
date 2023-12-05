/*
 *----------------------------------------------------------------------------
 *    Driver Sample
 *----------------------------------------------------------------------------
 *    Description of license
 *    Original Source:   Copyright (C) 2006 by T-Engine Forum. 
 *    This software is distributed under the T-Engine Public License.
 *    Refer to T-Engine Public License at http://www.t-engine.org for use
 *    conditions and redistribution conditions. 
 *    Modification and redistribution are allowed to this software, in
 *    accordance with T-Engine Public License. 
 *    Therefore, the modification may be added to the program distributed by
 *    T-Engine forum.
 *    The original source is available for download from the download page at
 *    http://www.t-engine.org.
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the STM3221G-EVAL.
 *    Modified by UC Technology at 2011/10/28.
 *    Modifications: Adapted to the MCBTMPM362 EVAL BD.
 *    Modified by UC Technology at 2012/01/31.
 *    Modifications: Adapted to the TWR-K60N512-KEIL.
 *    Modified by UC Technology at 2012/02/17.
 *    Modifications: Adapted to the EBDTMPM440 EVAL BD.
 *    Modified by UC Technology at 2012/09/27.
 *    Modified by UC Technology at 2015/06/04.
 *
 *    UCT micro T-Kernel Version 2.00.12
 *    Copyright (c) 2011-2015 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	gdrvif.h (device)
 *
 *	General-purpose device driver I/F layer
 *
 *	Used for all general devices that process in order of request.
 *	The device driver holds one or more request processing tasks
 *	and process the requests asynchronously.
 *
 *			Driver I/F layer
 *	-------------------------------------------------
 *		  |			  ^
 *		  |			  |
 *	open/close/abort/event	      Accept/Reply
 *		  |			  |
 *		  v			  |
 *	+---------------------+  +-------------------------+
 *	|  Process function   |  | Request processing task |
 *	|		      |  |	(one or more)	   |
 *	+---------------------+  +-------------------------+
 *
 *	Except where otherwise stated, the functions defined here cannot be
 *	called from the task-independent part or when dispatch or interrupt
 *	disabled.
 */

#ifndef	__DEVICE_GDRVIF_H__
#define	__DEVICE_GDRVIF_H__

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Driver I/F access handle
 */
typedef struct GeneralDriverInterface *	GDI;

/*
 * Maximum number of 'maxreqq' in GDefDev since this implementation can
 * notify at most 16 different device requets.
 */
#define GDRVIF_MAX_MAXREQQ (16)

/*
 * Device register information
 */
typedef struct {
	void	*exinf;		/* Extended information (may be optional) */
	UB	devnm[L_DEVNM+1]; /* Physical device name */
	UH	maxreqq;	/* Maximum queued requests (1 or more) */
	ATR	drvatr;		/* Driver attributes */
	ATR	devatr;		/* Device attributes */
	INT	nsub;		/* Number of subunits */
	INT	blksz;		/* Unique data block size (-1 = unknown) */

/*
 * Processing function (set to NULL if processing function not required)
 *	In all cases, processing must be completed promptly without
 *	switching to wait mode on an irregular basis. With the
 *	exception of abort, the processing function calls only one task
 *	at a time, because the driver I/F exercises exclusive control.
 *	Although abort can be called during execution of other
 *	processing functions, multiple abort functions will never be
 *	called simultaneously.
 *	Processing functions are executed as quasi task portions in the
 *	request task(application task) context. Since processing
 *	functions operate in the request task context,in the event of a
 *	change in task priority or other change, it is necessary to
 *	return them to their original state before returning from the
 *	processing function.
 *
 *	abort stops a devreq request being processed by a request
 *	process task. The abort function has limited power to call GDI
 *	functions. Only the functions listed below can be called from
 *	the abort function: GDI_devid(), GDI_exinf(), GDI_ddev(),
 *	GDI_Send Cmd()
 */
	ER  (*open )( ID devid, UINT omode, GDI );
	ER  (*close)( ID devid, UINT option, GDI );
	ER  (*abort)( T_DEVREQ *devreq, GDI );
	INT (*event)( INT evttyp, void *evtinf, GDI );
} GDefDev;

/*
 * Driver attributes
 *	If TDA_LOCKREQ is specified, the input/output buffer
 *	(T_DEVREQ.buf) area is locked (made resident) in the
 *	driver I/F.
 */
#ifndef	TDA_OPENREQ
#define	TDA_OPENREQ	0x0001	/* Each time open/close */
#endif
#define	TDA_LOCKREQ	0x8000	/* Address space lock required */
#define	TDA_LIMITEDREQ	0x4000	/* Limit request queue by request type */

/*
 * Device register
 *	Registers the device in accordance with the ddev registration
 *	information.
 *	Initial device information is returned in idev. If idev = NULL,
 *	information is notreturned.
 *	The driver I/F access handle is returned in GDI.
 * Update
 *	Updates the GDI device registration in accordance with the ddev
 *	registration information . The device name (devnm) and maximum number
 *	of queued requests (maxreqq) cannot (and must not) be changed.
 *	Accepted requests are held in a queue. Any requests not yet accepted
 *	will be aborted.
 *	The update process does not alter the device ID.
 * Delete
 *	Deletes the GID device registration.
 */
IMPORT ER GDefDevice( GDefDev *ddev, T_IDEV *idev, GDI* );	/* define */
IMPORT ER GRedefDevice( GDefDev *ddev, GDI );			/* re-define */
IMPORT ER GDelDevice( GDI );					/* delete */

/*
 * Get information from GDI
 *	These functions can be called from the task-independent part and when
 *	dispatch or interrupt is disabled.
 */
IMPORT ID GDI_devid( GDI );		/* Physical device ID */
IMPORT void *GDI_exinf( GDI );		/* Extended information (exinf) */
IMPORT const GDefDev* GDI_ddev( GDI );	/* Registration information */

/*
 * Accept request
 *	Getes one request from the accepted requests queue.
 *	If there are no requests in the queue, it switches to wait mode
 *	until a request is received.
 *	In acpptn, the value obtained from DEVREQ_ACPPTN() is specified
 *	in OR in terms of the accepted request type (TDC_READ/TDC_WRITE)
 *	or the user command pattern. Although ordinary requests
 *	(TDC_READ/TDC_WRITE) have priority over user commands, an
 *	ordinary request and a user command may on occasion be accepted
 *	at the same time.
 *	tmout specifies the timeout interval in milliseconds. TMO_POL
 *	and TMO_FEVR may also be specified. The return value is either
 *	the request pattern or an error.
 *
 *	[Ordinary requests]
 *	The return value is the pattern that indicates the accepted
 *	request type.
 *	The accepted request is returned in *devreq.
 *
 *	[User commands]
 *	The return value is the pattern that indicates the user commands
 *	accepted.Where multiple types of user commands are waiting,
 *	all user commands specified in acpptn are accepted together,
 *	and returned in an OR pattern. NULL is returned in *devreq.
 *
 *	[Ordinary requests and user commands accepted at the same time]
 *	The return value is an OR pattern with both the accepted
 *	ordinary requests and the accepted user commands.
 *	The accepted ordinary requests are returned in *devreq.
 *
 *	[Timeout or error]
 *	The return value is the error code, or E_TMOUT in the event
 *	of a timeout.
 *	*devreq is undefined.
 *
 *	exinf of the accepted request (T_DEVREQ) cannot be changed.
 *	A buf area check (ChkSpace) has been implemented in the
 *	driver I/F.
 *	Reply to user command (GDI_Reply) is not required.
 *
 *	Normally, a request will not be accepted until processing
 *	of the previous request has been completed and the result
 *	returned. However, multiple requests may be accepted and
 *	processed simultaneously.
 *	Multiple requests may be processed either in parallel,
 *	whereby multiple request processing tasks each execute their
 *	own GDI_Accept(), or simultaneously, whereby a single processing
 *	task performs multiple repetitions of GDI_Accept().
 *	The order in which the results are returned does not need
 *	to be the same as the order in which the requests were accepted.
 */

IMPORT INT GDI_Accept( T_DEVREQ **devreq, INT acpptn, TMO tmout, GDI );

/* cmd = TDC_READ || TDC_WRITE || user commands (16 - 23) */
#define	DEVREQ_ACPPTN(cmd)	( ((cmd)<8) ? 1 << (cmd) : 1 << (cmd-8) )

#define	DRP_READ	DEVREQ_ACPPTN(TDC_READ)		/* Read  (1) */
#define	DRP_WRITE	DEVREQ_ACPPTN(TDC_WRITE)	/* Write (2) */
#define	DRP_NORMREQ	( DRP_READ|DRP_WRITE )		/* Ordinary request */
#define	DRP_USERCMD	0xff00			/* User command */

/*
 * Extended accept request (Specified attribute/unique data)
 *	DRP_DREAD	Read unique data
 *	DRP_DWRITE	Write unique data
 *	DRP_AREAD	Read attribute data
 *	DRP_AWRITE	Write attribute data
 *
 *	DRP_DREAD  | DRP_AREAD	= DRP_READ
 *	DRP_DWRITE | DRP_AWRITE = DRP_WRITE
 *
 *	These option cannot use with DRP_READ/DRP_WRITE.
 */
#define DRP_ADSEL_OFS	(4)	/* Offset value for DRP_ADSEL (bits) */
#define	DRP_ADSEL	0x0010	/* set unique, attribute data */
#define	DRP_DREAD	( DRP_ADSEL | DEVREQ_ACPPTN(TDC_READ) )
#define	DRP_DWRITE	( DRP_ADSEL | DEVREQ_ACPPTN(TDC_WRITE) )
#define	DRP_AREAD	( DRP_ADSEL | DEVREQ_ACPPTN(TDC_READ  + DRP_ADSEL_OFS) )
#define	DRP_AWRITE	( DRP_ADSEL | DEVREQ_ACPPTN(TDC_WRITE + DRP_ADSEL_OFS) )

#define	DRP_REQMASK	( DRP_ADSEL | DRP_NORMREQ | (DRP_NORMREQ << DRP_ADSEL_OFS) )

/*
 * Issue user command
 *	Issues the user command specified in cmd.
 *	Only values in the range 16 - 23 may be specified in cmd.
 *	Issued user commands are accepted by GDI_Accept().
 *	Where user commands are waiting to be accepted by GDI_Accept(),
 *	only one of each command type is allowed. If the same command is
 *	issued several times, only one can be retained; accordingly, the
 *	command already waiting at GDI_Accept() will be deleted.
 *	Note that GDI_SendCmd() is only for issuing user commands;
 *	as a result, it returns withoutwaiting to be accepted by
 *	GDI_Accept().
 *
 *	User commands are generally used to enable release from
 *	GDI_Accept() waiting status at any time.
 */
IMPORT ER GDI_SendCmd( INT cmd, GDI );

/*
 * Request response
 *	Returns the processing result for the request accepted
 *	by GDI_Accept().
 *	Task issues GDI_Accept() and task issues GDI_Reply() do not
 *	have to be the same.
 */
IMPORT void GDI_Reply( T_DEVREQ*, GDI );

#define DEBUG_PRINT(x)		PRINTF(#x)
#define PRINTF(fmt, ...)	tm_printf((UB*) fmt, ##__VA_ARGS__)	

#ifdef __cplusplus
}
#endif
#endif /* __DEVICE_GDRVIF_H__ */
