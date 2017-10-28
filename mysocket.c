#ifndef MYSOCKET_C
#define MYSOCKET_C 0

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>

int Socket(int domain, int type, int protocol){
	int sockfd = socket(domain,type,protocol);
	if(sockfd==-1){
		printf("ERROR: socket create error\n");
		exit(1);
	}
}
int Bind(int sockfd,struct sockaddr* addr,int size){
	int r = bind(sockfd,addr,size);
	if(r==-1){
		printf("ERROR: bind error.\n");
		exit(1);
	}
	return r;
}
int Listen(int sockfd,int backlog){
	int r = listen(sockfd,backlog);
	if(r==-1){
		printf("ERROR: listen error.\n");
		exit(1);
	}
	return r;
}
int Accept(int sockfd,struct sockaddr* addr,int size){
	int cfd = accept(sockfd,addr,size);
	if(cfd==-1){
		printf("ERROR: accept error.\n");
		//exit(1);		
	}
	return cfd;
}

int CreateServer(int port,int backlog){
	int sockfd = Socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serveraddr;
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = INADDR_ANY;// TODO
	Bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	Listen(sockfd,backlog);
}
#endif