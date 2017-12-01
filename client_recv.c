#include "mystruct.c"
#include "mysocket.c"
#include <stdlib.h>
#include <string.h>

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
int client_recv(int sockfd,char** nickname,char** token){
	struct HEAD_RETURN* head_return = client_recv_HEAD_RETURN(sockfd);
	int returnmode = 0;
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