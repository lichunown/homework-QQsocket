/**********************************************
author:lcy
采用epoll多路复用。单进程模式。

***********************************************/
#include "mysocket.c"
#include "mystruct.c"
#include "mystring.c"
#include "encode.c"
#include <sys/select.h>
#include <poll.h>  
#include <sys/epoll.h>  
#include <stdlib.h>
#include "client_recv.c"

//最多处理的connect  
#define MAX_EVENTS 100  

void RecvData(int epollfd, int fd);  
void SendData(int epollfd, int fd);

void add_event(int epollfd,int fd,int state);
void delete_event(int epollfd,int fd,int state);
void modify_event(int epollfd,int fd,int state);

char* USERNAME;
char* PASSWORD;
char* NICKNAME;
char* TOKEN;
int status;
int epollfd;

int main(int argv,char* args[]){ 
	assert(sizeof(int)==4);
	assert(sizeof(long)==8);

	USERNAME = (char*)malloc(16);
	PASSWORD = (char*)malloc(16);
	TOKEN = (char*)malloc(TOKENSIZE);
	NICKNAME = (char*)malloc(16);
	status = 0;

	int port = 8001;
	epollfd = epoll_create(MAX_EVENTS);  
	struct epoll_event eventList[MAX_EVENTS];  

    int timeout = 1;  
    if(argv < 2){
    	printf("please use `client IPAdress [port]` to run.\n");
    	exit(0);
    }
    if(argv == 3){
        port = atoi(args[2]);
    }
    int sockfd = CreateClient(args[1],port);

    printf("Connected: %s:%d      sockfd: %d\n",args[1],port,sockfd);

    struct epoll_event event;  
    event.events = EPOLLOUT;  
    event.data.fd = sockfd;  
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event) < 0){  
        printf("epoll add fail : fd = %d\n", sockfd);  
        exit(1);
    }  
    for(;;){                 
        // printf("\n\nstart epoll....\n");
        int ret = epoll_wait(epollfd, eventList, MAX_EVENTS, timeout);  
        if(ret < 0){  
            printf("epoll error\n");  
            break;  
        } 

        for(int i = 0; i < ret; i++){  //错误退出  
            if ((eventList[i].events & EPOLLERR)){  
              printf ("epoll error.\n");  
              close (eventList[i].data.fd);   
            }  
			if(eventList[i].events & EPOLLIN){  
                RecvData(epollfd, eventList[i].data.fd);  
            }else if(eventList[i].events & EPOLLOUT){
            	SendData(epollfd, eventList[i].data.fd);
            }else{

            }
        }  
    }  
    close(epollfd);  
    close(sockfd);  
    return 0;  
}  
void RecvData(int epollfd, int sockfd){
	int result = client_recv(sockfd,&NICKNAME,&TOKEN);
	if(result==1){
		status = 1;
	}else if(result==-1){
		status = 0;
	}
	modify_event(epollfd,sockfd,EPOLLOUT);
}

void SendData(int epollfd, int sockfd){
	printf(">>> ");
	char* input = (char*)malloc(200);
	bzero(input,200);
	fgets(input,200,stdin);
	char** result = split(input);
	if(strcmp(result[0],"#login")==0){
		char** data = split_num(result[1],2);
		USERNAME = data[0];
		PASSWORD = data[1];
		printf("login username:`%s`   password:`%s`   ...\n",data[0],data[1]);
		struct HEAD_USER_ALL* senddata = data_login(data[0],data[1]);
		Send(sockfd,senddata,sizeof(struct HEAD_USER_ALL),0);
		free_splitdata_num(data,2);
		free(senddata);
	}else if(strcmp(result[0],"#signup")==0){
		char** data = split_num(result[1],3);
		printf("login username:`%s`   password:`%s`   nickname:`%s`\n",data[0],data[1],data[2]);
		struct HEAD_USER_ALL* senddata = data_signup(data[0],data[1],data[2]);
		Send(sockfd,senddata,sizeof(struct HEAD_USER_ALL),0);
		free_splitdata_num(data,3);
		free(senddata);
	}else if(strcmp(result[0],"#showlist")==0){
		if(status==1){
			struct HEAD_MAIN head_main;
			head_main.mode = 1;
			Send(sockfd,&head_main,sizeof(head_main),0);
			struct HEAD_DATA* data = data_showlist(TOKEN);
			Send(sockfd,data,sizeof(struct HEAD_DATA),0);
			free(data);
		}else{
			printf("please login first.\n");
		}
	}else if(strcmp(result[0],"#logout")==0){
		if(status==1){
			struct HEAD_MAIN head_main;
			head_main.mode = 1;
			Send(sockfd,&head_main,sizeof(head_main),0);
			struct HEAD_DATA* data = data_logout(TOKEN);
			Send(sockfd,data,sizeof(struct HEAD_DATA),0);
			free(data);
		}else{
			printf("please login first.\n");
		}
	}else if(strcmp(result[0],"#sendto")==0){
		if(status==1){
			char** userAndSenddata = split_num(result[1],2);
			struct HEAD_MAIN head_main;
			head_main.mode = 1;
			Send(sockfd,&head_main,sizeof(head_main),0);
			struct HEAD_DATA* data_head = data_sendto(TOKEN);
			Send(sockfd,data_head,sizeof(struct HEAD_DATA),0);
			free(data_head);
			printf("sending to `%s`\n",userAndSenddata[0]);
			struct client_to_server_send_to_user_head* send_head = data_sendto_head(userAndSenddata[0],strlen(userAndSenddata[1])+1);
			Send(sockfd,send_head,sizeof(struct client_to_server_send_to_user_head),0);
			Send(sockfd,userAndSenddata[1],strlen(userAndSenddata[1])+1,0);
			free(send_head);
			free_splitdata_num(userAndSenddata,2);
		}else{
			printf("please login first.\n");
		}
	}else if(strcmp(result[0],"#exit")==0){
		close(sockfd);
		close(epollfd);
		exit(0);
	}else if(strcmp(result[0],"#status")==0){
		if(status==1){
			printf("[status]: login\n");
			printf("          username: `%s` nickname: `%s`\n",USERNAME,NICKNAME);
			printf("          token: `%s`\n\n",TOKEN);
		}else{
			printf("[status]: not login.\n");
		}
		return;
	}else if(strcmp(result[0],"#help")==0){
		printf("please use cmd follows:\n");
		printf("\t `#login [username] [password]`\n");
		printf("\t `#signup [username] [password] [nickname]`\n");
		return;
	}else{
		printf("\n");
		return;
	}
	free_splitdata(result);
	free(input);
	modify_event(epollfd,sockfd,EPOLLIN);
}


void add_event(int epollfd,int fd,int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

void delete_event(int epollfd,int fd,int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}

void modify_event(int epollfd,int fd,int state){
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}
