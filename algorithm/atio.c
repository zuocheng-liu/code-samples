#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int bool;
#define true 1
#define false 0;

int myAtoi(char* str) {

    int len = strlen(str);
    int i;
    int num = 0;
    int sign = 1;
    int begin = 0;
    for (i = 0; i < len ; i ++) {
        if (str[i] == ' ') {
            if (begin) {
                break;
            }
            continue;
        }
        if (str[i] == '-') {
            if (begin) {
                break;
            }
            if (sign == -1) {
                return 0;
            } else {
                begin = 1;
                sign = -1;
                continue;
            }
        }
        if (str[i] == '+') {
            if (begin) {
                break;
            }
            if (sign == -1) {
                break;
            } else {
                begin = 1;
                sign = 1;
                continue;
            }
        }
        if (str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + str[i] - '0';
            begin = 1;
        } else {
            break;
        }
    }
    num = num * sign;
    if (sign == -1 && num == 2147483648) {
        return 2147483647;
    }
    return num;
} 

int main() {
    char *s = "-1";
    int res = myAtoi(s);
    printf ("res : %d", res);
}
