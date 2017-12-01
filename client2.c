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
#include "client_recv.c"
#include <stdlib.h>
#include <signal.h>
#include <assert.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#define DEBUG true

void client_login(int sockfd,char* data);
void client_signup(int sockfd,char* data);
void client_logout(int sockfd,char* data);
void client_sendto(int sockfd,char* data);
void client_test(int sockfd,char* data);
void client_showlist(int sockfd,char* token);

void mainReadLoop(int sockfd,struct LOGDATA* logdata);
void mainWriteLoop(int sockfd,struct LOGDATA* logdata);

void checkcmd(int sockfd,char** splitdata);


void checkresponse(int sockfd, struct HEAD_RETURN* receiveHead);
// void login_ok(struct server_login_return* data);

struct LOGDATA{
	char logstatus;
	char username[16];
	char token[TOKENSIZE];
	char nickname[16]; 
};
struct LOGDATA* logdata;
int shmid;

void bekilled(int n){
	shmctl(shmid,IPC_RMID,NULL);
	signal(SIGINT,SIG_DFL);
	kill(0,9);
	exit(0);
}

int main(int argv,char* args[]){
	char* initip = "0.0.0.0";
	int port = 8001;
	if(argv==2){
		port = atoi(args[1]);
	}else if(argv==3){
		port = atoi(args[1]);
		initip = args[2];
	}
	printf("Connecting  [IP]:%s  [PORT]:%d\n",initip,port);
	int sockfd = CreateClient(initip,port);
	signal(SIGINT,bekilled);
	shmid = shmget(IPC_PRIVATE, sizeof(struct LOGDATA), IPC_EXCL);
	if(shmid<0){
		printf("ERROR: shmid = %d\n",shmid);
		exit(1);
	}
	int pid = fork();
	if(pid ==0){//read
		logdata = shmat(shmid, 0, 0);
		logdata->logstatus = 0;
		mainReadLoop(sockfd,logdata);
	}else{//write
		logdata = shmat(shmid, 0, 0);
		mainWriteLoop(sockfd,logdata);		
	}
	shmctl(shmid,IPC_RMID,NULL);

	return 0;
}

void mainReadLoop(int sockfd,struct LOGDATA* logdata){
	int returnmode;
	while(1){
		returnmode = client_recv(sockfd,logdata->nickname,logdata->token);
		if(returnmode==1){
			logdata->logstatus = 1;
		}else if(returnmode==-1){
			logdata->logstatus = 0;
		}
	}
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
		printf("[error]: cmd not found.\n");
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
	}else if(receiveHead->mode == 13 && receiveHead->succ == 0){
		printf("[warning]: logout\nBecause another client login in this username.\n");
	}else if(receiveHead->mode == 22 && receiveHead->succ == 0){
		printf("LOGIN User:   length=%d\n",length);
		for(int i = 0;i<length/sizeof(struct list_per_user);i++){
			struct list_per_user perdata;
			Recv(sockfd,&perdata,sizeof(struct list_per_user),0);
			printf("\t %s: %s\n",perdata.username,perdata.nickname);
		}
	}else if(receiveHead->mode == 99 && receiveHead->succ == 0){
		/*other send to here*/
		struct server_to_client_send_to_user_head* head_data = (struct server_to_client_send_to_user_head*)data;
		printf("Recving data from %s:\n",head_data->username);
		char* recvdata = (char*)malloc(head_data->len);
		Recv(sockfd,recvdata,head_data->len,0);
		printf("%s\n\n",recvdata);
	}else if(receiveHead->mode == 20 && receiveHead->succ == 0){
		/*other send to here*/
		printf("send to other user succeed\n");
	}
	else {
		//printf("Test???\n:`%s`\n",(char*)receiveHead);
	}
	if(data != NULL)free(data);
}



void mainWriteLoop(int sockfd,struct LOGDATA* logdata){
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
	Send(sockfd,senddata,send_head.len,0);
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