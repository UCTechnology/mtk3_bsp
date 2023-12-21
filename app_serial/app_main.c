/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 *    Released by UC Technology (http://www.uctec.com/) at 2012/02/17.
 *    Modified by UC Technology at 2012/05/10.
 *    Modified by UC Technology at 2013/01/07.
 *    Modified by UC Technology at 2011/10/28.
 *    Modified by UC Technology at 2012/05/22.
 *    Modified by UC Technology at 2012/08/09.
 *    Modified by UC Technology at 2015/06/04.
 *    Modified by UC Technology at 2017/04/21.
 *    Modified by UC Technology at 2023/12/20.
 *
 *    Copyright (c) 2011-2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	usermain.c (usermain)
 *	User Main
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <device.h>

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
#if	defined(SERIAL_DRIVER)
LOCAL	void	sampleApplicationTask(INT stacd, void *exinf);
//#define TEST_DUAL
#if	defined(TEST_DUAL)
LOCAL	void	sampleApplicationTaskForRsb(INT stacd, void *exinf);
LOCAL	ID	tskidForRsb = 0;
#define ALTPORT	(UB*)"rsg"
#endif
#else
LOCAL	void	sioTestTask(INT stacd, void *exinf);
#endif


EXPORT	INT	usermain( void )
{
#if	defined(SERIAL_DRIVER)
	T_CTSK	ctsk = { 0, TA_HLNG|TA_RNG1, (FP)sampleApplicationTask, 15, 1*1024 };
#else
	T_CTSK	ctsk = { 0, TA_HLNG|TA_RNG1, (FP)sioTestTask, 15, 1*1024 };
#endif
	ER	ercd = E_OK;

	ercd = tk_cre_tsk(&ctsk);
	if (ercd < E_OK) goto ERROR;
	ercd = tk_sta_tsk((ID)ercd, 0);
	if (ercd != E_OK) goto ERROR;

#if	defined(TEST_DUAL) && defined(SERIAL_DRIVER)
	ctsk.task = (FP)sampleApplicationTaskForRsb;
	tskidForRsb = ercd = tk_cre_tsk(&ctsk);
	if (ercd < E_OK) goto ERROR;
#endif

	ercd = tk_slp_tsk( TMO_FEVR );
ERROR:
	return 0;
}

/*----------------------------------------------------------------------*/
/*	Application Tasks						*/
/*----------------------------------------------------------------------*/
#if	defined(SERIAL_DRIVER)
void	show_RsMode( UW rcm, UW speed)
{
	tm_printf( (UB*)"\rDN_RSMODE\n" );
	
	if((rcm & DEV_SER_MODE_DLMASK) == DEV_SER_MODE_7BIT){
		tm_printf( (UB*)"data length = 7bit\n" );
	}
	if((rcm & DEV_SER_MODE_DLMASK) == DEV_SER_MODE_8BIT){
		tm_printf( (UB*)"data length = 8bit\n" );
	}  
		
	if((rcm & DEV_SER_MODE_SBMASK) == DEV_SER_MODE_1STOP){
		tm_printf( (UB*)"Stop-bit = 1bit\n" );
	}
	if((rcm & DEV_SER_MODE_SBMASK) == DEV_SER_MODE_2STOP){
		tm_printf( (UB*)"Stop-bit = 2bit\n" );
	}
	
	if((rcm & DEV_SER_MODE_PBMASK) == DEV_SER_MODE_PNON){
		tm_printf( (UB*)"parity = None\n" );
	}
	if((rcm & DEV_SER_MODE_PBMASK) == DEV_SER_MODE_PODD){
		tm_printf( (UB*)"parity = Odd" );
	}
	if((rcm & DEV_SER_MODE_PBMASK) == DEV_SER_MODE_PEVEN){
		tm_printf( (UB*)"parity = Even\n" );
	}
	
	if((rcm & DEV_SER_MODE_FLMASK) == DEV_SER_MODE_CTSEN){
		tm_printf( (UB*)"CTS Hard flow control\n" );
	}
	if((rcm & DEV_SER_MODE_FLMASK) == DEV_SER_MODE_RTSEN){
		tm_printf( (UB*)"RTS Hard flow control\n" );
	}
	if((rcm & DEV_SER_MODE_FLMASK) == DEV_SER_MODE_FLMASK){
		tm_printf( (UB*)"CTS Hard flow control\n" );
		tm_printf( (UB*)"RTS Hard flow control\n" );
	}
	tm_printf( (UB*)"baud rate=%d\n", speed);
	tm_printf( (UB*)"\n" );
}

void show_RsStat(UW rcm_error)
{
	tm_printf( (UB*)"\rCommunication Error\n" );
	tm_printf( (UB*)"Overrun Error=%d\n", rcm_error & DEV_SER_ERR_OE);
	tm_printf( (UB*)"Parity  Error=%d\n", rcm_error & DEV_SER_ERR_PE);
	tm_printf( (UB*)"Framing Error=%d\n", rcm_error & DEV_SER_ERR_FE);
	//tm_printf( (UB*)"Break Error=%d\n", rcm_error & DEV_SER_ERR_BE);
}


#define RS_DEVNM	"serd"
#define	START_MESSAGE	("*** Start UART Test ***\n\n")
#define	SIZE_OF_SM	(sizeof(START_MESSAGE)-1)
#define	ENTER_MESSAGE	("\n\rEnter characters > ")
#define	SIZE_OF_EM	(sizeof(ENTER_MESSAGE)-1)
LOCAL	void	sampleApplicationTask(INT stacd, void *exinf)
{
#define	READ_SIZE		16
LOCAL	B	read_buf[READ_SIZE];
	ID	dd;
	ID	reqid;
	SZ	asz_com;
	UW	rcm;

	/* Wait for completion of output of KERNEL MESSAGE. */
	tk_dly_tsk(10);

	dd = tk_opn_dev( (UB*)RS_DEVNM, TD_UPDATE );

#if	defined(TEST_DUAL)
	tk_sta_tsk(tskidForRsb,0);
#endif

	/* output message via RS_DEVNM */
	reqid = tk_swri_dev( dd, 0, (void*)START_MESSAGE, SIZE_OF_SM, &asz_com );
	tk_dly_tsk(10);

	/* TEST : Read Communication Mode */
	reqid = tk_srea_dev( dd, TDN_SER_MODE, (void*)(&rcm), sizeof(rcm), &asz_com );

	UW speed;
	reqid = tk_srea_dev( dd, TDN_SER_SPEED, (void*)(&speed), sizeof(speed), &asz_com );
	show_RsMode( rcm, speed );

	/* Wait for output completion */
	tk_dly_tsk(10);

	/* Write Communication Mode */
#if EVK_XMC7200
	/* SCB3 is connected to the PC via Kit prog3.
	   Kit prog3 is fixed to data:8bit, stop:1bit, parity:none, so you can only use this setting. */
	UW w_rcm = DEV_SER_MODE_CTSEN |
		   DEV_SER_MODE_8BIT |
		   DEV_SER_MODE_1STOP |
		   DEV_SER_MODE_PNON;
#else
	UW w_rcm = DEV_SER_MODE_CTSEN |
		   DEV_SER_MODE_7BIT |
		   DEV_SER_MODE_1STOP |
		   DEV_SER_MODE_PEVEN;
#endif
	reqid = tk_swri_dev( dd, TDN_SER_MODE, (void*)(&w_rcm), sizeof(w_rcm), &asz_com );

	UW w_speed = 9600;
	reqid = tk_swri_dev( dd, TDN_SER_SPEED, (void*)(&w_speed), sizeof(w_speed), &asz_com );

	/* Check the set value. */
	UW r_rcm;
	reqid = tk_srea_dev( dd, TDN_SER_MODE, (void*)(&r_rcm), sizeof(r_rcm), &asz_com );
	UW r_speed;
	reqid = tk_srea_dev( dd, TDN_SER_SPEED, (void*)(&r_speed), sizeof(r_speed), &asz_com );
	show_RsMode( r_rcm, r_speed );

	tk_dly_tsk(10);

	/* Reset Communication Mode */
	reqid = tk_swri_dev( dd, TDN_SER_MODE, (void*)(&rcm), sizeof(rcm), &asz_com );
	reqid = tk_swri_dev( dd, TDN_SER_SPEED, (void*)(&speed), sizeof(speed), &asz_com );

	/* Check Serial Status */
	UW rcm_error;
	reqid = tk_srea_dev( dd, TDN_SER_COMERR, (void*)(&rcm_error), sizeof(rcm_error), &asz_com );
	show_RsStat(rcm_error);
	tk_dly_tsk(10);

	/*--------------------------------------------------------------*/
	/*	Read 4 Bytes from DCE and echo back.			*/
	/*--------------------------------------------------------------*/
	reqid = tk_swri_dev( dd, 0, (void*)ENTER_MESSAGE, SIZE_OF_EM, &asz_com );
	while(1){
		reqid = tk_srea_dev( dd, 0, (void*)read_buf, 4, &asz_com );
		reqid = tk_swri_dev( dd, 0, (void*)read_buf, asz_com, &asz_com );
	}

	(void)tk_cls_dev( dd, 0 );
	tk_exd_tsk();
}
#else
LOCAL	void	sioTestTask(INT stacd, void *exinf)
{
	int	a = 0;

	while(1){
		printf("Count = %d\n", a++ );
		tk_dly_tsk(500);
	}

	tk_exd_tsk();
}
#endif
