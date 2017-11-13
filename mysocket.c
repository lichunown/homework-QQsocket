#ifndef MYSOCKET_C
#define MYSOCKET_C 0

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <string.h>

#include "encode.c"
int Socket(int domain, int type, int protocol){
	int sockfd = socket(domain,type,protocol);
	if(sockfd==-1){
		printf("ERROR: socket create error\n");
		exit(1);
	}
	return sockfd;
}
int Bind(int sockfd,struct sockaddr* addr,socklen_t size){
	int r = bind(sockfd,addr,size);
	if(r < 0){
		printf("ERROR: bind error.\n");
		exit(1);
	}
	return r;
}
int Listen(int sockfd,int backlog){
	int r = listen(sockfd,backlog);
	if(r < 0){
		printf("ERROR: listen error.\n");
		exit(1);
	}
	return r;
}
int Accept(int sockfd,struct sockaddr* addr,socklen_t* size){
	int cfd = accept(sockfd,addr,size);
	if(cfd==-1){
		printf("ERROR: accept error.\n");
		//exit(1);		
	}
	return cfd;
}
int Connect(int sockfd,struct sockaddr* addr,int size){
	int r = connect(sockfd,addr,size);
	if(r==-1){
		printf("socket connect error.\n");
		exit(1);
	}
	return r;
}

int CreateClient(char* serverip,int port){
	int sockfd = Socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(serverip);// TODO
	Connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	return sockfd;
}
int CreateServer(int port,int backlog){
	int sockfd = Socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");// TODO inet_aton 
	Bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	Listen(sockfd,backlog);
	return sockfd;
}

ssize_t Send(int sockfd, const void *buf, size_t len, int flags){
	printf("sending:\n");
	print16((char*)buf,len);
	printf("\n\n");
	ssize_t n = send(sockfd,buf,len,flags);
	if(n!=len)printf("send not finish.\t %ld/%ld\n",n,len);
	return n;
}


ssize_t Recv(int sockfd, void *buf, size_t len, int flags){
	ssize_t n = recv(sockfd,buf,len,flags);
	printf("recving:\n");
	print16((char*)buf,n);
	printf("\n\n");	
	if(n!=len)printf("send not finish.\t %ld/%ld\n",n,len);
	return n;
}

#endif