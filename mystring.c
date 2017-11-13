#ifndef MYSTRING_C
#define MYSTRING_C 0
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>


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
		bzero(result[i], PERSTRLENGTH);
	}
	// printf("bzero\n");
	int i = 0;
	int ii = 0;
	char* p = str;
	int spaces = 0;
	while(*p != '\0'){
		printf("%c",*p);
		if(i>=num){
			// printf("break too big");
			break;
		}
		if(ii >= PERSTRLENGTH - 1){
			printf("string too big");
			result[i][ii] = '\0';
			break;
		}
		if(spaces != 0 && (*p == ' ' || *p == '\t' || *p=='\n')){
			// pass
			// printf("   p1\n");
		}else if(*p == ' ' || *p == '\t' || *p=='\n'){
			result[i][ii] = '\0';
			ii = 0;
			i++;
			spaces++;
			// printf("   p2\n");
		}else{
			result[i][ii] = *p;
			ii++;
			spaces = 0;
			// printf("   add\n");
		}
		p++;
	}
	printf("while end\n");
	return result;
}

void free_splitdata(char** data){
	free_splitdata_num(data,2);
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