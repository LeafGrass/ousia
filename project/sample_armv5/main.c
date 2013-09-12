#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <ulib/string.h>
#include <ulib/stdlib.h>
#include <sys/time.h>
#include <sys/mm.h>
#include <sys/sched.h>		/* FIXME This bull shit expose the _pcb_t... */
#include <sys/print.h>

#include "console/console.h"
#include "utils/utils.h"

void ps_main(void)
{
	os_log(LOG_INFO, "enter main\n");
	for (;;) ;
}
