/*
最长的回文子串
给你一个字符串 s，找到 s 中最长的回文子串。
如果字符串的反序与原始字符串相同，则该字符串称为回文字符串。

示例 1：
输入：s = "babad"
输出："bab"
解释："aba" 同样是符合题意的答案。

示例 2：
输入：s = "cbbd"
输出："bb"
 
提示：
1 <= s.length <= 1000
s 仅由数字和英文字母组成
 
 */

class Solution {
public:
    string longestPalindrome(string s) {
        if (s.size() <= 1) {
            return s;
        }
        if (s.size() == 2) {
            if (s[0] == s[1]) {
                return s;
            } else {
                return s.substr(0,1);
            }
        }
        string res;
        int max_start = 0;
        int max_end = 0;
        int max_d = 0;
        int max_i = 0;
        for (int i = 0;i < s.size(); ++i) {
            for (int d = 1; i + d < s.size() && i - d >= 0 ; ++d ) {
                int start = i - d;
                int end = i + d;
                if (s[start] == s[end] ) {
                    if (2*d+1 > max_d) {
                        max_d = 2*d+1;
                        max_i = i;
                        max_start = start;
                        max_end = end;
                    }
                } else {
                    break;
                }
            }
            for (int d = 1; i + d < s.size() && i - d +1 >= 0 ; ++d ) {
                int start2 = i - d + 1;
                int end2 = i + d;
                if (s[start2] == s[end2]) {
                    if (2*d > max_d) {
                        max_d = 2*d;
                        max_i = i;
                        max_start = start2;
                        max_end = end2;
                    }
                } else {
                    break;
                }
            }
        }
        return s.substr(max_start, max_end - max_start + 1);
    }
};
