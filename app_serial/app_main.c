/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.0 BSP
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 *    Modifications: Adapted to the AdBun-M4NR.
 *    Modified by UC Technology at 2023/09/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	usermain.c (usermain)
 *	User Main
 */
#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <device.h>
//#include <sysdepend.h>

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
#define SERIAL_DRIVER
#define RS_DEVNM	"sera"

#if defined(SERIAL_DRIVER)
LOCAL	void	sampleApplicationTask(INT stacd, void *exinf);
//#define TEST_DUAL
#if defined(TEST_DUAL)
LOCAL	void	sampleApplicationTaskForRsb(INT stacd, void *exinf);
LOCAL	ID	tskidForRsb = 0;
#define ALTPORT	(UB*)"rsg"
#endif
#else
LOCAL	void	sioTestTask(INT stacd, void *exinf);
#endif


EXPORT INT usermain( void )
{
#if defined(SERIAL_DRIVER)
	T_CTSK	ctsk = { 0, TA_HLNG|TA_RNG1, (FP)sampleApplicationTask, 15, 1*1024 };
#else
	T_CTSK	ctsk = { 0, TA_HLNG|TA_RNG1, (FP)sioTestTask, 15, 1*1024 };
#endif
	ER	ercd = E_OK;

	ercd = tk_cre_tsk(&ctsk);
	if (ercd < E_OK) goto ERROR;
	ercd = tk_sta_tsk((ID)ercd, 0);
	if (ercd != E_OK) goto ERROR;

#if defined(TEST_DUAL) && defined(SERIAL_DRIVER)
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
#if defined(SERIAL_DRIVER)
void show_RsMode( UW rcm, UW speed)
{
	tm_printf( "\rDN_RSMODE\n" );
	
	if((rcm & 0x00000300) == DEV_SER_MODE_7BIT){
		tm_printf( "data length = 7bit\n" );
	}
	if((rcm & 0x00000300) == DEV_SER_MODE_8BIT){
		tm_printf( "data length = 8bit\n" );
	}	
	
	if((rcm & 0x00000440) == DEV_SER_MODE_1STOP){
		tm_printf( "Stop-bit = 1bit\n" );
	}
	if((rcm & 0x00000440) == DEV_SER_MODE_2STOP){
		tm_printf( "Stop-bit = 2bit\n" );
	}
	
	if((rcm & 0x00018000) == DEV_SER_MODE_PNON){
		tm_printf( "parity = None\n" );
	}
	if((rcm & 0x00018000) == DEV_SER_MODE_PODD){
		tm_printf( "parity = Odd" );
	}
	if((rcm & 0x00018000) == DEV_SER_MODE_PEVEN){
		tm_printf( "parity = Even\n" );
	}
	
	if((rcm & 0x00000022) == DEV_SER_MODE_CTSEN){
		tm_printf( "CTS Hard flow control\n" );
	} else if((rcm & 0x00000022) == DEV_SER_MODE_RTSEN){
		tm_printf( "RTS Hard flow control\n" );
		tm_printf( "CTS Hard flow control\n" );
	}
	tm_printf( "baud rate=%d\n", speed);
	tm_printf( "\n" );
}

void show_RsStat(UW rcm_error)
{
	tm_printf( "\rCommunication Errors : %08X\n", rcm_error );
	tm_printf( "Overrun Error = %d\n", !!(rcm_error & DEV_SER_ERR_OE) );
	tm_printf( "Parity  Error = %d\n", !!(rcm_error & DEV_SER_ERR_PE) );
	tm_printf( "Framing Error = %d\n", !!(rcm_error & DEV_SER_ERR_FE) );
#ifdef DEV_SER_ERR_BE
	tm_printf( "Break   Error = %d\n", !!(rcm_error & DEV_SER_ERR_BE) );
#endif
#ifdef DEV_SER_ERR_NF
	tm_printf( "Break   Error = %d\n", !!(rcm_error & DEV_SER_ERR_NF) );
#endif
	tm_printf( "DEV_SER_ERR_ROVR = %d\n", !!(rcm_error & DEV_SER_ERR_ROVR) );
}

#define	START_MESSAGE		("*** Start UART Test ***\n\n")
#define	SIZE_OF_SM		(sizeof(START_MESSAGE)-1)
#define	ENTER_MESSAGE		("\n\rEnter characters > ")
#define	SIZE_OF_EM		(sizeof(ENTER_MESSAGE)-1)
LOCAL void sampleApplicationTask(INT stacd, void *exinf)
{
#define	READ_SIZE		128
LOCAL	B	read_buf[READ_SIZE];
	ID	dd;
	ER	ioer;
	W	asize;
	UW rcm;

	/* Wait for completion of output of KERNEL MESSAGE. */
	tk_dly_tsk(10);
	
	ioer = tk_opn_dev( (UB*)RS_DEVNM, TD_UPDATE );
	if (ioer >= E_OK) {
		dd = ioer;
	}
	else {
		tk_exd_tsk();
	}

#if defined(TEST_DUAL)
	tk_sta_tsk(tskidForRsb,0);
#endif

	/* output message via RS_DEVNM */
	ioer = tk_swri_dev( dd, 0, (void*)START_MESSAGE, SIZE_OF_SM, &asize );
	tk_dly_tsk(10);

	/* TEST : Read Communication Mode */
	ioer = tk_srea_dev( dd, TDN_SER_MODE, (void*)(&rcm), sizeof(rcm), &asize );

	UW speed;
	ioer = tk_srea_dev( dd, TDN_SER_SPEED, (void*)(&speed), sizeof(speed), &asize );
	show_RsMode( rcm, speed );
	tk_dly_tsk(10);			/* Wait for output completion */

	/* Write Communication Mode */
	UW w_rcm = DEV_SER_MODE_CTSEN |
		   DEV_SER_MODE_7BIT  |
		   DEV_SER_MODE_1STOP |
		   DEV_SER_MODE_PEVEN;
	ioer = tk_swri_dev( dd, TDN_SER_MODE, (void*)(&w_rcm), sizeof(w_rcm), &asize );

	UW w_speed = 9600;
	ioer = tk_swri_dev( dd, TDN_SER_SPEED, (void*)(&w_speed), sizeof(w_speed), &asize );

	/* Check the set value. */
	UW	r_rcm;
	ioer = tk_srea_dev( dd, TDN_SER_MODE, (void*)(&r_rcm), sizeof(r_rcm), &asize );
	UW	r_speed;
	ioer = tk_srea_dev( dd, TDN_SER_SPEED, (void*)(&r_speed), sizeof(r_speed), &asize );
	show_RsMode( r_rcm, r_speed );
	tk_dly_tsk(10);

	/* Reset Communication Mode */
	ioer = tk_swri_dev( dd, TDN_SER_MODE, (void*)(&rcm), sizeof(rcm), &asize );
	ioer = tk_swri_dev( dd, TDN_SER_SPEED, (void*)(&speed), sizeof(speed), &asize );

	/* Check Serial Status */
	UW rcm_error;
	ioer = tk_srea_dev( dd, TDN_SER_COMERR, (void*)(&rcm_error), sizeof(rcm_error), &asize );
	show_RsStat(rcm_error);
	tk_dly_tsk(10);

#if	1
	/*--------------------------------------------------------------*/
	/*	Read 4 Bytes from DCE and echo back.			*/
	/*--------------------------------------------------------------*/
	ioer = tk_swri_dev( dd, 0, (void*)ENTER_MESSAGE, SIZE_OF_EM, &asize );
	while(1){
		ioer = tk_srea_dev( dd, 0, (void*)read_buf, 4, &asize );
		ioer = tk_swri_dev( dd, 0, (void*)read_buf, asize, &asize );
	}
#else
	/*--------------------------------------------------------------*/
	/*	make BUFFER OVER RUN (DEV_SER_ERR_ROVR)			*/
	/*--------------------------------------------------------------*/
	ioer = tk_swri_dev( dd, 0, (void*)ENTER_MESSAGE, SIZE_OF_EM, &asize );
	while(1){
		tk_dly_tsk(1*1000);
		ioer = tk_srea_dev( dd, TDN_SER_COMERR, (void*)(&rcm_error), sizeof(rcm_error), &asize );
		if( rcm_error ){
			show_RsStat(rcm_error);
			tk_dly_tsk(10);
		}
		if( rcm_error & DEV_SER_ERR_ROVR ){
			ioer = tk_srea_dev( dd, 0, (void*)read_buf, 0, &asize );
			ioer = tk_srea_dev( dd, 0, (void*)read_buf, asize, &asize );
			ioer = tk_swri_dev( dd, 0, (void*)read_buf, asize, &asize );
//			ioer = tk_swri_dev( dd, 0, (void*)read_buf, 0, &asize );
			ioer = tk_swri_dev( dd, 0, (void*)ENTER_MESSAGE, SIZE_OF_EM, &asize );
		}
	}
#endif
	(void)tk_cls_dev( dd, 0 );
	tk_exd_tsk();
}
#else
LOCAL void sioTestTask(INT stacd, void *exinf)
{
	int a = 0;
	
	while(1){
		printf("Count = %d\n", a++ );
		tk_dly_tsk(500);
	}

	tk_exd_tsk();
}
#endif
