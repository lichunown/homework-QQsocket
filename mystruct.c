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

int login(char* username,char* password){

}
int signup(char* username,char* password){
	
}
#endif