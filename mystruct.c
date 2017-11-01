#ifndef MYSTRUCT_C
#define MYSTRUCT_C 0
#include "mysocket.c"

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
	char token[32];
	char datamode;
	int datalen;
}HEAD_DATA;

struct HEAD_USER_ALL{
	struct HEAD_MAIN main;
	struct HEAD_USER user;
}
HEAD_USER_ALL* data_login(char* username,char* password){
	struct HEAD_USER_ALL* data = (struct HEAD_USER_ALL*)malloc(sizeof(struct HEAD_USER_ALL));
	bzero(data,sizeof(struct HEAD_USER_ALL));
	data->main.mode = 0;
	data->user.logmode = (char)1;
	strcpy(data->user.username,username);
	strcpy(data->user.password,password);

}
int data_signup(char* username,char* password){
	
}
#endif