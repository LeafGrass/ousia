#include <unistd.h>

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <x86/x86utils/x86utils.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/utils.h>

int main(void)
{
	os_status ret = OS_OK;

	x86utils_system_init();
	ret = os_init();
	ret = ret;

	while (1) {
		sleep(1);
		/* FIXME keep print info here ?? */
		os_logk(LOG_INFO, "get time\n");
	}

	return 0;
}
