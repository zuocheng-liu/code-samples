#include <iostream>
#include <random>
using namespace std;

        default_random_engine e;
int main( ){
    for(int i=0; i<100; ++i) {
        uniform_int_distribution<unsigned> u(0, 19);
        cout<<u(e)<<endl;
    }
    return 0;
}
