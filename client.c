#include <stdio.h>
#include "mystring.c"
#include "mysocket.c"
#include "mystruct.c"
#include "encode.c"
#define DEBUG true

void client_login(int sockfd,char* data);
void client_signup(int sockfd,char* data);
void client_signout(int sockfd,char* data);



void checkcmd(int sockfd,char** splitdata){
	printf("cmd: `%s`\n",splitdata[0]);
	if(strcmp(splitdata[0],"#login")==0){
		client_login(sockfd,splitdata[1]);// TODO
	}else if(strcmp(splitdata[0],"#signup")==0){
		client_signup(sockfd,splitdata[1]);// TODO
	}else if(strcmp(splitdata[0],"#logout")==0){

	}else if(strcmp(splitdata[0],"#exit")==0){
		puts("exiting");
		exit(0);
	}else if(strcmp(splitdata[0],"#sendto")==0){

	}else{
		printf("please use cmds:\n\t\t`#login` `#signup` `#logout` `#exit` `#sendto`\n");
	}
}
void mainReadLoop(int sockfd){
	char getdatas[1024];
	while(1){
		read(sockfd,getdatas,sizeof(sockfd));
		fputs(getdatas,stdout);
	}
}

void mainWriteLoop(int sockfd){
	char input[1024];
	while(1){
		fgets(input,1024,stdin);
		printf("you input: %s\n",input);
		char** splitdata = split(input);
		checkcmd(sockfd,splitdata);
		free_splitdata(splitdata);
		bzero(input,1024);
	}
}


int main(int argv,char* args[]){
	if(argv != 3){
		printf("please input: `name [ip_address] [port]`\n");
		exit(1);
	}
	int port = atoi(args[2]);
	int sockfd = CreateClient(args[1],port);
	if(sockfd){
		printf("[message]: Connected %s:%s\n\n\n",args[1],args[2]);
	}
	int pid = fork();
	if(pid==0){// read
		mainReadLoop(sockfd);
	}else{// write
		mainWriteLoop(sockfd);
	}
	return 0;
}

void client_login(int sockfd,char* data){
	char** uandp = split(data);
	char* username = uandp[0];
	char* password = uandp[1];
	struct HEAD_USER_ALL* senddata = data_login(username,password);
	#ifdef DEBUG
	printf("[sending]:");
	print16((char*)senddata,sizeof(struct HEAD_USER_ALL));
	#endif
	write(sockfd,(char*)senddata,sizeof(senddata));
	free(senddata);
	free_splitdata(uandp);
}
void client_signup(int sockfd,char* data){// TODO
	char** uandp = split(data);
	char* username = uandp[0];
	char* password = uandp[1];
	struct HEAD_USER_ALL* senddata = data_login(username,password);
	#ifdef DEBUG
	printf("[sending]:");
	print16((char*)senddata,sizeof(struct HEAD_USER_ALL));
	#endif
	write(sockfd,(char*)senddata,sizeof(senddata));
	free(senddata);
	free_splitdata(uandp);
}