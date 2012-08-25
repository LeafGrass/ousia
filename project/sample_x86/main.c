#include <unistd.h>
#include <stdlib.h>

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <x86/x86utils/x86utils.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/utils.h>

void ps_main(void *args)
{
}

int main(void)
{
	os_status ret = OS_OK;

	x86utils_system_init();
	ret = os_init();

	if (ret != OS_OK) {
		os_logk(LOG_ERROR, "init error! exit.\n");
		return -1;
	}

	while (1) {
		system("sleep 1");
		os_logk(LOG_INFO, "get time\n");
	}

	return 0;
}
