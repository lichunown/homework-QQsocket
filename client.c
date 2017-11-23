/**********************************************
author:lcy
测试客户端，2个进程，一进程接收数据，另一个进程
发送数据。

***********************************************/


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
void client_test(int sockfd,char* data);
void client_showlist(int sockfd,char* token);

void mainReadLoop(int sockfd);
void mainWriteLoop(int sockfd);

void checkcmd(int sockfd,char** splitdata);


void checkresponse(int sockfd, struct HEAD_RETURN* receiveHead);
void login_ok(struct server_login_return* data);

int LOGINSTATUS = 0;
char* TOKEN = NULL;
char* USERNAME = NULL;
char* NICKNAME = NULL;

int main(int argv,char* args[]){
	TOKEN = (char*)malloc(TOKENSIZE);
	USERNAME = (char*)malloc(16);
	NICKNAME = (char*)malloc(16);


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
	}else if(strcmp(splitdata[0],"#test")==0){
		//client_test(sockfd,splitdata[1]);// TODO
	}else if(strcmp(splitdata[0],"#showlist")==0){
		client_showlist(sockfd,splitdata[1]);
	}else{
		printf("please use cmds:\n\t\t`#login` `#signup` `#logout` `#exit` `#sendto`\n");
	}
}
void mainReadLoop(int sockfd){
	struct HEAD_RETURN receiveHead;
	int n;
	while(1){
		bzero(&receiveHead,sizeof(receiveHead));
		n = Recv(sockfd,&receiveHead,sizeof(receiveHead),0);
		if(n<=0){
			printf("Server Error. \n");
			exit(0);
		}
		checkresponse(sockfd,&receiveHead);
	}
}
void checkresponse(int sockfd, struct HEAD_RETURN* receiveHead){
	void* data = NULL;
	unsigned int length;
	if(receiveHead->datalen != 0){
		length = receiveHead->datalen;
		data = malloc(length);
		Recv(sockfd,data,length,0);
		printf("recv str:`%s`\n",(char*)data);
	}
	if(receiveHead->succ==0){
		printf("[message] cmd succ\n");
	}else{
		printf("[message] cmd fail\n");
	}
	if(receiveHead->mode == 11 && receiveHead->succ == 0){
		assert(length == sizeof(server_login_return));
		login_ok(data);
	}else if(receiveHead->mode == 12 && receiveHead->succ == 0){
		printf("Please login use the new username.");
	}else if(receiveHead->mode == 22 && receiveHead->succ == 0){
		printf("LOGIN User:   length=%d\n",length);
		for(int i = 0;i<length/sizeof(struct list_per_user);i++){
			struct list_per_user perdata;
			Recv(sockfd,&perdata,sizeof(struct list_per_user),0);
			printf("\t %s: %s\n",perdata.username,perdata.nickname);
		}
	}else if(receiveHead->mode == 99 && receiveHead->succ == 0){
		/*other send to here*/
		struct server_to_client_send_to_user_head head_data;
		assert(sizeof(head_data)==receiveHead->datalen);
		Recv(sockfd,&head_data,sizeof(head_data),0);
		printf("Recving data from %s:\n",head_data.username);
		char* data = (char*)malloc(head_data.len);
		Recv(sockfd,data,head_data.len,0);
		printf("%s\n\n",data);
	}else if(receiveHead->mode == 20 && receiveHead->succ == 0){
		/*other send to here*/
		printf("send to other user succeed\n");
	}
	else {
		//printf("Test???\n:`%s`\n",(char*)receiveHead);
	}
	if(data != NULL)free(data);
}
void login_ok(struct server_login_return* data){
	printf("login successful\n");
	// printf("log_ok function:\n");
	// print16((char*)data,sizeof(struct server_login_return));
	// printf("\n");sta
	LOGINSTATUS = 1;
	memcpy(TOKEN,data->token,TOKENSIZE);
	strcpy(NICKNAME,data->nickname);
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
	strcpy(USERNAME,username);
	printf("you input username:%s password:%s\n",username,password);
	struct HEAD_USER_ALL* senddata = data_login(username,password);
	Send(sockfd,senddata,sizeof(struct HEAD_USER_ALL),0);
	free(senddata);
	free_splitdata(uandp);
}
void client_test(int sockfd,char* data){
	//Send(sockfd,"This is a Test string.\n",sizeof("This is a Test string.\n"),0);
}
void client_logout(int sockfd,char* data){

}
void client_sendto(int sockfd,char* data){
	//token sendto_username data
	char** splitdata = split_num(data,3);
	char* token = splitdata[0];
	char* sendto_username = splitdata[1];
	char* senddata = splitdata[2];
	struct HEAD_MAIN head_main;
	head_main.mode = 1;
	Send(sockfd,&head_main,sizeof(head_main),0);
	struct HEAD_DATA head_data;
	bzero(&head_data,sizeof(head_data));
	strcpy(head_data.token,token);
	head_data.datamode = 1;
	head_data.datalen = sizeof(struct client_to_server_send_to_user_head);
	Send(sockfd,&head_data,sizeof(head_data),0);
	struct client_to_server_send_to_user_head send_head;
	bzero(&send_head,sizeof(send_head));
	strcpy(send_head.username,sendto_username);
	send_head.len = strlen(senddata)+1;
	Send(sockfd,&send_head,sizeof(send_head),0);
	Send(sockfd,&senddata,send_head.len,0);
	free_splitdata_num(splitdata,3);
}

void client_showlist(int sockfd,char* token){
	struct HEAD_MAIN headmain;
	headmain.mode = 1;
	Send(sockfd,&headmain,sizeof(headmain),0);
	struct HEAD_DATA headdata;
	bzero(&headdata,sizeof(headdata));
	strcpy(headdata.token,token);	
	headdata.datamode = 2;
	Send(sockfd,&headdata,sizeof(headdata),0);

}