#include <stdio.h>
void real_func() __attribute__((weak));
void real_func()
{
    printf("fake func\n");
}
