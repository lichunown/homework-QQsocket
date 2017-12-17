#ifndef MYSTRUCT_C
#define MYSTRUCT_C 0
#include "mysocket.c"
#include <assert.h>
#include "my.h"



/*客户端向服务器发送的头*/
struct HEAD_MAIN{
	char mode;
}HEAD_MAIN;

struct HEAD_USER{
	char logmode; 
	char username[16];
	char password[16];
	char nickname[16];
	// char _blank[6]; // because of offset, it's hard to make 
				    //sure that sizeof HEAD_USER == sizeof HEAD_DATA
}HEAD_USER;

struct HEAD_DATA{
	char token[TOKENSIZE];
	char datamode;
	size_t datalen;
}HEAD_DATA;

struct HEAD_USER_ALL{
	struct HEAD_MAIN main;
	struct HEAD_USER user;
}HEAD_USER_ALL;

// struct HEAD_DATA_ALL{
// 	struct HEAD_MAIN main;
// 	struct HEAD_DATA data;
// }HEAD_DATA_ALL;

struct HEAD_RETURN{
	char mode;
	char succ;//是否成功
	unsigned int datalen;
}HEAD_RETURN;

struct SEND_FILE{
	char filename[32];
	unsigned int id;
	unsigned int perlength;
	unsigned long filelength;
};

/****************************/


struct SEND_DATA{// epoll发送数据，是个链表
	int len;
	void* data;
	struct SEND_DATA* next;
};




struct server_login_return{// 登陆成功后附加数据
    char nickname[16];
    char token[32];
}server_login_return;

struct client_to_server_send_to_user_head{
	char username[16];
	int len;
}client_to_server_send_to_user_head;

struct server_to_client_send_to_user_head{
	char username[16];
	int len;
}server_to_client_send_to_user_head;

struct list_per_user{
	char username[16];
	char nickname[16];
}list_per_user;

/////////
struct file_list{
	char name[32];
	int i;
	unsigned long size;
	struct file_list* next;
};

struct list_per_file{
	char filename[32];
	unsigned long size;
};
// struct HEAD_SEND_BY_FIFO{
// 	size_t len;
// }HEAD_SEND_BY_FIFO;




// struct SERVERSENDSTRUCT{
// 	int sockfd;
// 	int datalen;
// 	void* data;
// }SERVERSENDSTRUCT;



struct HEAD_USER_ALL* data_login(char* username,char* password){
	struct HEAD_USER_ALL* data = (struct HEAD_USER_ALL*)malloc(sizeof(struct HEAD_USER_ALL));
	bzero(data,sizeof(struct HEAD_USER_ALL));
	data->main.mode = 0;
	data->user.logmode = (char)1;
	strcpy(data->user.username,username);
	strcpy(data->user.password,password);
	return data;
}
struct HEAD_USER_ALL* data_signup(char* username,char* password,char* nickname){
	struct HEAD_USER_ALL* data = (struct HEAD_USER_ALL*)malloc(sizeof(struct HEAD_USER_ALL));
	bzero(data,sizeof(struct HEAD_USER_ALL));
	data->main.mode = 0;
	data->user.logmode = (char)0;
	strcpy(data->user.username,username);
	strcpy(data->user.password,password);
	strcpy(data->user.nickname,nickname);
	return data;
}

struct HEAD_DATA* data_HEAD_DATA(char* token, int mode,int len){
	struct HEAD_DATA* data = (struct HEAD_DATA*)malloc(sizeof(struct HEAD_DATA));
	bzero(data,sizeof(struct HEAD_DATA));
	memcpy(data->token,token,TOKENSIZE);
	data->datamode = mode;
	data->datalen = len;
	return data;
}
struct HEAD_DATA* data_logout(char* token){
	return data_HEAD_DATA(token, 0, 0);
}

struct HEAD_DATA* data_showlist(char* token){
	return data_HEAD_DATA(token, 2, 0);
}

struct HEAD_DATA* data_sendto(char* token){
	return data_HEAD_DATA(token, 1, sizeof(struct client_to_server_send_to_user_head));
}

struct HEAD_DATA* data_showfile(char* token){
	return data_HEAD_DATA(token, 3, 0);
}
struct HEAD_DATA* data_recvfile(char* token){
	return data_HEAD_DATA(token, 20, sizeof(struct SEND_FILE));
}
struct HEAD_DATA* data_sendfile(char* token){
	return data_HEAD_DATA(token, 10, sizeof(struct SEND_FILE));
}

struct client_to_server_send_to_user_head* data_sendto_head(char* username,int len){
	struct client_to_server_send_to_user_head* data = (struct client_to_server_send_to_user_head*)malloc(sizeof(struct client_to_server_send_to_user_head));
	bzero(data,sizeof(struct client_to_server_send_to_user_head));
	strcpy(data->username,username);
	data->len = len;
	return data;
}

struct HEAD_RETURN* data_head_return(char mode,char succ,unsigned int datalen){
	struct HEAD_RETURN* data = (struct HEAD_RETURN*)malloc(sizeof(struct HEAD_RETURN));
	bzero(data,sizeof(struct HEAD_RETURN));
	data->mode = mode;
	data->succ = succ;
	data->datalen = datalen;
	return data;
}


// struct HEAD_DATA_ALL* data_logout(char* token){
// 	struct HEAD_DATA_ALL* data = (struct HEAD_DATA_ALL*)malloc(sizeof(HEAD_DATA_ALL));
// 	bzero(data,sizeof(HEAD_DATA_ALL));
// 	data->main.mode = (char)1;
// 	memcpy(data->data.token,token,32);
// 	data->data.datamode = 0;
// 	return data;
// }

// struct HEAD_DATA_ALL* data_showlist(char* token){
// 	struct HEAD_DATA_ALL* data = (struct HEAD_DATA_ALL*)malloc(sizeof(HEAD_DATA_ALL));
// 	bzero(data,sizeof(HEAD_DATA_ALL));
// 	data->main.mode = (char)1;
// 	memcpy(data->data.token,token,32);
// 	data->data.datamode = 2;
// 	return data;
// }




#endif