#ifndef ENCODE_C
#define ENCODE_C 0

#include <stdlib.h>
#include <time.h>

char* createToken(int len); // 创建长度len-1的随机字符串（最后一位是'\0'）[需要手工free]
void print16(char* data,int size);// 每个字节以16进制输出

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
	unsigned char* p = (unsigned char*)data;
	for(int i=0;i<size;i++){
		printf("%02x ", *p++);
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