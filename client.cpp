
#include "local.h"
#include "utils.h"

using namespace std;

char message[BUF_SIZE];

int main(int argc,char* argv[])
{
  int sock,pid,pipe_fd[2],epfd;

	struct sockaddr_in addr;
	addr.sin_family =PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr  = inet_addr(HOST);
	static struct epoll_event ev,events[2];
	ev.events = EPOLLIN|EPOLLET;
	int flags = 1;
	CHK2(sock,socket(PF_INET,SOCK_STREAM,0));
	CHK(connect(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0);
	CHK(pipe(pipe_fd));
	CHK2(epfd,epoll_create(EPOLL_SIZE));
	ev.data.fd = sock;
  CHK(epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev));

	ev.data.fd = pipe_fd[0];

  CHK(epoll_ctl(epfd,EPOLL_CTL_ADD,pipe_fd[0],&ev));

	CHK2(pid,fork());
  switch(pid)
	{
	  case 0:
				close(pipe_fd[0]);
				printf("Enter exit to exit\n");
				while(flags)
				{
				  bzero(&message,BUF_SIZE);
					fgets(message,BUF_SIZE,stdin);

					if(strncasecmp(message,CMD_EXIT,strlen(CMD_EXIT)) == 0)
					{
								flags = 0;
					}
					else
					{
					  CHK(write(pipe_fd[1],message,strlen(message) - 1));
					}
				}
				break;

   default:
				printf("parent process\n");
				close(pipe_fd[1]);
				int count,res;
				while(flags)
				{
				  CHK2(count,epoll_wait(epfd,events,2,EPOLL_RUN_TIMEOUT));
					for(int i=0;i<count;i++)
					{
					  bzero(&message,BUF_SIZE);
						if(events[i].data.fd == sock)
						{
						  CHK2(res,recv(sock,message,BUF_SIZE,0));

							if(res == 0)
							{ 
								CHK(close(sock));
								flags = 0;
							}
							else
							{
							  printf("recv server:%s\n",message);
							}
						}
				   else
					 {
					   CHK2(res,read(events[i].data.fd,message,BUF_SIZE));
						 if(res == 0 )
						 {
						   printf("stdin error\n");
						   flags = 0;
						 }
						 else
						 {
						   CHK(send(sock,message,BUF_SIZE,0));
						 }
					 }

					}
				}
				

	}
	if(pid)
  {
	  close(pipe_fd[0]);
		close(sock);
  }
	else
	{
	  close(pipe_fd[1]);
	}
	return 0;
}
