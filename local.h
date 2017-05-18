
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <error.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define BUF_SIZE 1024
#define PORT 8080
#define HOST "172.18.169.54"
//#define HOST "127.0.0.1"
#define EPOLL_RUN_TIMEOUT -1 //epoll超时时间
#define EPOLL_SIZE 10000   //EPOLL监听的客户端最大数目

#define STR_WELCOME "Welcome to you! your ID is :#%d"
#define STR_MESSAGE "Client #%d>> %s"
#define STR_NOONE_CONNECTED "Noone connect to server except you"
#define CMD_EXIT "EXIT"

#define CHK(eval)  if(eval < 0) { perror("eval");exit(-1);}
#define CHK2(res,eval)  if( (res =eval) < 0) { perror("eval");exit(-1);} 

int setnonblocking(int sock);
int handle_message(int newfd);
