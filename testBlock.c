
// 非阻塞模式测试
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
				char buf[10] = {0};
				int ret;
				int flags;
				if(flags = fcntl(STDIN_FILENO,F_GETFL,0) < 0)
				{
								perror("fcntl");
								return -1;
				}
				flags |= O_NONBLOCK;
				if(fcntl(STDIN_FILENO,F_SETFL,flags) < 0)
				{
				   perror("perror fcntl");
					 return -1;
				}
				while(1)
				{
								sleep(2);
								ret = read(STDIN_FILENO,buf,9);
								if(ret == 0)
								{
												perror("read--no");							 
								}
								else
								{
												printf("read=%d\n",ret);
								}

								write(STDOUT_FILENO,buf,10);
								memset(buf,0,10);
								
				}
				return 0;
}
