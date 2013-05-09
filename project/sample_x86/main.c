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
	x86utils_system_init();
	os_init();

	while (1)
		usleep(10000);

	return 0;
}
