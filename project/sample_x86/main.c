#include <stdlib.h>
#include <unistd.h>

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
	int ret = OS_OK;

	x86utils_system_init();
	ret = os_init();

	if (ret != OS_OK) {
		os_printk(LOG_ERROR, "init error! exit.\n");
		return -1;
	}

	while (1) {
		usleep(10000);
	}

	return 0;
}
