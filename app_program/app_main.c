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
 *    Modifications: Adapted to the SBK-M4KN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	app_main.c (SBK-M4KN/TMPM4KNFYAFG)
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

#define LED_123		(0x00000015)			// SBK-M4KN (PORT B)
#define LED_45		(0x00000003)			// SBK-M4KN (PORT V)
LOCAL void task_1(INT stacd, void *exinf)
{
	// Enable Clock for PORT B & PORT V
	*CGPROTECT = 0xC1;
	*CGFSYSMENA |= CGFSYSMENA_PB;			// LED1,2,3 (,6)
	*CGFSYSMENA |= CGFSYSMENA_PV;			// LED4,5
	*CGPROTECT = 0x00;

	// Setup PORT B for LED1,2,3
	*GPIO_FR4(B) &= ~LED_123;			// DISABLE Function4
	*GPIO_IE(B)  &= ~LED_123;			// DISABLE Input
	*GPIO_CR(B)  |=  LED_123;			// ENABLE Output

	// Setup PORT V for LED4,5
	*GPIO_FR1(V) &= ~LED_45;			// DISABLE Function1
	*GPIO_FR2(V) &= ~LED_45;			// DISABLE Function2
	*GPIO_FR4(V) &= ~LED_45;			// DISABLE Function4
	*GPIO_IE(V)  &= ~LED_45;			// DISABLE Input
	*GPIO_CR(V)  |=  LED_45;			// ENABLE Output

	// LED Control
	while(1) {
		*GPIO_DATA(B) &= ~LED_123;
		*GPIO_DATA(V) &= ~LED_45;
		tk_dly_tsk(500);
		*GPIO_DATA(B) |=  LED_123;
		*GPIO_DATA(V) |=  LED_45;
		tk_dly_tsk(500);
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
		tk_dly_tsk(1*1000);
		tm_printf( "\r%4d sec", cnt++ );
	}
	return 0;
}
