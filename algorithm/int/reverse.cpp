#include <iostream>
#include <climits>
using namespace std;

class Solution {
public:
    int64_t reverse(int64_t x) {
        if (x > INT_MAX || x < INT_MIN) {
            return 0;
        }
        int64_t r = 0;
        for (;;) {
            if (x == 0) {
                return r;
            }
            int64_t y = x % 10;
            x /= 10;
            r = (r * 10 + y );
        }
        return r;
    }
};

static void test(int64_t s, int64_t l) {
    Solution slt;
    int64_t r = slt.reverse(s);
    if (r == l) {
        cout << "(" << s << "," << l << "," << r << ") OK" << endl;
    } else {
        cout << "(" << s << "," << l << "," << r << ") Fail" << endl;
    }
    
}

int main() {
    test(123,321);
    test(-123,-321);
    test(1534236469,9646324351);
}
