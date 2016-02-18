#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int bool;
#define true 1
#define false 0;

bool isAnagram(char* s, char* t) {
    int i,j;
    int sLen = strlen(s);    
    int tLen = strlen(t); 
    if (tLen != sLen) {
        return false;
    }   
    for (i = 0; i < sLen; i++) {
        for (j = 0; j < sLen; j++) {
        if (s[i] == t[j]) {
            return true;
        }
        }
    }
    return false;
}

int main() {
    char *s = "hello";
    char *t = "olleh";
    bool res = isAnagram(s, t);
    printf ("res : %d", res);
}
