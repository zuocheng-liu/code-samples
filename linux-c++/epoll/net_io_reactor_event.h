#pragma once

#include "reactor_event.h"

class NetIOReactorEvent : public ReactorEvent {
public:
    NetIOReactorEvent() {}
    virtual ~NetIOReactorEvent() {}
    virtual int32_t getFileDescripter() override {}
    virtual void trigger() override {}

    void setIpAndPort(const std::string& ip, int32_t port) {
        this->ip = ip;
        this->port = port;
    }

private:
    // 设置句柄为非阻塞方式 
    int32_t setnonblocking(int sockfd) {
        int32_t flag = fcntl(sockfd, F_GETFD, 0);
        flag |= O_NONBLOCK;
        return fcntl(sockfd, F_SETFL, flag);
    }

    std::string ip;
    int32_t port = 0;
    int32_t sockfd = 0;
};
