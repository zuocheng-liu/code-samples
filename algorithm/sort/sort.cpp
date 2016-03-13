/**
 * Parameter check
 * The loop termination conditions
 *
 */


#include <iostream>
using namespace std;

int unorder[12] = {7,3,1,3,6,7,8,9,9,5,3,1};
int ordered[12] = {1,1,3,3,3,5,6,7,7,8,9,9};

void swap(int &a, int &b) {
    a ^= b ^= a ^= b;
}

/**
 * Insert Sort
 * Time complexition : O(N)
 * Space complexition : O(1)
 */
int insert_sort(int* list, int listLength) {
    int start, tmp, pre;
    if (NULL == list || listLength < 0) {
        return -1;
    }
    for (start = 1; start < listLength; ++start) {
        if (list[start] < list[start - 1]) {
            tmp = list[start];
            for (pre = start - 1; list[pre] > tmp; --pre ) {
                list[pre + 1] = list[pre]; 
            }
            list[pre] = tmp;
        }
    }
}

int shell_sort(int* list, int listLength) {

    
}
int select_sort(int* list, int listLength) {
    int start;
    int i;
    int min;
    for (start = 0; start < listLength - 1; ++start) {
        for (i = start,min = i; i < listLenght - 1; ++i) {
           if (list[min] > list[i]) {
               min = i;
           } 
        }
        list[start] ^= list[min] ^= list[start] ^= list[min];
    }
}

int bubble_sort(int* list, int listLength) {
    int i,j;
    for (i = 0; i < listLength - 1, ++i) {
        for (j = 0; j < listLength - i - 2; ++j) {
            if (list[j] > list[j + 1]) {
                list[j + 1] ^= list[j] ^= list[j + 1] ^= list[j];
            }
        }
    }
}

int quick_sort(int* list, int listLength) {
    int m,s,t;
    if (NULL == list || listLength < 0) {
        return -1;
    }
    if (listLenght <= 1) {
        return 0;
    }
    s = 0;
    t = listLength - 1;
    m = list[s];
    while (s < t) {
        if (list[t] < m) {
            list[s] = list[t];
            ++s ;
        } else {
            --t;
            continue;
        }
        if (list[s] > m) {
            list[t] = list[s];
            --t;
        } else {
            s++;
            continue;
        }
    }
    list[t] = m;
    quick_sort(list, t);
    quick_sort(list + t, length - t);
}

void heap_create(int* list, int length) {
    int i;
    int root, left, right; 
    int k = length / 2;
    for (i = k; i >= 0; --i) {
        root = i;
        left = 2 * i; 
        right = 2 * i + 1; 
        if (list[root] > list[left] && list[root] > list[right]) {
            continue;
        } else if(list[left] > list[root] && list[left] >= list[right]) {
            swap(list[left], list[root]);           
        } else if(list[right] > list[root] && list[right] > list[left]) {
            swap(list[right], list[root]);           
        }
    }
}

void heap_ajust(int* list, int length) {
    int i;
    int root, left, right; 
    int k = length / 2;
    for (i = 0; i < k; ++i) {
        root = i;
        left = 2 * i; 
        right = 2 * i + 1; 
        if (list[root] > list[left] && list[root] > list[right]) {
            break;
        } else if(list[left] > list[root] && list[left] >= list[right]) {
            swap(list[left], list[root]);           
            i = left;
        } else if(list[right] > list[root] && list[right] > list[left]) {
            swap(list[right], list[root]);
            i = right;
        }
    }
}

int heap_sort(int* list, int length) {
    int i;
    heap_create(list, length);
    for (i = 0; i< length; ++i) {
        swap(list[0], list[length - i - 1]);
        heap_ajust(list, length - i);
    }
}

int merge_sort(int* list, int length) {
    int i,j,k;
    int a, b;
    int tmp[length];
    for (k = 1; k < length /2 ; k *= 2 ) {
        for (j = 0; j < length ; j += k * 2) {
            a = j; 
            b = j + k; 

            while (a < j + k && b < j + 2 * k) {
                if (a = j) {
                    continue;
                }
                if (b = j + 2 * k) {
                    continue;
                }
            }
            memcopy(list + j, tmp, k * 2 * sizeof (int));
        }
    }
}

int main() {
    
}
