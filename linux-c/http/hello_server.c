#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8888
#define BUFFER_SIZE 4096
#define RESPONSE_HEADER "HTTP/1.1 200 OK\r\nConnection: close\r\nAccept-Ranges: bytes\r\nContent-Type: text/html\r\n\r\n"
#define RESPONSE_BODY "<h1>Hello!</h1>"

int handle(int conn){
    int len = 0;
    char buffer[BUFFER_SIZE];
    char *pos = buffer;
    bzero(buffer, BUFFER_SIZE);
    len = recv(conn, buffer, BUFFER_SIZE, 0);
    if (len <= 0 ) {
        printf ("recv error");
        return -1;
    } else {
        printf("Debug request:\n--------------\n%s\n\n",buffer);
    }
    
    send(conn, RESPONSE_HEADER RESPONSE_BODY, sizeof(RESPONSE_HEADER RESPONSE_BODY), 0);
    close(conn);//关闭连接
}

int main(int argc,char *argv[]){
    int port = PORT;
    struct sockaddr_in client_sockaddr;     
    struct sockaddr_in server_sockaddr;
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1; 
    int conn;
    socklen_t length = sizeof(struct sockaddr_in);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1){
        printf("bind error!\n");
        return -1;   
    }  

    if(listen(listenfd, 10) < 0) {
        printf("listen failed!\n");
        return -1;   
    }

    while(1){
        conn = accept(listenfd, (struct sockaddr*)&client_sockaddr, &length);
        if(conn < 0){
            printf("connect error!\n");
            continue;
        }
        if (handle(conn) < 0) {
            printf("connect error!\n");
            close(conn);
            continue;
        }  
    }
    return 0;
}
