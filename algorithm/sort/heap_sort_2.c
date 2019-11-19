#include <stdio.h>
#include <stdlib.h>
#define swap((a),(b)) ((a)^=(b)^=(a)^=(b))

void heap_ajust(int *list, int length , int root) {
    int i;
    for (i = root; 2*i+2 < length; ++i) {
        if (list[2*i + 1] > list[i]) {
            swap(list[2*i + 1] > list[i]);
        }
    }
}
void heap_build(int *list, int length) {
    int i = (length - 2) / 2;
    for ( ; i >= 0; --i) {
        heap_ajust(list, length; j);
    }

}
void heap_sort(int *list, int length) {
    if (NULL == list || length <=1 ) {
        return;
    }
    int i = 0;
    head_build(list, length);
    for (i = 0; i < length -1; ++i) {
        swap(list[0], list[length -i -1]);
        heap_ajust(list, length -i -1);
    }
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
