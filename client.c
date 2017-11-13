#include <stdio.h>
#include "mystring.c"
#include "mysocket.c"
#include "mystruct.c"
#include "encode.c"
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#define DEBUG true

void client_login(int sockfd,char* data);
void client_signup(int sockfd,char* data);
void client_logout(int sockfd,char* data);
void client_sendto(int sockfd,char* data);

void mainReadLoop(int sockfd);
void mainWriteLoop(int sockfd);

void checkresponse(int sockfd, struct HEAD_RETURN* receiveHead);
void login_ok(struct server_login_return* data);

void checkcmd(int sockfd,char** splitdata){
	printf("cmd: `%s`\n",splitdata[0]);
	if(strcmp(splitdata[0],"#login")==0){
		client_login(sockfd,splitdata[1]);// TODO
	}else if(strcmp(splitdata[0],"#signup")==0){
		client_signup(sockfd,splitdata[1]);// TODO
	}else if(strcmp(splitdata[0],"#logout")==0){
		client_logout(sockfd,splitdata[1]);// TODO
	}else if(strcmp(splitdata[0],"#exit")==0){
		puts("exiting");
		close(sockfd);
		kill(0,9);
		exit(0);
	}else if(strcmp(splitdata[0],"#sendto")==0){
		client_sendto(sockfd,splitdata[1]);// TODO
	}else{
		printf("please use cmds:\n\t\t`#login` `#signup` `#logout` `#exit` `#sendto`\n");
	}
}
void mainReadLoop(int sockfd){
	struct HEAD_RETURN receiveHead;
	while(1){
		bzero(&receiveHead,sizeof(receiveHead));
		Recv(sockfd,&receiveHead,sizeof(receiveHead),MSG_WAITALL);
		checkresponse(sockfd,&receiveHead);
	}
}
void checkresponse(int sockfd, struct HEAD_RETURN* receiveHead){
	void* data = NULL;
	if(receiveHead->datalen != 0){
		data = malloc(receiveHead->datalen);
		Recv(sockfd,data,receiveHead->datalen,MSG_WAITALL);
		printf("recv str:`%s`\n",(char*)data);
	}
	if(receiveHead->succ==0){
		printf("[message] cmd succ\n");
	}else{
		printf("[message] cmd fail\n");
	}if(receiveHead->mode == 11 && receiveHead->succ == 0){
		assert(receiveHead->datalen == sizeof(server_login_return));
		login_ok(data);
	}else {

	}
	if(data != NULL)free(data);
}
void login_ok(struct server_login_return* data){
	printf("login successful\n");
	printf("\t nickname: `%s`\n",data->nickname);
	printf("\t    token: `%s`\n",data->token);
}

void mainWriteLoop(int sockfd){
	char input[1024];
	puts("");
	while(1){
		// fputs(">>>",stdout);
		fgets(input,sizeof(input),stdin);
		printf("you input: %s\n",input);
		char** splitdata = split(input);
		checkcmd(sockfd,splitdata);
		free_splitdata(splitdata);
		bzero(input,1024);
	}
}


int main(int argv,char* args[]){
	printf("HEAD_USER_ALL:%ld  HEAD_DATA_ALL:%ld\n",sizeof(struct HEAD_USER_ALL),sizeof(struct HEAD_DATA_ALL));
	assert(sizeof(struct HEAD_USER_ALL)==sizeof(struct HEAD_DATA_ALL));
	if(argv != 3){
		printf("please input: `name [ip_address] [port]`\n");
		exit(1);
	}
	printf("you input  [IP]:%s  [PORT]:%s\n",args[1],args[2]);
	int port = atoi(args[2]);
	int sockfd = CreateClient(args[1],port);
	if(sockfd){
		printf("[message]: Connected %s:%s\n\n",args[1],args[2]);
	}
	int pid = fork();
	if(pid==0){// read
		mainReadLoop(sockfd);
	}else{// write
		mainWriteLoop(sockfd);		
	}
	return 0;
}

void client_signup(int sockfd,char* data){
	char** uandp = split_num(data,3);
	char* username = uandp[0];
	char* password = uandp[1];
	char* nickname = uandp[2];
	printf("you input username:%s password:%s nickname:%s\n",username,password,nickname);
	struct HEAD_USER_ALL* senddata = data_signup(username,password,nickname);
	Send(sockfd,senddata,sizeof(struct HEAD_USER_ALL),0);
	free(senddata);
	free_splitdata_num(uandp,3);
}
void client_login(int sockfd,char* data){// TODO
	char** uandp = split(data);
	char* username = uandp[0];
	char* password = uandp[1];
	printf("you input username:%s password:%s\n",username,password);
	struct HEAD_USER_ALL* senddata = data_login(username,password);
	Send(sockfd,senddata,sizeof(struct HEAD_USER_ALL),0);
	free(senddata);
	free_splitdata(uandp);
}

void client_logout(int sockfd,char* data){

}
void client_sendto(int sockfd,char* data){
	
}