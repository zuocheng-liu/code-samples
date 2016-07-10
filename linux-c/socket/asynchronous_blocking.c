#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netdb.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SERVPORT 8080
#define MAXDATASIZE 100
#define TFILE "data_from_socket.txt"


int main(int argc, char *argv[])
{
  int sockfd, recvbytes;
  char rcv_buf[MAXDATASIZE]; /*./client 127.0.0.1 hello */
  char snd_buf[MAXDATASIZE];
  struct hostent *host;             /* struct hostent
                                     * {
                                     * char *h_name; // general hostname
                                     * char **h_aliases; // hostname's alias
                                     * int h_addrtype; // AF_INET
                                     * int h_length; 
                                     * char **h_addr_list;
                                     * };
                                     */
  struct sockaddr_in server_addr;


  /* */
  fd_set readset, writeset;
  int check_timeval = 1;
  struct timeval timeout={check_timeval,0}; //阻塞式select, 等待1秒，1秒轮询
  int maxfd;
  int fp;
  int cir_count = 0;
  int ret;


  if (argc < 3)
  {
    printf("Usage:%s [ip address] [any string]\n", argv[0]);
    return 1;
  }


  *snd_buf = '\0';
  strcat(snd_buf, argv[2]);


  if ((fp = open(TFILE,O_WRONLY)) < 0)    //不是用fopen
  {
    perror("fopen:");
    exit(1);
  }


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
   * means that connect "sockfd" to "server_addr"
   */
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
  {
    perror("connect");
    exit(1);
  }


  /**/
  if (send(sockfd, snd_buf, sizeof(snd_buf), 0) == -1)
  {
    perror("send:");
    exit(1);
  }
  printf("send:%s\n", snd_buf);

  while (1)
  {
    FD_ZERO(&readset);            //每次循环都要清空集合，否则不能检测描述符变化
    FD_SET(sockfd, &readset);     //添加描述符       
    FD_ZERO(&writeset);
    FD_SET(fp,     &writeset);

    maxfd = sockfd > fp ? (sockfd+1) : (fp+1);    //描述符最大值加1

    ret = select(maxfd, &readset, NULL, NULL, NULL);   // 阻塞模式
    switch( ret)
    {
      case -1:
        exit(-1);
        break;
      case 0:
        break;
      default:
        if (FD_ISSET(sockfd, &readset))  //测试sock是否可读，即是否网络上有数据
        {
          recvbytes = recv(sockfd, rcv_buf, MAXDATASIZE, MSG_DONTWAIT);
          rcv_buf[recvbytes] = '\0';
          printf("recv:%s\n", rcv_buf);

          if (FD_ISSET(fp, &writeset))
          {
            write(fp, rcv_buf, strlen(rcv_buf));   // 不是用fwrite
          }
          goto end;
        }
    }
    cir_count++;
    printf("CNT : %d \n",cir_count);
  }

end:
  close(fp);
  close(sockfd);


  return 0;
}