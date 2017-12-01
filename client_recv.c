#include "mystruct.c"
#include "mysocket.c"
#include <stdlib.h>

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

void client_recv(int sockfd){
	struct HEAD_RETURN* head_return = client_recv_HEAD_RETURN(sockfd);
	switch(head_return->mode){
		case 11://login
			break;
		case 12://signup
			break;
		case 13://another user login
			break;
		case 21://logout（登出）
			break;
		case 22://showlist
			break;
		case 20://send succ
			break;
		case 99://recv data
			break;		
		case 50://token error
			break;					
		default:
			break;
	}
	free(head_return);
}