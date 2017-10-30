#include <stdio.h>
#include "mystring.c"
#include "mysocket.c"

void checkcmd(int sockfd,char** splitdata){
	if(strcmp(splitdata[0],"#login")==0){

	}else if(strcmp(splitdata[0],"#signup")==0){

	}else if(strcmp(splitdata[0],"#logout")==0){

	}else if(strcmp(splitdata[0],"#exit")==0){

	}else if(strcmp(splitdata[0],"#sendto")==0){

	}else{
		printf("please use cmds:\n\t\t`#login` `#signup` `#logout` `#exit` `#sendto`\n");
	}
}
void mainReadLoop(int sockfd){
	while(1){

	}
}

void mainWriteLoop(int sockfd){
	char input[1024];
	while(1){
		fgets(input,80,stdin);
		printf("you input: %s\n",input);
		char** splitdata = split(input);
		checkcmd(sockfd,splitdata);
		free_splitdata(splitdata);
	}
}


int main(int argv,char* args[]){
	if(argv != 3){
		printf("please input: `name [ip_address] [port]`\n");
		exit(1);
	}
	int port = atoi(args[2]);
	int sockfd = CreateClient(args[1],port);
	if(sockfd){
		printf("[message]: Connected %s:%s\n\n\n",args[1],args[2]);
	}
	int pid = fork();
	if(pid==0){// read
		mainReadLoop(sockfd);
	}else{// write
		mainWriteLoop(sockfd);
	}
	return 0;
}

