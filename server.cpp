

#include "local.h"
#include "utils.h"
#include <list>
using namespace std;

list<int> clients_list;

int main(int argc,char* argv[])
{
  int listener;
	struct sockaddr_in addr,their_addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(HOST);
	socklen_t socklen;
	socklen = sizeof(struct sockaddr_in);
	static struct epoll_event ev,events[EPOLL_SIZE];
	ev.events = EPOLLIN | EPOLLET;

	char message[BUF_SIZE];
	int epfd;
	clock_t tStart;
	int client,res,epoll_events_count;
	CHK2(listener,socket(PF_INET,SOCK_STREAM,0));
	setnonblocking(listener);
	CHK(bind(listener,(struct sockaddr *)&addr,sizeof(addr)));
  CHK(listen(listener,1));

 CHK2(epfd,epoll_create(EPOLL_SIZE));
 ev.data.fd = listener;
 CHK(epoll_ctl(epfd,EPOLL_CTL_ADD,listener,&ev));

 while(1)
 {
				printf("wait\n");
				CHK2(epoll_events_count,epoll_wait(epfd,events,EPOLL_SIZE,EPOLL_RUN_TIMEOUT));
				tStart = clock();
				for(int i=0;i<epoll_events_count;i++)
				{
				 if(events[i].data.fd == listener)
				 {
				   CHK2(client,accept(listener,(struct sockaddr*)&their_addr,&socklen));
					 setnonblocking(client);

					 ev.data.fd = client;
					 CHK(epoll_ctl(epfd,EPOLL_CTL_ADD,client,&ev));

					 clients_list.push_back(client);
					 bzero(message,BUF_SIZE);
				   res = sprintf(message,STR_WELCOME,client);
					 CHK2(res,send(client,message,BUF_SIZE,0));

				 }
				 else
				 {
				    CHK2(res,handle_message(events[i].data.fd));
				 }
				}
				
				printf("Statistics:%d events handled at:%.2f seconds",epoll_events_count,(double)(clock()-tStart)/CLOCKS_PER_SEC);

 }
 close(listener);
 close(epfd);

}

int handle_message(int client)
{
  char buf[BUF_SIZE],message[BUF_SIZE];

	bzero(buf,BUF_SIZE);
	bzero(message,BUF_SIZE);
	int len;

	CHK2(len,recv(client,buf,BUF_SIZE,0));
	printf("recv client #%d:%s\n",client,buf);
	if(len == 0)
	{
     CHK(close(client));
		 clients_list.remove(client);

	}
	else
	{
	  if(clients_list.size() == 1)
		{
			
		  CHK(send(client,STR_NOONE_CONNECTED,strlen(STR_NOONE_CONNECTED),0));
			return len;
		}

		sprintf(message,STR_MESSAGE,client,buf);
		list<int>::iterator it;
		for(it = clients_list.begin();it!=clients_list.end();it++)
		{
		  if(*it != client)
			{
			  CHK(send(*it,message,BUF_SIZE,0));
			}
		}
	}
	return len;
}

