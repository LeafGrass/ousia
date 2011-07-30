#include <stdio.h>

#include <ousia/scheduler.h>

int main(void)
{
    int result = 0;

    sample_function(10, 20, &result);
    printf("result = %d\n", result);

    printf("Build success.\n");

    return 0;
}
