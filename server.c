#include "mysocket.c"
#include <sys/select.h>
int main(){
	int sockfd = CreateServer(8080,10);
	struct timeval tv; 
	fd_set server_fd_set;  
	tv.tv_sec = 20;  
	tv.tv_usec = 0;  
	FD_ZERO(&server_fd_set);  
	FD_SET(STDIN_FILENO, &server_fd_set);  

	return 0;
}