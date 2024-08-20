/*

给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。
 

示例 1:

输入: s = "abcabcbb"
输出: 3 
解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。
示例 2:

输入: s = "bbbbb"
输出: 1
解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。
示例 3:

输入: s = "pwwkew"
输出: 3
解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。
     请注意，你的答案必须是 子串 的长度，"pwke" 是一个子序列，不是子串。
 

提示：

0 <= s.length <= 5 * 104
s 由英文字母、数字、符号和空格组成

*/

#include <iostream>
#include <string>
#include <cstring>
using namespace std;


class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if (s.size() <= 0) {
            return 0;
        }
        int max = 0;
        int len = 0;
        int table[512];
        int start = 0;
        bzero(table, 512);
        for (int i = 0 ; i < s.size(); ++i) {
            int c = ((int)s[i] + 256) % 256;
            int r_p = table[c] - 1;
            if (r_p >= 0) {
                for(int j = start; j <= r_p; ++j) {
                    int c = ((int)s[j] + 256) % 256;
                    table[c] = 0;
                }
                start = r_p + 1;
                table[c] = i + 1;
                len = i - start + 1;
                if (len > max) {
                    max = len;
                }
            } else {
                ++len;
                table[c] = i + 1;
                if (len > max) {
                    max = len;
                }
            }
        }
        return max;
    }
};

static void test(string s, int l) {
    Solution slt;
    int r = slt.lengthOfLongestSubstring(s);
    if (r == l) {
        cout << "(" << s << "," << l << ") OK" << endl;
    } else {
        cout << "(" << s << "," << l << ") Fail" << endl;
    }
    
}

int main() {
    test("au", 2);
}



/*
 * 解体思路
 * 1. 队列查询
 * 2. table 查询
 * 3. kmp算法
 */ 
