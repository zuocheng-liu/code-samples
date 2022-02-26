#include <stdio.h>
void __attribute__((weak)) real_func()
{
    printf("fake func\n");
}
void main()
{
        real_func();
}


/*
 * LD_PRELOAD=./libstrong.so ./main_with_weak
 */
