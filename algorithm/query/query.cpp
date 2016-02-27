#include <iostream>
using namespace std;

int list[] = {1,2,3,4,5,6,7,8,9,10,111,112,113,114,115};

/**
 * Binary Search
 * Time complexity  : O(logN)
 * Space complexity : 0
 */
int binary_search(int target, int* list, int listLength) {
    int head, tail, mid;
    if (NULL == list || listLength <=0 ) {
        return -1;
    }
    head = 0;
    tail = listLength - 1;
    while (1) {
        if (head > tail) {
            return -1;
        }
        mid = (head + tail) / 2; // mid = (head + tail) >> 1
        if (target == *(list + mid)) {
            return mid;
        }
        if (target > *(list + mid)) {
            head = mid + 1;
        }
        if (target < *(list + mid)) {
            tail = mid - 1;
        }
    }
}

void main() {

}
