#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include <list>
#include <vector>
#include <iostream>

#define MAX_NUM 100000

using namespace std;

timeval tv;
uint64_t timer;


bool NumCmp(uint32_t a, uint32_t b) { return (a > b); }

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
    vector<uint32_t> v;
    vector<uint32_t> v2;
    list<uint32_t> l;
    list<uint32_t> l2;
    for (int i = 0; i< MAX_NUM; ++i) {
        srand((int)time(0)  * i * i + 1);
        int num = rand();
        v.push_back(num);
        l.push_back(num);
    }
    
    // compare oper traverse
    startTimer();   
    for (vector<uint32_t>::iterator iter = v.begin(); iter != v.end(); ++ iter) {
        *iter;
    }
    cout<<"vector\t traverse\t :\t"<< stopTimer() << endl;  
    
    startTimer();   
    for (int i = 0 ; i < MAX_NUM; ++ i) {
        //v[i];
    }
    cout<<"vector\t num traverse\t :\t"<< stopTimer() << endl;  


    startTimer();   
    for (list<uint32_t>::iterator iter = l.begin(); iter != l.end(); ++ iter) {
    }
    cout<<"list\t traverse\t :\t"<< stopTimer() << endl;  


    // compare oper push_back   
    startTimer();   
    for (vector<uint32_t>::iterator iter = v.begin(); iter != v.end(); ++ iter) {
        v2.push_back(*iter);
    }
    cout<<"vector\t push_back\t :\t"<< stopTimer() << endl;  

    startTimer();   
    for (list<uint32_t>::iterator iter = l.begin(); iter != l.end(); ++ iter) {
        l2.push_back(*iter);
    }
    cout<<"list\t push_back\t :\t"<< stopTimer() << endl;  

    // compare oper delete
    startTimer();
    v2.clear();
    for (vector<uint32_t>::iterator iter = v2.begin(); iter != v2.end(); ++ iter) {
    //    v2.erase(iter);
    }
    //cout<<"vector\t delete\t :\t"<< stopTimer() << endl;  

    startTimer();   
    for (list<uint32_t>::iterator iter = l2.begin(); iter != l2.end(); ++ iter) {
        iter = l2.erase(iter);
    }
    cout<<"list\t delete\t :\t"<< stopTimer() << endl;  
    
    // compare oper push_back   
    startTimer();   
    for (vector<uint32_t>::iterator iter = v.begin(); iter != v.end(); ++ iter) {
        v2.push_back(*iter);
    }
    cout<<"vector\t push_back\t :\t"<< stopTimer() << endl;  

    startTimer();   
    for (list<uint32_t>::iterator iter = l.begin(); iter != l.end(); ++ iter) {
        l2.push_back(*iter);
    }
    cout<<"list\t push_back\t :\t"<< stopTimer() << endl;  


    // compare oper clear
    startTimer();   
    v2.clear();
    cout<<"vector\t clear      \t:\t"<< stopTimer() << endl;  

    startTimer();   
    l2.clear();
    cout<<"list\t clear     \t :\t"<< stopTimer() << endl;  

    // compare oper sort
    startTimer();   
    std::sort(v.begin(), v.end(), NumCmp);
    cout<<"vector\t sort    \t :\t"<< stopTimer() << endl;  

    startTimer();   
    l.sort(NumCmp);
    cout<<"list\t sort    \t :\t"<< stopTimer() << endl;  

    return 0;
}
