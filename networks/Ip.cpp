#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define swap(a, b) ((a)^=(b)^=(a)^=(b))
char* itoa(const int val, char* str, int base) {
    if (val == 0) {
        str[0] = '0';
        return str;
    }
    
    int cal = val;
    int i = 0;
    while (true) {
        if (cal == 0) {
            break;
        }
        int mod = cal % base;
        str[i] = mod + '0';
        cal /= base;
        i++;
    }
    str[i] = '\0';
    int len = strlen(str);
    i = 0;
    while (true) {
        if (len / 2 > i) {
            swap (str[i], str[len -1 - i]);
            ++i;
        } else {
            break;
        }
    }
    return str;
}
int IpToInt(const char *ip) {
    int i;
    int length = strlen(ip);
    int session_level = 3;
    int session = 0;
    int res = 0;
    for (i = 0; i < length; ++i) {
        if (session_level < 0) {
            return -1;
        }
        if (ip[i] <= '9' && ip[i] >= '0') {
            session = session * 10 + (ip[i] - '0');
        } else if (ip[i] == '.') {
            res += session << session_level * 8;
            session = 0;
            --session_level;
        } else {
            return -1;
        }
    }
    res += session;
    return res;
}
void IntToIp(const int i, char *ip) {
    int section0 = i & 0xFF;
    int section1 = (i >> 8) & 0xFF;
    int section2 = (i >> 16) & 0xFF;
    int section3 = (i >> 24) & 0xFF;
    char section0_str[4] = {0};
    char section1_str[4] = {0};
    char section2_str[4] = {0};
    char section3_str[4] = {0};
    itoa(section0, section0_str, 10);
    itoa(section1, section1_str, 10);
    itoa(section2, section2_str, 10);
    itoa(section3, section3_str, 10);
    sprintf(ip, "%s.%s.%s.%s", section3_str, section2_str, section1_str, section0_str);
}
int main(int argc , char **argv) {
    if (argc < 2) {
        return -1;
    }
    //char ip[16] = "10.22.128.31";
    char* ip = argv[1];
    int i = IpToInt(ip);
    printf("ip:%s \t int:%d \n", ip, i);
    bzero(ip, 16);
    IntToIp(i, ip);
    printf("ip:%s \t int:%d \n", ip, i);
}
