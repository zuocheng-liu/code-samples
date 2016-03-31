#include <sys/epoll.h>
#include <iostream>
#include <exception>
#include <string>
using namespace std;

class EpollException : exception {
    public :
        EpollException();
        EpollException(int code);
        EpollException(string message);
        EpollException(int code, string message);
        inline int getCode() {
            return _code;
        }
        inline string getMessage() {
            return _message;
        }
        inline string getFile() {
            return _file;
        }
        inline int getLine() {
            return _line;
        }

    private :
        int _code;
        string _message;
        flie _file;
        line _line;
};

class EpollEvent {
    public :
        EpollEvent();
        inline void setTypeIn()  { _ev.events |= EPOLLIN }
        inline void setTypeOut() { _ev.events |= EPOLLOUT }  
        inline void setTypeFri() { _ev.events |= EPOLLFRI }  
        inline void setTypeErr() { _ev.events |= EPOLLERR }  
        inline void setTypeHub() { _ev.events |= EPOLLHUB }  
        inline void setTypeET()  { _ev.events |= EPOLLET }
        inline struct epoll_event & getCEvent() {
            return _ev;
        }
    private :
        struct epoll_event _ev;
};

EpollEvent::EpollEvent() {
    _ev.events = 0;
}

class Epoll {
    public :
        Epoll(int size);
        ~Epoll();
        int addEvent(EpollEvent &ev);
        int modifyEvent(EpollEvent &ev);
        int deleteEvent(EpollEvent &ev);
        int wait();
    protected : 
        int _size;
        int _fd;
        struct epoll_event *_evRead;
};


Epoll::Epoll(int size) {
    _size = size;
    _fd = epoll_create(size);
    _evRead = new struct epoll_event[size];
    if (_fd) {
        throw new EpollException();
    }
}

Epoll::~Epoll() {
    close(_fd);
    delete[] _evRead;
}

int Epoll::addEvent(EpollEvent &ev) {
    struct epoll_event &ev =  ev.getCEvent();
    epoll_ctl(_fd,EPOLL_CTL_ADD, ev.data.fd, &ev);
}

int Epoll::modifyEvent(EpollEvent &ev) {
    struct epoll_event &ev =  ev.getCEvent();
    epoll_ctl(_fd,EPOLL_CTL_MOD, ev.data.fd, &ev);
}

int Epoll::deleteEvent(EpollEvent &ev) {
    struct epoll_event &ev =  ev.getCEvent();
    epoll_ctl(_fd, EPOLL_CTL_DEL, ev.data.fd, &ev);
}

int Epoll::wait() {
    int nfds = epoll_wait(_fd,_evRead,20, -1);
}

void func() {
}

int main () {
    Epoll epoll(20);
    EpollEvent event();
    event.setTypeIn();
    epoll.addEvent(event);
    epoll.wait();
}
