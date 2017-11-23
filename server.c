/**********************************************
author:lcy
服务器。采用epoll多路复用。单进程模式。

***********************************************/
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
#include "g_hash_extend.c"
//最多处理的connect  
#define MAX_EVENTS 100  

void AcceptConn(int epollfd, int srvfd);  
void RecvData(int epollfd, int fd);  
void SendData(int epollfd, struct epoll_event* event);

void userDataProcess(int epollfd,int sockfd, struct HEAD_USER* data);
void dataDataProcess(int epollfd,int sockfd, struct HEAD_DATA* data);

// void server_login(char* username,char* password);
// void server_signup(char* username,char* password,char* nickname);

void SendToFd(int epollfd, int sockfd,void* data,int size);

void add_event(int epollfd,int fd,int state);
void delete_event(int epollfd,int fd,int state);
void modify_event(int epollfd,int fd,int state);

void iteratorUser2Nick(char* username, char* nickname, int* sockfd);




GHashTable* User2Sock;// 用户名到sockfd
GHashTable* Token2User;// token 到用户名
GHashTable* User2Nick;// 用户名到昵称
GHashTable* sock2event;
GHashTable* sock2data;// 向sockfd发送的临时数据

sqlite3* db = NULL;

int epollfd;

int main(int argv,char* args[]){   
	db = databaseInit();
    
	User2Sock = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	Token2User = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	User2Nick = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	sock2event = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	sock2data = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);

	int port = 8001;
	epollfd = epoll_create(MAX_EVENTS);  
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
        exit(1);
    }  
    for(;;){                 
        printf("\n\nstart epoll....\n");
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
    return 0;  
}  

void AcceptConn(int epollfd, int srvfd){  // 新的客户端建立连接
    struct sockaddr_in sin;  
    socklen_t len = sizeof(struct sockaddr_in);  
    bzero(&sin, len);  
    int clientfd = accept(srvfd, (struct sockaddr*)&sin, &len);  
    printf("Accept Connection: %d\n", clientfd);  
    struct epoll_event event;  
    event.data.fd = clientfd;  
    event.events =  EPOLLIN; // 设置为接收消息模式 
    epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event);  
    // printf("create & insert event:%p\n",&event);
    g_hash_table_insert(sock2event, itoa(clientfd), ptoa(&event));
}  
  
//读取数据  
void RecvData(int epollfd, int sockfd){  
	printf("Recving data from:%d\n",sockfd);
	struct HEAD_MAIN head_main;
	int n = Recv(sockfd,&head_main, sizeof(head_main),0);
	if(n<=0){
		printf("sockfd %d receive %d.  error\n",sockfd,n);
		char* sockfd_str = itoa(sockfd);
		g_hash_table_remove(sock2event,sockfd_str);
		g_hash_table_remove(sock2data,sockfd_str);
		char* username = find_g_hash_by_value(User2Sock,sockfd_str);
		if(username != NULL){
			printf("delete login user `%s`\n",username);
			g_hash_table_remove(User2Nick,username);
			g_hash_table_remove(User2Sock,username);
			char* rmtoken = find_g_hash_by_value(Token2User,username);
			if(rmtoken!=NULL){
				printf("delete token `%s`\n",rmtoken);
				g_hash_table_remove(Token2User,rmtoken);
			}
		}
		free(sockfd_str);
		delete_event(epollfd,sockfd,EPOLLIN);
	}
	if(head_main.mode==0){// user模式
		// printf("receive mode = 0  #USER mode#\n");
		struct HEAD_USER data;
		Recv(sockfd,&data, sizeof(data),0);
		userDataProcess(epollfd,sockfd, &data);
	}else if(head_main.mode==1){// data模式
		// printf("receive mode = 1  #DATA mode#\n");
		struct HEAD_DATA data;
		Recv(sockfd,&data, sizeof(data),0);
		dataDataProcess(epollfd,sockfd,&data);
	}else{// 数据接收错误
		//printf("\tdata err   or or This is a test string\n");
		//SendToFd(epollfd,fd,"This is an echo.\n",sizeof("This is an echo.\n"));
	}
	printf("End recving\n\n");
}  

void userDataProcess(int epollfd,int sockfd, struct HEAD_USER* data){
    /*处理用户登录模式的逻辑*/
	if(data->logmode==0){ //注册
		printf("[sockfd %d]: [signup] username = `%s` password = `%s` nickname = `%s` \n",sockfd,data->username,data->password,data->nickname);
		int r = sql_createUser(db,data->username,data->password,data->nickname);
		if(r){// 用户注册成功
			printf("[sockfd %d]: signup successful.\n",sockfd);
			struct HEAD_RETURN* returndata = data_head_return(12,0,0);
			SendToFd(epollfd, sockfd, returndata,sizeof(struct HEAD_RETURN));
		}else{// 用户注册失败
			printf("[sockfd %d]: signup error.\n",sockfd);
			struct HEAD_RETURN* returndata = data_head_return(12,1,0);
			SendToFd(epollfd, sockfd, returndata,sizeof(struct HEAD_RETURN));
		}
	}else if(data->logmode==1){// 登录
		printf("[sockfd %d]: [login] username = `%s` password = `%s` \n",sockfd,data->username,data->password);
		char* nickname = (char*)malloc(16);
		int r = sql_login(db,data->username,data->password,&nickname);
		if(r){// 用户登录成功
			struct server_login_return* returndata = malloc(sizeof(struct server_login_return));
			bzero(returndata,sizeof(struct server_login_return));		
			printf("[sockfd %d]: login succeed\n",sockfd);
			struct HEAD_RETURN* returnhead = data_head_return(11,0,sizeof(struct server_login_return));
			// printf("return head:  %p\n",returnhead);
			// print16((char*)returnhead,sizeof(struct HEAD_RETURN));
			strcpy(returndata->nickname,data->username);
			char* token = createToken(32);
			memcpy(returndata->token, token,32);
			// printf("create return data\n");

			char* username = (char*)malloc(16);
			strcpy(username,data->username);
			g_hash_table_insert(User2Sock,username,itoa(sockfd));
			username = (char*)malloc(16);
			strcpy(username,data->username);			
			g_hash_table_insert(Token2User,token,username);
			username = (char*)malloc(16);
			strcpy(username,data->username);
			char* nickname = (char*)malloc(16);
			strcpy(nickname,data->nickname);
			g_hash_table_insert(User2Nick,username,nickname);

			SendToFd(epollfd, sockfd, returnhead,sizeof(struct HEAD_RETURN));
			SendToFd(epollfd, sockfd, returndata,sizeof(struct server_login_return));
		}else{// 登录失败
			printf("[sockfd %d]: login error\n",sockfd);
			struct HEAD_RETURN* returndata = data_head_return(11,1,0);
			SendToFd(epollfd, sockfd, returndata,sizeof(returndata));
		}
		free(nickname);
	}
}

void dataDataProcess(int epollfd,int sockfd, struct HEAD_DATA* data){
	char* username = g_hash_table_lookup(Token2User,data->token);
	if(username==NULL){//用户不存在
		printf("[sockfd %d]: [user mode]: user doesnt exist\n",sockfd);
		struct HEAD_RETURN* returndata = data_head_return(50,1,0);
		SendToFd(epollfd, sockfd, returndata,sizeof(struct HEAD_RETURN));
		return;
	}
	if(data->datamode==0){//登出
		printf("[sockfd %d]: [mode logout]\n",sockfd);
		g_hash_table_remove(User2Sock, username);
		g_hash_table_remove(User2Nick, username);
		g_hash_table_remove(Token2User, data->token);
		struct HEAD_RETURN* returndata = data_head_return(21,0,0);
		SendToFd(epollfd, sockfd, returndata,sizeof(struct HEAD_RETURN));	
		printf("[sockfd %d]: logout succeed.\n",sockfd);	
	}else if(data->datamode == 1){//用户发送数据
		printf("[sockfd %d]: mode senttosend\n",sockfd);
		struct client_to_server_send_to_user_head senddata_head;
		Recv(sockfd,&senddata_head,sizeof(senddata_head),0);
		void* senddata = malloc(senddata_head.len);
		Recv(sockfd,senddata,senddata_head.len,0);

		char* s_user2sockfd = g_hash_table_lookup(User2Sock, senddata_head.username);
		if(s_user2sockfd==NULL){// 要发送的人不存在
			printf("[sockfd %d]: sendto username:`%s` doesnt exist.\n",sockfd,senddata_head.username);
			struct HEAD_RETURN* sendtouser1 = data_head_return(20,1,0);
			SendToFd(epollfd, sockfd, sendtouser1, sizeof(struct HEAD_RETURN));
			return;			
		}
		int user2sockfd = atoi(s_user2sockfd);
		struct HEAD_RETURN* user2head = data_head_return(99,0,sizeof(struct server_to_client_send_to_user_head));
		struct server_to_client_send_to_user_head* user2data_head = (struct server_to_client_send_to_user_head*)malloc(sizeof(struct server_to_client_send_to_user_head));
		strcpy(user2data_head->username,username);
		user2data_head->len = senddata_head.len;
		SendToFd(epollfd, user2sockfd, user2head,sizeof(struct HEAD_RETURN));
		SendToFd(epollfd, user2sockfd, user2data_head, sizeof(struct server_to_client_send_to_user_head));
		SendToFd(epollfd, user2sockfd, senddata, senddata_head.len);
		struct HEAD_RETURN* sendtouser1 = data_head_return(20,0,0);
		SendToFd(epollfd, sockfd, sendtouser1, sizeof(struct HEAD_RETURN));

	}else if(data->datamode==2){// show list
		printf("[mode showlist]\n");
		int loginusernum = g_hash_table_size(User2Nick);
		struct HEAD_RETURN* returndata = data_head_return(22,0,loginusernum*sizeof(list_per_user));
		SendToFd(epollfd, sockfd, returndata, sizeof(struct HEAD_RETURN));
		g_hash_table_foreach(User2Nick, (GHFunc)iteratorUser2Nick, &sockfd);
	}else{

	}
}

void iteratorUser2Nick(char* username, char* nickname, int* sockfd){
	struct list_per_user* perdata = (struct list_per_user*)malloc(sizeof(struct list_per_user));
	bzero(perdata,sizeof(struct list_per_user));
	strcpy(perdata->username, username);
	strcpy(perdata->nickname, nickname);
	printf("[have login]: %s:%s \n", username, nickname);
	SendToFd(epollfd, *sockfd, perdata, sizeof(struct list_per_user));
}


void SendData(int epollfd, struct epoll_event* event){// 服务器发送数据
	int sockfd = event->data.fd;
	char* str_sockfd = itoa(sockfd);
	char* str_datap = g_hash_table_lookup(sock2data,str_sockfd);//寻找发送到sockfd的数据
	struct SEND_DATA* senddata = (struct SEND_DATA*)atol(str_datap);
	while(senddata != NULL){// 循环读入数据链表，发送数据并free已发送数据
		// printf("send list....... %p  next:%p\n",senddata,senddata->next);
		// printf("Send: len:%d   data address:%p\n ",senddata->len,senddata->data);
		Send(sockfd,senddata->data,senddata->len,0);
		free(senddata->data);
		struct SEND_DATA* temp = senddata;
		senddata = senddata->next;
		free(temp);
	}
	g_hash_table_remove(sock2data,str_sockfd);// 自动free数据段
	free(str_sockfd);
	modify_event(epollfd,sockfd,EPOLLIN);
}


void SendToFd(int epollfd, int sockfd,void* data,int size){// 向sockfd发送数据（不是实时发送）
    /*因使用epoll，实际发送为下一次epoll循环时*/
	char* str_sockfd = itoa(sockfd);
	char* str_datap = g_hash_table_lookup(sock2data,str_sockfd);

	struct SEND_DATA* newdata = (struct SEND_DATA*)malloc(sizeof(struct SEND_DATA));
	newdata->len = size;
	newdata->data = data;
	newdata->next = NULL;

	if(str_datap==NULL){
        // printf("sock2data create new   %p data:%p \n",newdata,data);
        // print16(data,8);
		g_hash_table_insert(sock2data,itoa(sockfd),ptoa(newdata));
	}else{
		struct SEND_DATA* sendlist = (struct SEND_DATA*)atol(str_datap);
        // printf("sock2data add new  %p  data:%p\n",newdata,data);
		while(sendlist->next != NULL){
			// print16(sendlist->data,8);
			sendlist = sendlist->next;
		}
		sendlist->next = newdata;
		// printf("insert data %p\n",sendlist->next);
	}
	free(str_sockfd);
	modify_event(epollfd,sockfd,EPOLLOUT);// 将sockfd转换为发送数据模式
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
