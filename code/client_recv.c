#ifndef CLIENT_RECV_C
#define CLIENT_RECV_C 0

#include "mystruct.c"
#include "mysocket.c"
#include "filetransport.c"
#include "my.h"

struct HEAD_RETURN* client_recv_HEAD_RETURN(int sockfd);
struct server_login_return* client_recv_login_return(int sockfd);
struct server_to_client_send_to_user_head* client_recv_data_return(int sockfd);
struct list_per_user* client_recv_list_return(int sockfd);


int client_recv(int sockfd, char** nickname,char** token);

void message_out(char* data);
void message_out_login_peruser(char* username,char* nickname);

void client_recv_and_out_perlist(int sockfd);
void client_recv_and_out_data(int sockfd);
void message_out_recv_data(char* username,char* data,int len);

struct HEAD_RETURN* client_recv_HEAD_RETURN(int sockfd){
	struct HEAD_RETURN* data = (struct HEAD_RETURN*)malloc(sizeof(struct HEAD_RETURN));
	Recv(sockfd,data,sizeof(struct HEAD_RETURN),0);
	return data;
}

struct server_login_return* client_recv_login_return(int sockfd){
	struct server_login_return* data = (struct server_login_return*)malloc(sizeof(struct server_login_return));
	Recv(sockfd,data,sizeof(struct server_login_return),0);
	return data;
}

struct server_to_client_send_to_user_head* client_recv_data_return(int sockfd){
	struct server_to_client_send_to_user_head* data = (struct server_to_client_send_to_user_head*)malloc(sizeof(struct server_to_client_send_to_user_head));
	Recv(sockfd,data,sizeof(struct server_to_client_send_to_user_head),0);
	return data;
}

struct list_per_user* client_recv_list_return(int sockfd){
	struct list_per_user* data = (struct list_per_user*)malloc(sizeof(struct list_per_user));
	Recv(sockfd,data,sizeof(struct list_per_user),0);
	return data;
}


void client_recv_and_out_perlist(int sockfd){
	struct list_per_user* data = client_recv_list_return(sockfd);
	message_out_login_peruser(data->username,data->nickname);
	free(data);
}
void client_recv_and_out_perfile(int sockfd){
	struct list_per_file* data = malloc(sizeof(struct list_per_file));
	Recv(sockfd,data,sizeof(struct list_per_file),0);
	printf("\t%s\t\t\t\t\t%ld\n",data->filename,data->size);
	free(data);
}
int client_recv(int sockfd,char** nickname,char** token){
	struct HEAD_RETURN* head_return = client_recv_HEAD_RETURN(sockfd);
	int returnmode = 0;
	struct SEND_FILE* filehead;
	switch(head_return->mode){
		case 11://login
			if(head_return->succ==0){
				message_out("[msg]:   login successfull.");
				struct server_login_return* data = client_recv_login_return(sockfd);
				strcpy(*nickname, data->nickname);
				strcpy(*token, data->token);
				returnmode = 1;
			}else{
				message_out("[error]: login error.");
				returnmode = -1;
			}
			break;
		case 12://signup
			if(head_return->succ==0){
				message_out("[msg]:   signup successfull.");
			}else{
				message_out("[error]: signup error.");
				returnmode = -1;
			}		
			break;
		case 13://another user login
			message_out("[error]: another user login.");
			returnmode = -1;
			break;
		case 21://logout（登出）
			if(head_return->succ==0){
				message_out("[msg]:   logout successfull.");
				returnmode = -1;
			}
			break;
		case 22://showlist
			for(int i=0;i < (head_return->datalen)/sizeof(list_per_user);i++){
				client_recv_and_out_perlist(sockfd);
			}
			break;
		case 23:
			printf("server file list:\n");
			for(int i=0;i < (head_return->datalen)/sizeof(struct list_per_file);i++){
				client_recv_and_out_perfile(sockfd);
			}			
			break;
		case 20://send succ
			if(head_return->succ==0){
				message_out("[msg]:   send successfull.");
			}else{
				message_out("[warning]: send error.");
			}
			break;
		case 99://recv data
			client_recv_and_out_data(sockfd);
			break;		
		case 50://token error
			message_out("[error]: token error.\n\ttry login again.");
			returnmode = -1;
			break;	
		case 35:
			if(head_return->succ==0){
				client_recvfile(sockfd,CLIENTDATAPATH,*token);
			}else{
				printf("[recv file]: error: %d\n",head_return->succ);
			}
			break;
		case 30:
			filehead = malloc(sizeof(struct SEND_FILE));
			Recv(sockfd, filehead,sizeof(struct SEND_FILE),0);
			if(head_return->succ==0){
				printf("[sendfile]: ---  >  filename:%s    (id:%d)\t\t[OK]\n",filehead->filename,filehead->id);
			}else{
				printf("[sendfile]: ---  >  filename:%s    (id:%d)\t\t[ERROR]\n",filehead->filename,filehead->id);
			}
			free(filehead);
			break;				
		default:
			break;
	}
	free(head_return);
	return returnmode;
}

void client_recv_and_out_data(int sockfd){
	struct server_to_client_send_to_user_head* headdata = client_recv_data_return(sockfd);
	int datalen = headdata->len;
	void* data = malloc(datalen);
	Recv(sockfd,data,datalen,0);
	message_out_recv_data(headdata->username, (char*)data, datalen);
}

void message_out_recv_data(char* username,char* data,int len){
	char* printdata = (char*)malloc(len + 20);
	sprintf(printdata,"user: %s\n%s",username,data);
	message_out(printdata);
}
void message_out(char* data){
	printf("%s\n",data);
}
void message_out_login_peruser(char* username,char* nickname){
	char data[80];
	sprintf(data,"username: %s    nickname: %s",username,nickname);
	message_out(data);
}

#endif