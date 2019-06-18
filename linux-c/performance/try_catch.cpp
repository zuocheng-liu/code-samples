#include <sys/time.h>
#include <inttypes.h>

#include <iostream>
#include <exception>

#define TEST_CYCLE 1000000 //1M

using namespace std;

timeval tv;
uint64_t timer;

void startTimer() {
    gettimeofday(&tv,NULL);
    timer = 1000000 * tv.tv_sec + tv.tv_usec;
}

uint64_t stopTimer() {
    gettimeofday(&tv,NULL);
    timer = 1000000 * tv.tv_sec + tv.tv_usec - timer;
    return timer;
}

int biz() {
    return -1;
}

void bizE() {
    exception e;
    throw e;
}

int main() {

    startTimer();
    for (int i = 0 ; i< TEST_CYCLE ; ++i ) {
        try {
            bizE();
        } catch (exception& e) {
        }
    }
    cout<<"try cache"<<stopTimer()<<endl;
    
    startTimer();
    for (int i = 0 ; i< TEST_CYCLE ; ++i ) {
        if (biz() < 0) {
        }
    }
    cout<<"return error:" <<stopTimer()<<endl;
    
    return 0;
}
