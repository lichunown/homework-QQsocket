#ifdef CLIENT_C
#define CLIENT_C 0

#include "mysocket.c"

int main(int argv,char* args[]){
	if(argv != 3){
		printf("please input: `name [ip_address] [port]`\n");
		exit(1);
	}
	int port = atoi(args[2]);
	int sockfd = CreateClient(args[1],port);
	return 0;
}

#endif