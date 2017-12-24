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
#include "filetransport.c"
#include <stdlib.h>
#include <signal.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "my.h"

struct LOGDATA{
	char logstatus;
	char username[16];
	char token[TOKENSIZE];
	char nickname[16]; 
};

struct LOGDATA* mlogdata;

void client_login(int sockfd,char* data);
void client_signup(int sockfd,char* data);
void client_logout(int sockfd,char* data);
void client_sendto(int sockfd,char* data);
void client_test(int sockfd,char* data);
void client_showlist(int sockfd,char* token);

void mainRecvLoop(int sockfd,struct LOGDATA* logdata);
void mainInputLoop(int sockfd,struct LOGDATA* logdata);

void checkcmd(int sockfd,char** splitdata);



int shmid;

void bekilled(int n){
	shmctl(shmid,IPC_RMID,NULL);
	signal(SIGINT,SIG_DFL);
	kill(0,9);
	exit(0);
}

int main(int argv,char* args[]){
    if(access(CLIENTDATAPATH,0)==-1){
        if (mkdir(CLIENTDATAPATH,0777)){//如果不存在就用mkdir函数来创建  
            printf("creat file bag failed!!!\n");  
        }  	
    }	
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
	shmid = shmget(IPC_PRIVATE, sizeof(struct LOGDATA), 0666 | IPC_EXCL);
	if(shmid<0){
		printf("ERROR: shmid = %d\n",shmid);
		exit(1);
	}
	int pid = fork();
	if(pid ==0){//recv
		mlogdata = shmat(shmid, 0, 0);
		if((long)mlogdata == -1){
			printf("recv shmat error.\n");
			exit(1);
		}		
		// printf("recv progress: %p\n",mlogdata);
		mlogdata->logstatus = 0;
		mainRecvLoop(sockfd,mlogdata);
		
	}else{//write
		mlogdata = shmat(shmid, 0, 0);
		if((long)mlogdata == -1){
			printf("input shmat error.\n");
			exit(1);
		}
		// printf("input progress: %p\n",mlogdata);
		mainInputLoop(sockfd,mlogdata);	
	}
	shmctl(shmid,IPC_RMID,NULL);

	return 0;
}

void mainRecvLoop(int sockfd,struct LOGDATA* logdata){
	int returnmode=0;
	char* nickname = malloc(16);
	char* token = malloc(TOKENSIZE);
	while(1){
		returnmode = client_recv(sockfd,&nickname,&token);
		if(returnmode==1){
			logdata->logstatus = 1;
			strcpy(logdata->nickname,nickname);
			strcpy(logdata->token,token);
		}else if(returnmode==-1){
			logdata->logstatus = 0;
		}else{

		}
	}
}


void mainInputLoop(int sockfd,struct LOGDATA* logdata){
	while(1){
		usleep(100*1000);
		printf(">>> ");
		char* input = (char*)malloc(200);
		bzero(input,200);
		fgets(input,200,stdin);
		char** result = split(input);
		if(strcmp(result[0],"#login")==0){
			char** data = split_num(result[1],2);
			strcpy(logdata->username, data[0]);
			printf("login username:`%s`   password:`%s`   ...\n",data[0],data[1]);
			struct HEAD_USER_ALL* senddata = data_login(data[0],data[1]);
			Send(sockfd,senddata,sizeof(struct HEAD_USER_ALL),0);
			free_splitdata_num(data,2);
			free(senddata);
		}else if(strcmp(result[0],"#signup")==0){
			char** data = split_num(result[1],3);
			printf("login username:`%s`   password:`%s`   nickname:`%s`\n",data[0],data[1],data[2]);
			struct HEAD_USER_ALL* senddata = data_signup(data[0],data[1],data[2]);
			Send(sockfd,senddata,sizeof(struct HEAD_USER_ALL),0);
			free_splitdata_num(data,3);
			free(senddata);
		}else if(strcmp(result[0],"#showlist")==0){
			if(logdata->logstatus==1){
				struct HEAD_MAIN head_main;
				head_main.mode = 1;
				Send(sockfd,&head_main,sizeof(head_main),0);
				struct HEAD_DATA* data = data_showlist(logdata->token);
				Send(sockfd,data,sizeof(struct HEAD_DATA),0);
				free(data);
			}else{
				printf("please login first.\n");
			}
		}else if(strcmp(result[0],"#showfiles")==0){
			if(logdata->logstatus==1){
				struct HEAD_MAIN head_main;
				head_main.mode = 1;
				Send(sockfd,&head_main,sizeof(head_main),0);
				struct HEAD_DATA* data = data_showfile(logdata->token);
				Send(sockfd,data,sizeof(struct HEAD_DATA),0);
				free(data);
			}else{
				printf("please login first.\n");
			}
		}else if(strcmp(result[0],"#logout")==0){
			if(logdata->logstatus==1){
				struct HEAD_MAIN head_main;
				head_main.mode = 1;
				Send(sockfd,&head_main,sizeof(head_main),0);
				struct HEAD_DATA* data = data_logout(logdata->token);
				Send(sockfd,data,sizeof(struct HEAD_DATA),0);
				free(data);
			}else{
				printf("please login first.\n");
			}
		}else if(strcmp(result[0],"#sendto")==0){
			if(logdata->logstatus==1){
				char** userAndSenddata = split_num(result[1],2);
				struct HEAD_MAIN head_main;
				head_main.mode = 1;
				Send(sockfd,&head_main,sizeof(head_main),0);
				struct HEAD_DATA* data_head = data_sendto(logdata->token);
				Send(sockfd,data_head,sizeof(struct HEAD_DATA),0);
				free(data_head);
				printf("sending to `%s`\n",userAndSenddata[0]);
				struct client_to_server_send_to_user_head* send_head = data_sendto_head(userAndSenddata[0],strlen(userAndSenddata[1])+1);
				Send(sockfd,send_head,sizeof(struct client_to_server_send_to_user_head),0);
				Send(sockfd,userAndSenddata[1],strlen(userAndSenddata[1])+1,0);
				free(send_head);
				free_splitdata_num(userAndSenddata,2);
			}else{
				printf("please login first.\n");
			}
		}else if(strcmp(result[0],"#exit")==0){
			close(sockfd);
			kill(0,9);
			exit(0);
		}else if(strcmp(result[0],"#status")==0){
			if(logdata->logstatus==1){
				printf("[status]: login\n");
				printf("          username: `%s` nickname: `%s`\n",logdata->username,logdata->nickname);
				printf("          token: `%s`\n\n",logdata->token);
			}else{
				printf("[status]: not login.\n");
			}
		}else if(strcmp(result[0],"#sendfile")==0){
			if(logdata->logstatus==1){
				char** n = split(result[1]);
				printf("sendfile [%s] ---> %s\n",n[0],n[1]);				
				if(access(n[0],0)==-1){
					printf("[error]: \t%s not exist.\n",n[0]);
					continue;
				}
				client_sendfile(sockfd,logdata->token,n[0],n[1]);
				free_splitdata(n);
			}else{
				printf("[status]: not login.\n");
			}
		}else if(strcmp(result[0],"#recvfile")==0){
			if(logdata->logstatus==1){
				int recvid = 0;
				char** n = split(result[1]);
				recvid = atoi(n[1]);
				printf("recvfile [%s] <--- [%s]  %d\n",n[0],n[0],recvid);				
				client_startrecv_id(sockfd,logdata->token,n[0],recvid,0);
				free_splitdata(n);
			}else{
				printf("[status]: not login.\n");
			}
		}else if(strcmp(result[0],"#help")==0){
			printf("please use cmd follows:\n");
			printf("\t `#login [username] [password]`\n");
			printf("\t `#signup [username] [password] [nickname]`\n");
			printf("\n  if you have login:\n");
			printf("\t `#showlist`\n");
			printf("\t `#logout`\n");
			printf("\t `#sendto [username] [data]`\n");
			printf("\n\n  files:\n");
			printf("\t `#showfiles`\n");
			printf("\t `#sendfile [filename] [dstfilename]`\n");
			printf("\t `#recvfile [filename] [id=0]`\n");
			printf("\n\n  others:\n");
			printf("\t `#status`\n");
			printf("\t `#exit`\n");
		}else{
			printf("use `#help` to show commands.\n");
		}
		free_splitdata(result);
		free(input);
	}
}