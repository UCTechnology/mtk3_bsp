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
 *    Modifications: Adapted to the AdBun-M4GR.
 *    Modified by UC Technology at 2023/09/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	app_main.c (AdBun-M4GR/TMPM4GRF20FG)
 *	Application main program
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

LOCAL void task_1(INT stacd, void *exinf);
LOCAL ID	tskid_1;
LOCAL T_CTSK	ctsk_1 = {
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= task_1,
	.tskatr		= TA_HLNG | TA_RNG3,
};

LOCAL void task_1(INT stacd, void *exinf)
{
	tk_dly_tsk(50);
	while(1) {
		tk_dly_tsk(100);
		tm_printf( "*" );
	}

	tk_exd_tsk();
}

EXPORT INT usermain(void)
{
	INT	cnt = 0;

	tm_printf((UB*)"User program started\n");

	tskid_1 = tk_cre_tsk(&ctsk_1);
	tk_sta_tsk(tskid_1, 0);

	while(1){
		tm_printf( "\r%4d sec", cnt++ );
		tk_dly_tsk(1*1000);
	}
	return 0;
}
