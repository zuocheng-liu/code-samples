#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#define SERVPORT 8080
#define MAXDATASIZE 100

typedef struct hostent {
    char *h_name; // general hostname
    char **h_aliases; // hostname's alias
    int h_addrtype; // AF_INET
    int h_length; 
    char **h_addr_list;
};

int main(int argc, char *argv[]) {
    int sockfd, recvbytes;
    char rcv_buf[MAXDATASIZE]; /*./client 127.0.0.1 hello */
    char snd_buf[MAXDATASIZE];
    struct hostent *host;             

    if (argc < 3)
    {
        printf("Usage:%s [ip address] [any string]\n", argv[0]);
        return 1;
    }

    *snd_buf = '\0';
    strcat(snd_buf, argv[2]);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket:");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVPORT);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
    memset(&(server_addr.sin_zero), 0, 8);

    /* create the connection by socket 
       means that connect "sockfd" to "server_addr"
       同步阻塞模式 
     */
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }

    /* 同步阻塞模式  */
    if (send(sockfd, snd_buf, sizeof(snd_buf), 0) == -1)
    {
        perror("send:");
        exit(1);
    }
    printf("send:%s\n", snd_buf);

    /* 同步阻塞模式  */
    if ((recvbytes = recv(sockfd, rcv_buf, MAXDATASIZE, 0)) == -1)
    {
        perror("recv:");
        exit(1);
    }

    rcv_buf[recvbytes] = '\0';
    printf("recv:%s\n", rcv_buf);

    close(sockfd);
    return 0;
}
