#include "mysocket.c"
#include "mystruct.c"
#include "mystring.c"
#include <sys/select.h>
#include <poll.h>  
#include <sys/epoll.h>  
#include <glib.h>

//最多处理的connect  
#define MAX_EVENTS 100  

//数据接受 buf  
#define REVLEN 10  
char recvBuf[REVLEN];  

  
void AcceptConn(int epollfd, int srvfd);  
void RecvData(int epollfd, int fd);  
void SendData(int epollfd, struct epoll_event* event);

void userDataProcess(int sockfd, struct HEAD_USER* data);
void dataDataProcess(int sockfd, struct HEAD_DATA* data);

GHashTable* UserTable = NULL;
GHashTable* TokenTable = NULL;
GHashTable* EventTable = NULL;

int main(){   

	UserTable = g_hash_table_new(g_str_hash, g_str_equal);//username -> event
	TokenTable = g_hash_table_new(g_str_hash, g_str_equal); //token -> username
	EventTable = g_hash_table_new(g_str_hash, g_str_equal); //sockfd -> event*
	int epollfd = epoll_create(MAX_EVENTS);  
	struct epoll_event eventList[MAX_EVENTS];  

    int timeout = -1;  

    int sockListen = CreateServer(8001,10);
    struct epoll_event event;  
    event.events = EPOLLIN;  
    event.data.fd = sockListen;  
    // epoll_ctrl  
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockListen, &event) < 0){  
        printf("epoll add fail : fd = %d\n", sockListen);  
        return -1;  
    }  
    while(1){                 
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
}  
  
//读取数据  
void RecvData(int epollfd, int fd){  
	printf("Recving data:%d\n",fd);
	struct HEAD_MAIN head_main;
	read(fd,&head_main, sizeof(head_main));
	if(head_main.mode==0){
		printf("\t receive mode = 0\n");
		struct HEAD_USER data;
		read(fd,&data, sizeof(data));
		userDataProcess(fd, &data);
	}else if(head_main.mode==1){
		printf("\t receive mode = 1\n");
		struct HEAD_DATA data;
		read(fd,&data, sizeof(data));
		dataDataProcess(fd,&data);
	}else{
		printf("\tdata error\n");
	}
	printf("End recving\n\n");
}  

void userDataProcess(int sockfd, struct HEAD_USER* data){
	if(data->logmode==0){
		printf("\t\tlogmode = 0  [signup]\n");
		printf("\t\tusername = %s password = %s nickname = %s \n",data->username,data->password,data->nickname);
	}else if(data->logmode==1){
		printf("\t\tlogmode = 1  [login]\n");
		printf("\t\tusername = %s password = %s \n",data->username,data->password);
	}
}
void dataDataProcess(int sockfd, struct HEAD_DATA* data){

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
	struct epoll_event* event = g_hash_table_lookup(EventTable,str_sockfd);
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
