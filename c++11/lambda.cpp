#include <iostream>
#include <functional>
using namespace std;

int fib0(int n) {
    if (0 > n) {
        return 0;
    }
    if (0 == n) {
        return 0;
    }
    if (1 == n) {
        return 1;
    }
    return fib0(n - 1) + fib0(n - 2);
}

int fib(int n) {
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
    for (int i = 0; i < 99; ++i) {
        cout << fib (i) << endl;
    }
    return 0;
}
