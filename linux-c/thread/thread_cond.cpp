#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;
void* fun(void *arg) {
    pthread_mutex_lock(&mymutex);
    pthread_cond_wait(&mycond, &mymutex);
    std::cout << "Thread awake! : " << pthread_self() << endl;
    pthread_mutex_unlock(&mymutex);
    std::cout << "Thread ID : " << pthread_self() << endl;
}

int main() {
    pthread_t t1;
    pthread_t t2;
    //pthread_mutex_lock(&mymutex);
    pthread_create(&t1, NULL, fun, NULL);
    pthread_create(&t2, NULL, fun, NULL);
    usleep(3000);
    std::cout << "Call Thread : " << endl;
    pthread_cond_signal(&mycond);
    usleep(9000000);
    std::cout << "Call Thread : " << endl;
    pthread_cond_signal(&mycond);
    usleep(3000);
    usleep(3000);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
