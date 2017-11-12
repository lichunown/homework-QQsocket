#ifndef MYSTRING_C
#define MYSTRING_C 0
#include <stdlib.h>
#include <stdio.h>


#define PERSTRLENGTH 200


char** split(char* str);
char** split_num(char* str,int num);
void free_splitdata(char** data);
void free_splitdata_num(char** data,int num);


char** split(char* str){
	return split_num(str,2);
}

char** split_num(char* str,int num){
	char** result = (char**)malloc(sizeof(char*) * num);
	for(int i=0;i<num;i++){
		result[i] = (char*)malloc(PERSTRLENGTH);
		bzero(&(result[i]), PERSTRLENGTH);
	}
	int i = 0;
	int ii = 0;
	char* p = str;
	while(*p != '\0'){
		if(i>=num){
			break;
		}
		if(ii >= PERSTRLENGTH){
			printf("string too big");
			break;
		}
		if(*p == ' '){
			result[i][ii] = '\0';
			ii = 0;
			i++;
		}else if(*p=='\n'){

		}else{
			result[i][ii] = *p;
			ii++;
		}
		p++;
	}
	for(int j=i+1;j<num;j++){
		result[j][0] = '\0';
	}
	result[i][ii] = '\0';
	return result;
}

void free_splitdata(char** data){

	free(data[0]);
	free(data[1]);
	free(data);

}
void free_splitdata_num(char** data,int num){

	for(int i=0;i<num;i++){
		free(data[i]);
	}
	free(data);

}

unsigned long m_pow(int num,int n){
	if(n==0)return 1;
	unsigned long nn = num;
	for(int i = 0;i < n-1; i++){
		nn *= num;
	}
	return nn;
}
char* itoa(int num){
	int len = 1;
	while((num/m_pow(10,len)) > 0 )len++;
	len++;
	char* data = (char*)malloc(len);
	sprintf(data,"%d",num);
	return data;
}
char* ltoa(long num){
	long len = 1;
	while((num/m_pow(10,len)) > 0 )len++;
	len++;
	char* data = (char*)malloc(len);
	sprintf(data,"%ld",num);
	return data;
}
char* ptoa(size_t num){
	unsigned int len = 1;
	while((num/m_pow(10,len)) > 0 )len++;
	len++;
	char* data = (char*)malloc(len);
	sprintf(data,"%ld",num);
	return data;
}
#endif