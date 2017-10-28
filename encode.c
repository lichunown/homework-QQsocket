#ifndef ENCODE_C
#define ENCODE_C 0

#include <stdlib.h>
#include <time.h>


char* createToken(int len){
	srand(time(NULL));
	char* data = (char*)malloc(len);
	for(int i=0;i<len-1;i++){	
		data[i] = (char)(rand()%64+48);
	}
	data[len-1] = '\0';
	return data;
}

char* encodePassword(char* password){
	return password;
}
#endif