#include <tk/tkernel.h>
#include <tm/tmonitor.h>

EXPORT INT usermain(void)
{
	tm_putstring((UB*)"Push any key to shutdown the micro T-Kernel.\n");
	tm_getchar(-1);

	return 0;
}
