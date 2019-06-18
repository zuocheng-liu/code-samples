#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <netdb.h>

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#define PROGRAM_VERSION "1.0"
#define PROGRAM_NAME "BoolBench"
#define REQUEST_SIZE 2048

class Thread {
    public :
        Thread():_tid(0) {}
        virtual ~Thread() {}
        void start() {
            int res = pthread_create(&_tid, NULL, _StartThread, (void *)this);
            if (res != 0) {
                std::cerr << "Fatal: thread created fail!" << std::endl;
                ::exit(-1);
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
        static void * _StartThread(void *arg) {
            Thread *ptr = (Thread *)arg;
            ptr->run();
            return NULL;
        }

        pthread_t _tid;
}; // end class Thread

class ThreadPool {
    public :
        typedef std::vector<Thread *> ThreadTypePtrContainer;
        
        ThreadPool() : _idle(0), _threadNum(0) {}

        ~ThreadPool() {}

        inline void addThread(Thread* threadPtr) {
            _threadContainer.push_back(threadPtr);
            ++_threadNum;
        }

        
        inline void startAll() {
            ThreadTypePtrContainer::iterator it;
            for(it = _threadContainer.begin(); it != _threadContainer.end(); ++ it) {
                (*it)->start();
            }
        }

        inline void stopAll() {
            ThreadTypePtrContainer::iterator it;
            for(it = _threadContainer.begin(); it != _threadContainer.end(); ++ it) {
                (*it)->join(NULL);
                //delete *it;
            }
        }
        
    protected :
        ThreadTypePtrContainer _threadContainer;
        u_int32_t _threadNum;
        u_int32_t _idle; // the number of idle threads
}; 
volatile int consume = 0;
// end Thread Pool

class Settings {
    public :
        const static uint32_t DEFAULT_THREAD_NUM = 100;

        std::string fileName; //file which contains request log
        std::string fileType; // 
        std::string ip;
        uint16_t port;
        std::string portString;
        std::string serverName;
        std::string host;
        std::string urlPrefix;
        std::string regularExpression;
        std::string domainName;
        uint32_t threadNum;
        struct sockaddr_in ad;
        
        volatile bool stop;
       
        pthread_cond_t fullCond;
        pthread_cond_t emptyCond;
        pthread_mutex_t mutex;
        
        uint32_t totalSendSucc;      
        uint32_t totalSendFail;      
        Settings() : 
            threadNum(DEFAULT_THREAD_NUM), 
            port(80), 
            stop(false),
            totalSendSucc(0),
            totalSendFail(0) {

            portString = "80";
            memset(&ad, 0, sizeof(ad));
            
            pthread_mutex_init(&mutex, NULL);
            pthread_cond_init(&fullCond, NULL);
            pthread_cond_init(&emptyCond, NULL);

        }
        ~Settings() {
            pthread_mutex_destroy(&mutex);
            pthread_cond_destroy(&fullCond);
            pthread_cond_destroy(&emptyCond);
        }
        
        void processParams(int argc, char **argv) {
            int c;
            if (argc < 2) {
                _usage();
                exit(0);
            }
            while ((c = getopt(argc, argv, "hf:H:u:c:t:")) != -1) {
                switch (c) {
                    case 'f' :
                        fileName.assign(optarg);
                        break;
                    case 'H' :
                        host.assign(optarg);
                        break;
                    case 'u' :
                        urlPrefix.assign(optarg);
                        break;
                    case 'c' :
                        threadNum = atoi(optarg);
                        if (threadNum <= 0) {
                            threadNum = DEFAULT_THREAD_NUM;
                        }
                        break;
                    case 'h' :
                        _usage();
                        exit(0);
                } // end switch
            } // end while

            // parase Domain name
            if (urlPrefix.empty()) {
                std::cerr <<"Fatal: Illegal url prefix" << std::endl;
                exit(-1);
            }
            size_t domainStartPos = std::string::npos;
            size_t domainEndPos = std::string::npos;
            size_t colonsPos = std::string::npos;
            size_t prefixPos = std::string::npos;
            if (std::string::npos == urlPrefix.find("http://")) {
                urlPrefix = "http://" + urlPrefix;
            }
            domainStartPos = 7;
            prefixPos = urlPrefix.find("/", domainStartPos);
            if (std::string::npos != prefixPos) {
                urlPrefix = urlPrefix.substr(0, prefixPos);
            }
            colonsPos = urlPrefix.find(":", domainStartPos);
            // case for http://www.bullsoft.org
            if (std::string::npos == colonsPos) {
                domainEndPos = urlPrefix.length() - 1;
            // case for http://www.bullsoft.org:8080
            } else {
                domainEndPos = colonsPos - 1;
            }
            domainName.assign(urlPrefix.substr(domainStartPos, domainEndPos - domainStartPos + 1));
            if (domainName.empty()) {
                std::cerr <<"Fatal: Illegal url prefix" << std::endl;
                exit(-1);
            }

            // parse Port
            // case for :8080
            if (std::string::npos != colonsPos) {
                size_t portStartPos = colonsPos + 1;
                size_t portEndPos = urlPrefix.length() - 1;
                portString = urlPrefix.substr(portStartPos, portEndPos - portStartPos + 1);
                port = atoi(portString.c_str());
                if (port <= 0) {
                    std::cerr <<"Error: Illegal Port:"<< portString << std::endl;
                    port = 80;
                    portString = "80";
                }
            }

            // get IP adin according to domain name
            struct hostent *hp;
            unsigned long inaddr;
            inaddr = inet_addr(domainName.c_str());
            // case for http://127.0.0.1:8080
            if (inaddr != INADDR_NONE) {
                ip = domainName;
                memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
            } else {
                // case for http://www.bullsoft.org
                hp = gethostbyname(domainName.c_str());
                if (hp == NULL) {
                    std::cerr <<"Fatal: Invalid domain name:"<< domainName << std::endl;
                    exit(-1);
                }
                memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);
            }
            ad.sin_family = AF_INET;
            ad.sin_port = htons((unsigned short)port);
        }
       
    private :
        void _usage() {
            std::cout << std::endl << PROGRAM_NAME " " PROGRAM_VERSION 
            << std::endl << "-f <filename> request file path, maybe nginx access log"
            << std::endl << "-t <num>      request file type, 1 for nginx access log, 0 for others , default is 1"
            << std::endl << "-u <url>      request url prefix, unsuport https, such as http://www.bullsoft.org"
            << std::endl << "-H <host>     http request header Host, default is NULL"
            << std::endl << "-c <num>      the number of concurrent threads, default is 100"
            << std::endl << "-h            print this help and exit"
            << std::endl
            //<< std::endl printf("-r            regex expression, used to extract request string from log file \n"); 
            << std::endl << "Examples 1:   ./boolbench -f /var/log/nginx/access.log -u http://127.0.0.1:8080"
            << std::endl << "Examples 2:   ./boolbench -f /var/log/nginx/access.log -u http://127.0.0.1:8080 -H www.bullsoft.org" 
            << std::endl;
        }
};
// end class Settings

class BoolBenchThread : public Thread {
    public:
        BoolBenchThread(Settings& settings, std::queue<std::string>& requestQueue) 
            : _settings(settings), _requestQueue(requestQueue) {}
        ~BoolBenchThread() {}; 
        
        void run() {
            int succ = 0;
            for (;;) {
                pthread_mutex_lock(&_settings.mutex);
                if (succ > 0) _settings.totalSendSucc ++;
                if (succ < 0) _settings.totalSendFail ++;
                if (succ > 0 && _settings.totalSendSucc % 1000 == 0) {
                    std::cout<<"send request uri count:" << _settings.totalSendSucc << std::endl;
                }
                if (_requestQueue.empty()) {
                    if (_settings.stop) {
                        pthread_mutex_unlock(&_settings.mutex);
                        exit(NULL);
                    }
                    pthread_cond_broadcast(&_settings.emptyCond);
                    pthread_cond_wait(&_settings.fullCond, &_settings.mutex);
                    pthread_mutex_unlock(&_settings.mutex);
                    continue;
                }
                
                std::string url = _requestQueue.front();
                _requestQueue.pop();
                pthread_mutex_unlock(&_settings.mutex);
                                
                char request[REQUEST_SIZE];
                buildRequest(request, url);
                int rlen = strlen(request);
                
                int sock = _getSocket();
                if (sock < 0) {
                    std::cerr << "Error: sock fail :" 
                        << _settings.domainName << ':' << _settings.port 
                        << std::endl;
                    succ = -1;
                    continue;
                }
                if(rlen!=write(sock,request,rlen)) {
                    close(sock);
                    std::cerr<<"Error: write fail :" << std::endl;
                    succ = -1;
                    continue;
                }
                // Read all
                char response[8192];
                while (read(sock,response,8192) > 0) {
                    // do nothing;
                }
                close(sock);
                succ = 1;
            } // end for
        }
    private :
        int _getSocket() {
            int sock;
            sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                return -1;
            }
            if (connect(sock, (struct sockaddr *)&(_settings.ad), sizeof(_settings.ad)) < 0) {
                return -1;
            }
            return sock;
        }

        void buildRequest(char* request,std::string& uri) {
            //Five_comment: same as memset bytes to sezo('\0')
            bzero(request,REQUEST_SIZE);
            
            strcpy(request,"GET");
            strcat(request," ");
            
            if (_settings.host.empty()) {
                strcat(request, (char *)uri.c_str());
            } else {
                strcat(request, (char *)_settings.urlPrefix.c_str());
                strcat(request, (char *)uri.c_str());
            }

            // detemine which http version
            strcat(request," HTTP/1.1");
            strcat(request,"\r\n");
            strcat(request,"User-Agent: "PROGRAM_NAME" "PROGRAM_VERSION"\r\n");
            strcat(request,"Host: ");
            if (_settings.host.empty()) {
                if (80 == _settings.port) {
                    strcat(request, _settings.domainName.c_str());
                } else {
                    strcat(request, (_settings.domainName + ":" + _settings.portString).c_str());
                }
            } else {
                strcat(request, _settings.host.c_str());
            }
            strcat(request,"\r\n");
            strcat(request,"Pragma: no-cache\r\n");
            strcat(request,"Connection: close\r\n");
            //strcat(request,"Connection: keep-alive\r\n");
            strcat(request,"Keep-Alive: timeout=20\r\n");
            strcat(request,"\r\n"); 
        }

        Settings& _settings;
        std::queue<std::string>& _requestQueue;
};
// end class BoolBenchThread

int main(int argc, char **argv) {
    Settings settings;
    settings.processParams(argc, argv);
    std::queue<std::string> requestQueue;
    
    ThreadPool threadPool;
    for (int i = settings.threadNum; i > 0; --i) {
        threadPool.addThread(new BoolBenchThread(settings, requestQueue));
    }
    threadPool.startAll();
       
    int count = 0;
    uint32_t bufferSize = 2 * settings.threadNum;
    std::ifstream sourceFile(settings.fileName.c_str());
    while( ! sourceFile.eof() ) {
        std::string line;
        getline(sourceFile, line);
        if (line.empty() || line.length() < 10 || line.length() > 1024) {
            continue;
        }
        size_t posGET = 0;
        posGET = line.find("GET ");
        if (std::string::npos == posGET) {
            continue;
        }
        size_t posHTTP = 0;
        posHTTP = line.find(" HTTP");
        if (std::string::npos == posHTTP) {
            continue;
        }
        size_t posRequest = posGET + 4;
        std::string requestUri = line.substr(posRequest, posHTTP - posRequest); 
        if (requestUri.empty() || line.length() <= 0) {
            continue;
        }
        ++count;
        if (++count % 100 == 0) {
            std::cout<<"get request uri count:" << count << std::endl;
        }
        pthread_mutex_lock(&settings.mutex);
        if (bufferSize <= requestQueue.size()) {
            pthread_cond_broadcast(&settings.fullCond);
            pthread_cond_wait(&settings.emptyCond, &settings.mutex);
        }
        requestQueue.push(requestUri);
        pthread_mutex_unlock(&settings.mutex); 
    }

    settings.stop = true;
    pthread_cond_broadcast(&settings.fullCond);
    threadPool.stopAll();
    
    std::cout<<std::endl;
    std::cout<<"total request uri get:" << count << std::endl;
    std::cout<<"total request uri sent succ:" << settings.totalSendSucc << std::endl;
    std::cout<<"total request uri sent fail:" << settings.totalSendFail << std::endl;
    sourceFile.close();
    exit(0);
    //threadPool.stopAll();
}
