#include <iostream>
#include <random>
int main()
{
    std::random_device rd;
    for(int n=0; n<20000; ++n)
        std::cout << rd() << std::endl;
    return 0; 
}
