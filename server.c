#include "mysocket.c"
#include "mystruct.c"
#include "mystring.c"
#include "sql.c"
#include "encode.c"
#include <sys/select.h>
#include <poll.h>  
#include <sys/epoll.h>  
#include <glib.h>
#include <stdlib.h>
//最多处理的connect  
#define MAX_EVENTS 100  

//数据接受 buf  
#define REVLEN 10  
char recvBuf[REVLEN];  

  
void AcceptConn(int epollfd, int srvfd);  
void RecvData(int epollfd, int fd);  
void SendData(int epollfd, struct epoll_event* event);

void userDataProcess(int epollfd,int sockfd, struct HEAD_USER* data);
void dataDataProcess(int epollfd,int sockfd, struct HEAD_DATA* data);

void server_login(char* username,char* password);
void server_signup(char* username,char* password,char* nickname);
void SendToFd(int epollfd, int sockfd,char* data,int size);

GHashTable* UserTable = NULL;
GHashTable* UserDataTable = NULL;
GHashTable* TokenTable = NULL;
GHashTable* EventTable = NULL;
sqlite3* db = NULL;

int main(int argv,char* args[]){   
	db = databaseInit();
    int port = 8001;
	UserTable = g_hash_table_new(g_str_hash, g_str_equal);//username -> event
	UserDataTable = g_hash_table_new(g_str_hash, g_str_equal);// username -> nickname
	TokenTable = g_hash_table_new(g_str_hash, g_str_equal); //token -> username
	EventTable = g_hash_table_new(g_str_hash, g_str_equal); //sockfd -> event*

	int epollfd = epoll_create(MAX_EVENTS);  
	struct epoll_event eventList[MAX_EVENTS];  

    int timeout = -1;  
    if(argv == 2){
        port = atoi(args[1]);
    }
<<<<<<< HEAD
    int sockListen = CreateServer(port,10);
    printf("listened: %d      sockfd: %d\n",port,sockListen);
=======
    int sockListen = CreateServer(port,5);
    printf("listened: %d\n",port);
>>>>>>> 7e7263ee43b22d2fcf1a2d58acacb8ede31d96aa
    struct epoll_event event;  
    event.events = EPOLLIN;  
    event.data.fd = sockListen;  
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockListen, &event) < 0){  
        printf("epoll add fail : fd = %d\n", sockListen);  
        return -1;  
    }  
    while(1){                 
        printf("start epoll....\n");
        int ret = epoll_wait(epollfd, eventList, MAX_EVENTS, timeout);  
        if(ret < 0){  
            printf("epoll error\n");  
            break;  
        } 
        for(int i = 0; i < ret; i++){  //错误退出  
            if ((eventList[i].events & EPOLLERR)){  
              printf ("epoll error\n");  
              close (eventList[i].data.fd);   
            }  
            if (eventList[i].data.fd == sockListen){  
                AcceptConn(epollfd, sockListen);  
            }else if(eventList[i].events & EPOLLIN){  
                RecvData(epollfd, eventList[i].data.fd);  
            }else if(eventList[i].events & EPOLLOUT){
            	SendData(epollfd, &(eventList[i]));
            }else{

            }
        }  
    }  
    close(epollfd);  
    close(sockListen);  
    printf("test\n");  
    return 0;  
}  

void AcceptConn(int epollfd, int srvfd){  
    struct sockaddr_in sin;  
    socklen_t len = sizeof(struct sockaddr_in);  
    bzero(&sin, len);  
    int confd = accept(srvfd, (struct sockaddr*)&sin, &len);  
    printf("Accept Connection: %d\n", confd);  
    struct epoll_event event;  
    event.data.fd = confd;  
    event.events =  EPOLLIN;  
    epoll_ctl(epollfd, EPOLL_CTL_ADD, confd, &event);  
    g_hash_table_insert(EventTable, itoa(confd), itoa((size_t)&event));
}  
  
//读取数据  
void RecvData(int epollfd, int fd){  
	printf("Recving data:%d\n",fd);
	struct HEAD_MAIN head_main;
	recv(fd,&head_main, sizeof(head_main),MSG_WAITALL);
	if(head_main.mode==0){
		printf("\t receive mode = 0\n");
		struct HEAD_USER data;
		recv(fd,&data, sizeof(data),MSG_WAITALL);
		userDataProcess(epollfd,fd, &data);
	}else if(head_main.mode==1){
		printf("\t receive mode = 1\n");
		struct HEAD_DATA data;
		recv(fd,&data, sizeof(data),MSG_WAITALL);
		dataDataProcess(epollfd,fd,&data);
	}else{
		printf("\tdata error\n");
	}
	printf("End recving\n\n");
}  

void userDataProcess(int epollfd,int sockfd, struct HEAD_USER* data){
	if(data->logmode==0){
		printf("\t\tlogmode = 0  [signup]\n");
		printf("\t\tusername = `%s` password = `%s` nickname = `%s` \n",data->username,data->password,data->nickname);
	}else if(data->logmode==1){
		printf("\t\tlogmode = 1  [login]\n");
		printf("\t\tusername = `%s` password = `%s` \n",data->username,data->password);
		#ifdef DEBUG
		printf("[recv]:");
		print16((char*)data,sizeof(struct HEAD_USER));
		#endif
		char* nickname = (char*)malloc(16);
		int r = sql_login(db,data->username,data->password,&nickname);
		if(r){
			printf("\tlogin succeed\n");
			struct HEAD_USER_ALL returndata;
			returndata.main.mode = 0;
			returndata.user.logmode = 10;
			strcpy(returndata.user.username,data->username);
			char* token = createToken(32);
			memcpy(returndata.user.password,token,16);
			memcpy(returndata.user.nickname,token+16,16);
			SendToFd(epollfd, sockfd, (char*)&returndata,sizeof(returndata));
		}else{
			printf("\tlogin error\n");
			struct HEAD_USER_ALL returndata;
			returndata.main.mode = 0;
			returndata.user = (struct HEAD_USER)*data;
			returndata.user.logmode = 11;
			SendToFd(epollfd, sockfd, (char*)&returndata,sizeof(returndata));
		}
		free(nickname);
	}
}
void dataDataProcess(int epollfd,int sockfd, struct HEAD_DATA* data){

}
void SendData(int epollfd, struct epoll_event* event){
	struct SERVERSENDSTRUCT* data = event->data.ptr;
	int sockfd = data->sockfd;
	send(sockfd,data->data,data->datalen,0);
	(*event).events = EPOLLIN;
	(*event).data.fd = sockfd;
	epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, event);
}


void SendToFd(int epollfd, int sockfd,char* data,int size){
	char* str_sockfd = itoa(sockfd);
	char* str_event = g_hash_table_lookup(EventTable,str_sockfd);
	if(str_event==NULL){
		printf("hash error");
		close(sockfd);
		return;
	}
	struct epoll_event* event = (struct epoll_event*)atol(str_event);
	free(str_sockfd);
	if(event->events & EPOLLOUT){
		printf("[error]: send to %d busy\n",sockfd);
	}
	struct SERVERSENDSTRUCT structdata;
	structdata.sockfd = sockfd;
	structdata.data = data;
	structdata.datalen = size;
	event->data.ptr = &structdata;
	event->events = EPOLLOUT;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, event); 
}
