#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#define MAX_NUM 100000

using namespace std;

timeval tv;
uint64_t timer;


bool NumCmp(int a, int b) { return (a > b); }

void startTimer() {
    gettimeofday(&tv,NULL);
    timer = 1000000 * tv.tv_sec + tv.tv_usec;
}

uint64_t stopTimer() {
    gettimeofday(&tv,NULL);
    timer = 1000000 * tv.tv_sec + tv.tv_usec - timer;
    return timer;
}

int main() {
    vector<int> v;
    list<int> l;
    for (int i = 0; i< MAX_NUM; ++i) {
        srand((int)time(0)  * i * i + 1);
        int num = rand();
        v.push_back(num);
        l.push_back(num);
    }
    
    // compare oper sort
    startTimer();   
    make_heap(v.begin(), v.end(), NumCmp);
    cout<<"vector\t heap sort    \t :\t"<< stopTimer() << endl;  

    startTimer();   
    make_heap(l.begin(), l.end(), NumCmp);
    cout<<"list\t heap sort    \t :\t"<< stopTimer() << endl;  

    return 0;
}
