#include <unistd.h>

#include <x86/x86utils/x86utils.h>
#include <ousia/ousia.h>
#include <ousia/ousia_type.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/utils.h>

int main(void)
{
	os_status stat = OS_OK;

	stat = os_init();
	stat = stat;
	x86utils_system_init();

	BOOT_LOGO();

	while (1) {
		sleep(1);
		/* FIXME keep print info here ?? */
		os_log(LOG_INFO, "get time\n");
	}

	return 0;
}
