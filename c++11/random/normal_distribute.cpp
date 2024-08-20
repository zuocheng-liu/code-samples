#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>


using namespace std;


int main()
{
    std::default_random_engine e; //引擎
    std::normal_distribution<float> n(0, 33); //均值, 方差
    std::vector<int> vals(100);
    for(std::size_t i=0; i != 2000; ++i) {
        int v = std::floor(n(e)); //取整-最近的整数
        if (v < 0) v = 0 - v;
        if (v > 99) v = 0;
        if (v < vals.size())
            ++vals[v];
    }
    for (std::size_t j=0; j != vals.size(); ++j)
        std::cout << j << " : " << vals[j] << std::string(vals[j], '*') << std::endl;

    return 0;
}
