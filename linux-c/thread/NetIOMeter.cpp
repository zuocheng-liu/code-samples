#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <netdb.h>

#include <iostream>
#include <queue>
#include <vector>

#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.5"
#define REQUEST_SIZE 2048

using namespace std;

class Thread {
    public :
        Thread():_tid(0) {}
        virtual ~Thread() {}
        void start() {
            int res = pthread_create(&_tid, NULL, _StartThread, (void *)this);
            if (res != 0) {
                // print error
            }
        }

        inline pthread_t getId() {return _tid; }
        inline int join(void **status = NULL) {
            return pthread_join(_tid, status);
        }

        inline int detach() {
            return pthread_detach(_tid);
        }

        inline void exit(void *status = NULL) {
            pthread_exit(status);
        }
        virtual void run() = 0;
    protected :
        static void * _StartThread(void *arg);
        pthread_t _tid;
}; // end class Thread

void* Thread::_StartThread(void *arg) {
    Thread *ptr = (Thread *)arg;
    ptr->run();
    return NULL;
}
class ThreadPool {
    public :
        typedef vector<Thread *> ThreadTypePtrContainer;

        ThreadPool() : _idle(0), _threadNum(0) {
            pthread_mutex_init(&_mutex, NULL);
            pthread_cond_init(&_cond, NULL);
        }

        ~ThreadPool() {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_cond);
        }

        inline void addThread(Thread* threadPtr) {
            _threadContainer.push_back(threadPtr);
            ++_threadNum;
        }

        inline void startAll() {
            typename ThreadTypePtrContainer::iterator it;
            for(it = _threadContainer.begin(); it != _threadContainer.end(); ++ it) {
                (*it)->start();
            }
        }

        inline void stopAll() {
            typename ThreadTypePtrContainer::iterator it;
            for(it = _threadContainer.begin(); it != _threadContainer.end(); ++ it) {
                (*it)->exit(NULL);
                delete *it;
            }
        }
    protected :
        ThreadTypePtrContainer _threadContainer;
        u_int32_t _threadNum;
        u_int32_t _idle; // the number of idle threads
        static pthread_cond_t _cond;
        static pthread_mutex_t _mutex;
}; // end Thread Pool

pthread_mutex_t ThreadPool::_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::_cond = PTHREAD_COND_INITIALIZER;

class NetIOMeterStat {
    public :
        NetIOMeterStat() : timerexpired(false) {}
        inline bool isTimerExpired {
            return _timerExpired;
        }
    private :
        volatile bool _timerExpired;
        static pthread_cond_t _cond;
        static pthread_mutex_t _mutex;
};

pthread_mutex_t NetIOMeterStat::_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t NetIOMeterStat::_cond = PTHREAD_COND_INITIALIZER;


class NetIOMeterThread : public Thread {
    public:
        ~NetIOMeterThread() {}
        void run() {
            char request[REQUEST_SIZE];
            int sock = getSocket();
            buildRequest(request);
            int rlen = strlen(request);
            for (int i = 3; i>0; --i) {
                cout<<"write:"<<_tid<<endl;
                if(rlen!=write(sock,request,rlen)) {
                    cout<<"fail:"<<_tid<<endl;
                    close(sock);
                    break;
                }
                char readString[4096];
                //read(sock,readString,4096);
                while (read(sock,readString,4096) > 0) {
                    cout<< readString;
                }
            }
        }
    private :
        void getSocket() {
            const char* host="www.zuocheng.net";
            int sock;
            unsigned long inaddr;
            struct sockaddr_in ad;
            struct hostent *hp;

            memset(&ad, 0, sizeof(ad));
            ad.sin_family = AF_INET;

            inaddr = inet_addr(host);
            if (inaddr != INADDR_NONE) {
                memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
            } else {
                hp = gethostbyname(host);
                if (hp == NULL)
                    return -1;
                memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
            }
            ad.sin_port = htons(80);

            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                return -1;
            }
            if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0) {
                return -1;
            }
            return sock;
        }
        void buildRequest(char* request) {
            //Five_comment: same as memset bytes to sezo('\0')
            bzero(request,REQUEST_SIZE);
            //setting http version
            strcpy(request,"GET");
            strcat(request," ");
            strcat(request,"/param.php");
            // detemine which http version
            strcat(request," HTTP/1.1");
            strcat(request,"\r\n");
            strcat(request,"User-Agent: WebBench "PROGRAM_VERSION"\r\n");
            strcat(request,"Host: ");
            strcat(request,"www.zuocheng.net");
            strcat(request,"\r\n");
            strcat(request,"Pragma: no-cache\r\n");
            strcat(request,"Connection: close\r\n");
            //strcat(request,"Connection: keep-alive\r\n");
            strcat(request,"Keep-Alive: timeout=20\r\n");
            strcat(request,"\r\n"); 
            // printf("Req=%s\n",request);
        }
};

int main() {
    int maxThreadNum = 1;
    ThreadPool threadPool;
    while (maxThreadNum-- > 0) {
        threadPool.addThread(new NetIOMeterThread());
    }
    threadPool.startAll();
    threadPool.stopAll();
}
