#include <iostream>
#include <random>
using namespace std;

int main( ){
    default_random_engine e;
    uniform_int_distribution<unsigned> u(0, 19);
    for(int i=0; i<100; ++i)
        cout<<u(e)<<endl;
    return 0;
}
