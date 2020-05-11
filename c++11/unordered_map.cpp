#include <iostream>
#include <unordered_map>
#include <map>
#include <random>
using namespace std;


int main() {
    map<int64_t, int64_t> ordered_map;
    default_random_engine e;
    uniform_int_distribution<int64_t> u(0, 100);
    for(int i=0; i<100; ++i) {
        ordered_map[u(e)] = i;
    }
    for (auto it = ordered_map.begin(); it != ordered_map.end(); ++it) {
        cout << it->first << endl;
    }
    return 0;
}
