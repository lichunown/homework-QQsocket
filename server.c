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

void AcceptConn(int epollfd, int srvfd);  
void RecvData(int epollfd, int fd);  
void SendData(int epollfd, struct epoll_event* event);

void userDataProcess(int epollfd,int sockfd, struct HEAD_USER* data);
void dataDataProcess(int epollfd,int sockfd, struct HEAD_DATA* data);

void server_login(char* username,char* password);
void server_signup(char* username,char* password,char* nickname);
void SendToFd(int epollfd, int sockfd,void* data,int size);

void add_event(int epollfd,int fd,int state);
void delete_event(int epollfd,int fd,int state);
void modify_event(int epollfd,int fd,int state);

GHashTable* User2Sock;
GHashTable* Sock2Fifo;
GHashTable* Token2User;
GHashTable* User2Nick;
GHashTable* sock2event;
GHashTable* sock2data;

sqlite3* db = NULL;



int main(int argv,char* args[]){   
	db = databaseInit();
    
	User2Sock = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	Sock2Fifo = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	Token2User = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	User2Nick = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	sock2event = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	sock2data = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);

	int port = 8001;
	int epollfd = epoll_create(MAX_EVENTS);  
	struct epoll_event eventList[MAX_EVENTS];  

    int timeout = -1;  
    if(argv == 2){
        port = atoi(args[1]);
    }
    int sockListen = CreateServer(port,10);
    printf("listened: %d      sockfd: %d\n",port,sockListen);

    struct epoll_event event;  
    event.events = EPOLLIN;  
    event.data.fd = sockListen;  
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockListen, &event) < 0){  
        printf("epoll add fail : fd = %d\n", sockListen);  
        return -1;  
    }  
    for(int i=0;i<10;i++){                 
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
    int clientfd = accept(srvfd, (struct sockaddr*)&sin, &len);  
    printf("Accept Connection: %d\n", clientfd);  
    struct epoll_event event;  
    event.data.fd = clientfd;  
    event.events =  EPOLLIN;  
    epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event);  
    printf("create & insert event:%p\n",&event);
    g_hash_table_insert(sock2event, itoa(clientfd), ptoa(&event));
}  
  
//读取数据  
void RecvData(int epollfd, int fd){  
	printf("Recving data:%d\n",fd);
	struct HEAD_MAIN head_main;
	int n = Recv(fd,&head_main, sizeof(head_main),MSG_WAITALL);
	if(n<=0){
		printf("socked %d receive %d.  error\n",fd,n);
		delete_event(epollfd,fd,EPOLLIN);
	}
	if(head_main.mode==0){
		printf("\t receive mode = 0\n");
		struct HEAD_USER data;
		Recv(fd,&data, sizeof(data),MSG_WAITALL);
		userDataProcess(epollfd,fd, &data);
	}else if(head_main.mode==1){
		printf("\t receive mode = 1\n");
		struct HEAD_DATA data;
		Recv(fd,&data, sizeof(data),MSG_WAITALL);
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
		int r = sql_createUser(db,data->username,data->password,data->nickname);
		struct HEAD_RETURN returndata;
		bzero(&returndata,sizeof(returndata));
		if(r){
			printf("signup successful.\n");
			returndata.mode = 12;
			returndata.succ = 0;
			returndata.datalen = 0;
			SendToFd(epollfd, sockfd, &returndata,sizeof(returndata));
		}else{
			printf("signup error.\n");
			returndata.mode = 12;
			returndata.succ = 1;
			returndata.datalen = 0;
			SendToFd(epollfd, sockfd, &returndata,sizeof(returndata));
		}
	}else if(data->logmode==1){
		printf("\t\tlogmode = 1  [login]\n");
		printf("\t\tusername = `%s` password = `%s` \n",data->username,data->password);
		char* nickname = (char*)malloc(16);
		int r = sql_login(db,data->username,data->password,&nickname);
		if(r){
			struct login_return_all{
				struct HEAD_RETURN head;
				struct server_login_return data;
			}returndata;	
			bzero(&returndata,sizeof(returndata));		
			printf("\tlogin succeed\n");
			returndata.head.mode = 11;
			returndata.head.succ = 0;
			returndata.head.datalen = sizeof(struct server_login_return);
			strcpy(returndata.data.nickname,data->username);
			char* token = createToken(32);
			memcpy(returndata.data.token, token,32);
			printf("create return data\n");
			SendToFd(epollfd, sockfd, &returndata,sizeof(returndata));
		}else{
			printf("\tlogin error\n");
			struct HEAD_RETURN returndata;
			bzero(&returndata,sizeof(returndata));
			returndata.head.mode = 11;
			returndata.head.succ = 1;
			returndata.head.datalen = 0;
			SendToFd(epollfd, sockfd, &returndata,sizeof(returndata));
		}
		free(nickname);
	}
}
void dataDataProcess(int epollfd,int sockfd, struct HEAD_DATA* data){

}

void SendData(int epollfd, struct epoll_event* event){
	printf("SendData function start   event.fd:%d\n",event->data.fd);
	int sockfd = event->data.fd;
	char* str_sockfd = itoa(sockfd);
	char* str_datap = g_hash_table_lookup(sock2data,str_sockfd);
	struct SEND_DATA* senddata = (struct SEND_DATA*)atol(str_datap);
	//if(senddata==NULL)printf("[error]senddata==NULL.\n");
	while(senddata != NULL){
		printf("send............\n");
		printf("Send: len:%d\n",senddata->len);
		// printf("Send: data:`%s`\n",(char*)senddata->data);
		Send(sockfd,senddata->data,senddata->len,0);
		struct SEND_DATA* temp = senddata;
		senddata = senddata->next;
		free(temp);
	}
	free(str_sockfd);
	modify_event(epollfd,sockfd,EPOLLIN);
}


void SendToFd(int epollfd, int sockfd,void* data,int size){
	char* str_sockfd = itoa(sockfd);
	struct SEND_DATA* sendlist = g_hash_table_lookup(sock2data,str_sockfd);

	struct SEND_DATA* newdata = (struct SEND_DATA*)malloc(sizeof(struct SEND_DATA));
	newdata->len = size;
	newdata->data = data;
	newdata->next = NULL;

	if(sendlist==NULL){
		g_hash_table_insert(sock2data,itoa(sockfd),ptoa(newdata));
	}else{
		while(sendlist->next != NULL)sendlist = sendlist->next;
		sendlist->next = newdata;
	}
	free(str_sockfd);
	modify_event(epollfd,sockfd,EPOLLOUT);
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
