#include <stdio.h>
void real_func()
{
    printf("int real func\n");
}
void main()
{
        real_func();
}


/*
 * LD_PRELOAD=./libweak.so ./main_ls
 */
