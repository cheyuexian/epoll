#include "local.h"
#include <fcntl.h>

int setnonblocking(int sockfd)
{
 CHK(fcntl(sockfd,F_SETFL,fcntl(sockfd,F_GETFL,0) | O_NONBLOCK));
 return 0;
}
