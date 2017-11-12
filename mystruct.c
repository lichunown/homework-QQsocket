#ifndef MYSTRUCT_C
#define MYSTRUCT_C 0
#include "mysocket.c"

#define TOKENSIZE 38

struct HEAD_MAIN{
	char mode;
}HEAD_MAIN;

struct HEAD_USER{
	char logmode; 
	char username[16];
	char password[16];
	char nickname[16];
}HEAD_USER;

struct HEAD_DATA{
	char token[TOKENSIZE];
	char datamode;
	int datalen;
}HEAD_DATA;

struct HEAD_USER_ALL{
	struct HEAD_MAIN main;
	struct HEAD_USER user;
}HEAD_USER_ALL;

struct HEAD_DATA_ALL{
	struct HEAD_MAIN main;
	struct HEAD_DATA data;
}HEAD_DATA_ALL;

struct HEAD_RETURN{
	char mode;
	char succ;//是否成功
	int datalen;
}HEAD_RETURN;

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
struct HEAD_DATA_ALL* data_logout(char* token){
	struct HEAD_DATA_ALL* data = (struct HEAD_DATA_ALL*)malloc(sizeof(HEAD_DATA_ALL));
	bzero(data,sizeof(HEAD_DATA_ALL));
	data->main.mode = (char)1;
	memcpy(data->data.token,token,32);
	data->data.datamode = 0;
	return data;
}

struct HEAD_DATA_ALL* data_showlist(char* token){
	struct HEAD_DATA_ALL* data = (struct HEAD_DATA_ALL*)malloc(sizeof(HEAD_DATA_ALL));
	bzero(data,sizeof(HEAD_DATA_ALL));
	data->main.mode = (char)1;
	memcpy(data->data.token,token,32);
	data->data.datamode = 2;
	return data;
}

struct SERVERSENDSTRUCT{
	int sockfd;
	int datalen;
	char* data;
}SERVERSENDSTRUCT;


#endif