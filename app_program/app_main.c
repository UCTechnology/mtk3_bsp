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
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	app_main.c (ADBUN-M4NR/TMPM4NRF20FG)
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

#define LED_1		(0x00000010)			// ADBUN-M4NR (PORT W4 )
#define LED_2		(0x00000020)			// ADBUN-M4NR (PORT W5 )
#define LED_3		(0x00000040)			// ADBUN-M4NR (PORT W6 )
#define LED_4		(0x00000080)			// ADBUN-M4NR (PORT W7 )
LOCAL void task_1(INT stacd, void *exinf)
{
        // Enable Clock for PORT W
	*CGPROTECT = 0xC1;
	*CGFSYSMENB |= CGFSYSMENB_PW;			// LED1,2,3,4
	*CGPROTECT = 0x00;

	// Setup PORT W for LED1,2,3,4
	*GPIO_FR3(W) &= ~(LED_1|LED_2|LED_3|LED_4);     // DISABLE Function3
	*GPIO_FR5(W) &= ~(LED_1|LED_4);                 // DISABLE Function5
	*GPIO_FR7(W) &= ~(LED_1|LED_2|LED_3|LED_4);     // DISABLE Function7
	*GPIO_IE(W)  &= ~(LED_1|LED_2|LED_3|LED_4);	// DISABLE Input
	*GPIO_CR(W)  |=  (LED_1|LED_2|LED_3|LED_4);	// ENABLE Output
	// LED Control
	*GPIO_DATA(W) &= ~(LED_1|LED_2|LED_3|LED_4);
	while(1) {
		tk_dly_tsk(500);
		*GPIO_DATA(W) |=  LED_1;
		tk_dly_tsk(500);
		*GPIO_DATA(W) &= ~LED_1;
		tk_dly_tsk(500);
		*GPIO_DATA(W) |=  LED_2;
		tk_dly_tsk(500);
		*GPIO_DATA(W) &= ~LED_2;
		tk_dly_tsk(500);
		*GPIO_DATA(W) |=  LED_3;
		tk_dly_tsk(500);
		*GPIO_DATA(W) &= ~LED_3;
		tk_dly_tsk(500);
		*GPIO_DATA(W) |=  LED_4;
		tk_dly_tsk(500);
		*GPIO_DATA(W) &= ~LED_4;
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
