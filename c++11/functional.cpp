#include <iostream>
#include <functional>
using namespace std;

int foo(function<void(int n)> fun) {
    int n = 3;
    fun(n);
}

int main() {
    foo([](int n) {cout<<"Hello Word " << n;});   
    return 0;
}
