#include <tk/tkernel.h>
#include <tm/tmonitor.h>

IMPORT	void	createSubTask(void);

/* usermain関数 */
EXPORT INT usermain(void)
{
	tm_printf((UB*)"hello,world\n");

#if	0
	createSubTask();
	tk_slp_tsk(TMO_FEVR);
#endif

	return 0;
}

/*----------------------------------------------------------------------*/
/*	Additional program						*/
/*----------------------------------------------------------------------*/
#define MSG(f,...)	do{ tm_printf((UB*)f,##__VA_ARGS__); }while(0)

LOCAL	void	subTask(void)
{
	W	cnt = 0;
	while(1){
		tk_dly_tsk(1*1000);
		MSG( "\r%4d 秒経過",cnt++ );
	}

}

const T_CTSK	ctsk_subTask = {
	0,			/* exinf	*/
	TA_HLNG|TA_RNG0,	/* tskatr	*/
	(FP)&subTask,		/* task		*/
	1,			/* itskpri	*/
	10*1024,		/* stksz	*/
#if USE_OBJECT_NAME
	"SubTask",		/* dsname[8]	*/
#endif
	NULL,			/* bufptr	*/
};

LOCAL	ID	tid;		/* Sub Task ID	*/
EXPORT	void	createSubTask(void)
{
	ER	ercd;

	tid = tk_cre_tsk( (CONST T_CTSK*)&ctsk_subTask );
	MSG( "tk_cre_tsk() = %d\n", tid );
	ercd = tk_sta_tsk( tid, 0);
	MSG( "tk_sta_tsk() = %d\n", ercd );
}
