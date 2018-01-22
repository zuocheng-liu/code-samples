#include <iostream>
#include <functional>
using namespace std;

int main() {
    std::string a = "A medium-sized string to avoid SSO";
    std::string b = a;
    if (a.data() == b.data()) {
        cout<< "uncopy"<<endl;
    } else {
        cout<< "copied"<<endl;
    }

    b.append("A");

    if (a.data() == b.data()) {
        cout<< "uncopy"<<endl;
    } else {
        cout<< "copied"<<endl;
    }
    return 0;
}
