/**
 * Parameter check
 * The loop termination conditions
 * @author Zuocheng Liu
 */


#include <iostream>
using namespace std;

int unorder[12] = {7,3,1,3,6,7,8,9,9,5,3,1};
int ordered[12] = {1,1,3,3,3,5,6,7,7,8,9,9};

void swap(int &a, int &b) {
    a ^= b ^= a ^= b;
}


void heap_create(int* list, int length) {
    int i;
    for (i = length / 2; i > 0; --i) {
        heap_ajust(list, length, i);
    }
}

void heap_ajust(int* list, int length, int root) {
    int left, right; 
    left = 2 * i; 
    right = 2 * i + 1;
    if (right == length) {
        right = left;
    }
    while (right < length) {
        if (list[root] >= list[left] && list[root] >= list[right]) {
            return ;
        } else if (list[left] >= list[root] && list[left] >= list[right]) {
            swap(list[left], list[root]);           
            heap_ajust(list, length, left);
        } else if (list[right] >= list[root] && list[right] >= list[left]) {
            swap(list[right], list[root]);
            heap_ajust(list, length, right);
        }
    }
}

void heap_sort(int* list, int length) {
    int i;
    heap_create(list, length);
    for (i = 1; i < length - 1; ++ i) {
        swap(list[1], list[length - i]);
        heap_ajust(list, 1, length - i);
    }
}

int main() {
    
}
