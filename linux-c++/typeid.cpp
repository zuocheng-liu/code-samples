#include <iostream>
using namespace std;

class A {
    public :
        int foo() {
            return 0;
        }
};

int main() {
    A a;
    cout << typeid(a).name() << endl;
}
