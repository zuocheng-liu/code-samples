// 用来验证 ++ 自增操作不是原子性的
#include<unistd.h>
#include<pthread.h>
#include<iostream>
using namespace std;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mycond = PTHREAD_COND_INITIALIZER;


static int i=0;
class test{
    public:
        test():a(0){}
        void increase(){//对变量自增
            pthread_mutex_lock(&mymutex);
            pthread_cond_wait(&mycond, &mymutex);
            pthread_mutex_unlock(&mymutex);
            int i = 10000;
            //pthread_mutex_lock(&mymutex);
            while (i-- > 0) {
               __asm__ __volatile__(
                       "lock movq %1,%%rax\n\r" 
                       "lock incq %%rax\n\r" 
                       "lock movq %%rax,%0\n\r" 
                       : "=m"(a) :"m"(a)); 
            }
            //pthread_mutex_unlock(&mymutex);
        }
        void get() const{
            cout<<"a="<<a<<endl;
        }
    private:
        volatile uint64_t a;
};

void* worker(void* arg){
    test* temp=(test*)arg;
    temp->increase();
}
#define THREAD_NUM 100
int main(){

    pthread_t pthd[THREAD_NUM];
    test* one=new test;
    for(int i=0;i<THREAD_NUM;i++){
        pthread_create(&pthd[i],NULL,worker,one);//开启100个线程对变量自增
    }
    sleep(3);
    pthread_mutex_lock(&mymutex);
    pthread_cond_broadcast(&mycond);
    pthread_mutex_unlock(&mymutex);
    for(int i=0;i<THREAD_NUM;i++){
        pthread_join(pthd[i],NULL);
    }
    one->get();//获取结果
    return 0;
}
