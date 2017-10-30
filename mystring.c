#ifndef MYSTRING_C
#define MYSTRING_C 0
#include <stdlib.h>
#define CMDLENGTH 20
#define DATALENGTH 1024
char** split(char* str){
	char** result = (char**)malloc(sizeof(char*)*2);
	result[0] = (char*)malloc(sizeof(char)*CMDLENGTH);
	result[1] = (char*)malloc(sizeof(char)*DATALENGTH);
	char b=(char)0;
	int i = 0;
	while(*str!='\0'){
		if(i >= CMDLENGTH){
			printf("cmd can't bigger than max_length.\n");
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
		result[1][i] = *str;
		str++;i++;
	}
	return result;
}
int free_splitdata(char** data){
	free(data[0]);
	free(data[1]);
	free(data);
}

#endif