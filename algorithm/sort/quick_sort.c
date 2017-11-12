#include <stdio.h>
#include <stdlib.h>

void quick_sort(int *left, int *right) {
    //printf("left:%p right:%p\n", left, right);
    if (left >= right) {
        return;
    }
    int *left_origin = left;
    int *right_origin = right;
    int bench = *left;
    int *blank = left;

    while(left < right) {
        printf("left:%p right:%p\n", left, right);
        while(left < right) {
            printf("bench:%d *right:%d\n", bench, *right);
            if (bench > *right) {
                *blank = *right;
                blank = right;
                ++left;
                break;
            } else {
                --right;
            }
        }

        while (left < right) {
            printf("bench:%d *left:%d\n", bench, *left);
            if (bench < *left) {
                *blank = *left;
                blank = left;
                --right;
                break;
            } else {
                ++left;
            }
        }
    }
    *left = bench;
    quick_sort(left_origin, left - 1);
    quick_sort(right + 1, right_origin);
}
void dump_array(int *confusion, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d,", confusion[i]);
    }
    printf("\r\n");
}

void main(void) {
    int confusion[] = {1,23,22,23,33,7,12,9,123,78};
    int size = sizeof(confusion)/sizeof(int);
    printf("before sort:");
    dump_array(confusion, size);

    quick_sort(confusion, confusion + sizeof(confusion)/sizeof(int) - 1);
    
    printf("after sort:");
    dump_array(confusion, size);
}
