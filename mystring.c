#ifndef MYSTRING_C
#define MYSTRING_C 0
#include <stdlib.h>
#include <stdio.h>


#define CMDLENGTH 20
#define DATALENGTH 1024
char** split(char* str){
	char** result = (char**)malloc(sizeof(char*)*2);
	result[0] = (char*)malloc(sizeof(char)*CMDLENGTH);
	result[1] = (char*)malloc(sizeof(char)*DATALENGTH);
	int i = 0;
	while(*str!='\0'){
		if(i >= CMDLENGTH){
			printf("cmd can't bigger than max_length.\n");
			break;
		}
		if(*str=='\n'){str++;continue;}
		if(*str=='\0'){
			result[0][i] = *str;
			break;
		}
		result[0][i] = *str;
		str++;
		i++;	
		if(*str==' '){
			result[0][i] = '\0';
			i = 0;
			break;
		}	
	}
	str++;
	while(*str!='\0'){
		if(i >= DATALENGTH){
			printf("cmd can't bigger than max_length.\n");
			break;
		}	
		if(*str=='\n'){str++;continue;}	
		result[1][i] = *str;
		str++;i++;
	}
	result[1][i] = '\0';
	return result;
}

char** split_num(char* str,int num){
	char** result = (char**)malloc(sizeof(char*) * num);
	for(int i=0;i<num;i++){
		result[i] = (char*)malloc(80);
	}
	int i = 0;
	int ii = 0;
	char* p = str;
	while(*p != '\0'){
		if(i>=num){
			break;
		}
		if(ii>=80){
			printf("string too big");
			break;
		}
		if(*p == ' '){
			result[i][ii] = '\0';
			ii = 0;
			i++;
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