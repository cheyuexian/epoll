

#include <list>
#include "local.h"
#include "utils.h"
using namespace std;

char message[BUF_SIZE];
list<int>  list_of_clients;
int res;
clock_t tStart;

int main(int argc,char *argv[])
{
   int sock;
	 struct sockaddr_in addr;
	 addr.sin_family = PF_INET;
	 addr.sin_port = htons(PORT);
	 addr.sin_addr.s_addr = inet_addr(HOST);

	 tStart  = clock();
	 for(int i = 0; i < EPOLL_SIZE;i++)
	 {
	   CHK2(sock,socket(PF_INET,SOCK_STREAM,0));
		 CHK(connect(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0);
		 list_of_clients.push_back(sock);

		 bzero(&message,BUF_SIZE);
		 CHK2(res,recv(sock,message,BUF_SIZE,0));
		 printf("recv server:%s\n",message);
	 }

	 list<int>::iterator it;
	 for(it = list_of_clients.begin();it!=list_of_clients.end();it++)
	 {
	    close(*it);
	 }
	 printf("Test passed at: %.2f seconds\n",(double)(clock()-tStart)/CLOCKS_PER_SEC);
	 printf("Total server connectiong was:%d\n",EPOLL_SIZE);
}
