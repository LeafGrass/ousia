#include <stdlib.h>
#include <unistd.h>

#include <ousia/ousia.h>
#include <ousia/ousia_type.h>

#include <x86/x86utils/x86utils.h>
#include <sys/print.h>
#include <sys/time.h>
#include <sys/utils.h>

void __os_main(void);

void ps_main(void *args)
{
}

int main(void)
{
	__os_main();

	while (1) {
		sleep(1);
	}

	return 0;
}
