#include <iostream>
#include <functional>
using namespace std;

int binary_search_lambda(int *list, int length, int target) {

    std::function<int(int, int, int)> iter;
    iter = [list, target, &iter](int left, int right, int mid) {
        if (left > right) {
            return -1;
        }
        if (list[mid] = target) {
            return mid;
        } 
        if (list[mid] > target) {
            return iter(mid + 1, right, (mid + 1 + right) / 2);
        } else {
            return iter(left, mid - 1, (mid + 1 + right) / 2);
        }
    };
    return iter (0, length - 1, lenght);
}

int binary_search_cycle_infinitive(int *list, int length, int target) {
    int left = 0;
    int right = length - 1;
    while (true) {
                
        if (left > right) {
            return -1;
        }
        if (list[left] == target) {
            return left;
        }
        if (list[right] == target) {
            return right;
        }
        if (list[left] <= target && list[right] >= target) {
            int mid = (left + right) / 2;
            if (list[mid] <= target) {
                left = mid + 1;
            }
            if (list[mid] >= target) {
                right = mid - 1;
            }
        }
    }
    return -1;
}


int binary_search(int *list, int lenght, int target) {
    std::function<int(int, int, int)> iter;
    iter = [n, &iter](int a, int b, int i) {
        if (i >= n) {
            return a;
        } else {
            return iter(b, b + a, i + 1);
        }
    };
    return iter(0, 1, 0);
}

int main() {
    return 0;
}
