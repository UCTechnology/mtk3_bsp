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
 *    Modifications: Adapted to the SBK-M4KN.
 *    Modified by UC Technology at 2023/03/27.
 *
 *    Copyright (c) 2023 UC Technology. All Rights Reserved.
 *----------------------------------------------------------------------
 */

/*
 *	sys_msg.h (SBK-M4KN TMPM4KN)
 *	Hardware-Dependent System message
 */

#ifndef _SYSDEPEND_TARGET_SYSMSG_
#define _SYSDEPEND_TARGET_SYSMSG_

#include <tm/tmonitor.h>

#if (USE_SYSTEM_MESSAGE && USE_TMONITOR)
#define SYSTEM_MESSAGE(s)	tm_putstring((UB*)s)
#else
#define SYSTEM_MESSAGE(s)
#endif /* USE_SYSTEM_MESSAGE && USE_TMONITOR */

#endif /* _SYSDEPEND_TARGET_SYSMSG_ */
