#ifndef ENCODE_C
#define ENCODE_C 0

#include <stdlib.h>
#include <time.h>


char* createToken(int len){
	srand(time(NULL));
	char* data = (char*)malloc(len);
	for(int i=0;i<len-1;i++){	
		data[i] = (char)(rand()%64+48);
		if(data[i]>57)data[i] += 65-57;
		if(data[i]>90)data[i] += 97-90;
	}
	data[len-1] = '\0';
	return data;
}

char* encodePassword(char* password){
	return password;
}

void print16(char* data,int size){
	for(int i=0;i<size;i++){
		printf("%02x ", data[i]);
	}
	printf("\n");
}
void printAscii(char* data,int size){
	for(int i=0;i<size;i++){
		printf("%c ", data[i]);
	}
	printf("\n");
}
#endif