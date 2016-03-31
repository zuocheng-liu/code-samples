#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
/*
 * 任务缺少队列，不能
 */
class Thread {
    public :
        Thread(void *(*func)(void *), void *arg = NULL);
        ~Thread();
        pthread_t getId();
        int join(void **status = NULL);
        int detach();
        void exit(void *status = NULL);
    protected :
        pthread_t _tid;
}; 
// End Thread declare

template<class ThreadType>
class ThreadPool {
    public :
        typedef ThreadType *ThreadTypePtr;
        typedef vector<ThreadTypePtr> ThreadTypePtrVector;
        
        ThreadPool(u_int32_t threadNum);
        ~ThreadPool();
        void addTask();
        void stop();
    protected :
        static void* run(void * arg);
        ThreadTypePtrVector _threadVector;
        u_int32_t _threadNum;
        u_int32_t _idle; // the number of idle threads
        u_int32_t _threadNum;
        static pthread_cond_t _cond;
        static pthread_mutex_t _mutex;
};
// End ThreadPool declare
/*
Thread::Thread() {
   int res = pthread_create(&_tid, NULL, func, arg); 
   if (res != 0) {
       // print error
   }
}
*/

Thread::Thread(void *(*func)(void *), void *arg) {
   int res = pthread_create(&_tid, NULL, func, arg); 
   if (res != 0) {
       // print error
   }
}

Thread::~Thread() {
}

pthread_t Thread::getId() {
    return _tid;
}

int Thread::join(void **status) {
    return pthread_join(_tid, status);
}

int Thread::detach() {
    return pthread_detach(_tid);
}

void Thread::exit(void *status) {
    pthread_exit(status);
}
/*
void* Thread::run(int argc, void *argv) {
    cout<<"Thread ID : " << (int)pthread_self() <<endl;
}
*/
// End Thread definition

template<class ThreadType>
pthread_mutex_t ThreadPool<ThreadType>::_mutex = PTHREAD_MUTEX_INITIALIZER;   
template<class ThreadType>
pthread_cond_t ThreadPool<ThreadType>::_cond = PTHREAD_COND_INITIALIZER; 

template<class ThreadType>
ThreadPool<ThreadType>::ThreadPool(u_int32_t threadNum) {
    ThreadTypePtr ptr;
    
    pthread_mutex_init(&_mutex, NULL);  
    pthread_cond_init(&_cond, NULL);  
    
    _threadNum = threadNum;
    _idle = threadNum;
    _threadVector.resize(threadNum);
    for (int i = 0; i < threadNum; ++ i ) {
        ptr = new ThreadType(run);
        _threadVector[i] = ptr;
    }
}

template<class ThreadType>
ThreadPool<ThreadType>::~ThreadPool() {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_cond); 
}

template<class ThreadType>
void ThreadPool<ThreadType>::addTask() {
    pthread_mutex_lock(&_mutex);  
    // Exclusive operation here
    if (_idle > 0) {
        pthread_cond_signal(&_cond);
    }  
    pthread_mutex_unlock(&_mutex);
}

template<class ThreadType>
void ThreadPool<ThreadType>::stop() {
    //pthread_cond_broadcast(&_cond);
    
    typename ThreadTypePtrVector::iterator it;
    for(it = _threadVector.begin(); it != _threadVector.end(); ++ it) {
        (*it)->exit(NULL);
        delete *it;
    }
    cout<<__LINE__<<endl;
}

template<class ThreadType>
void* ThreadPool<ThreadType>::run(void *arg) {
    for(;;) {
        pthread_mutex_lock(&_mutex);  
        cout<<"Before waiting : " << pthread_self() <<endl;
        ++ _idle;
        pthread_cond_wait(&_cond, &_mutex);  
        // Exclusive operation here
        -- _idle;
        pthread_mutex_unlock(&_mutex);
        cout<<"Thread ID : " << pthread_self() <<endl;
        usleep(1000 * 3000);  
    }
}
// End Thread Define

int main() {
    ThreadPool<Thread> *threadPool = new ThreadPool<Thread>(3);
    for (int i = 0; i < 999; ++ i) {
        usleep(1000 * 1000);  
        cout<<"add Task " << endl;
        threadPool->addTask();
    }
    //threadPool->stop();
    delete threadPool;
}
