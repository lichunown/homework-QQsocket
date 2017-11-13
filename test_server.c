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
#include <assert.h>
void checkloop(int sockfd);
void server_signup(int sockfd,struct HEAD_USER* data);
void server_login(int sockfd,struct HEAD_USER* data);

GHashTable* UserTable = NULL;    // username  ->  event
GHashTable* UserDataTable = NULL;// username  ->  nickname
GHashTable* TokenTable = NULL;   // token     ->  username
GHashTable* EventTable = NULL;   // sockfd    ->  event*

GHashTable* SockTable = NULL;    // username  ->  socket

sqlite3* db = NULL;

int main(int argv,char* args[]){   
    printf("HEAD_USER_ALL:%ld  HEAD_DATA_ALL:%ld",sizeof(struct HEAD_USER_ALL),sizeof(HEAD_DATA_ALL));
    assert(sizeof(struct HEAD_USER_ALL)==sizeof(struct HEAD_DATA_ALL));
    db = databaseInit();
    int port = 8001;
    UserTable = g_hash_table_new(g_str_hash, g_str_equal);//username -> event
    UserDataTable = g_hash_table_new(g_str_hash, g_str_equal);// username -> nickname
    TokenTable = g_hash_table_new(g_str_hash, g_str_equal); //token -> username
    EventTable = g_hash_table_new(g_str_hash, g_str_equal); //sockfd -> event*
    SockTable = g_hash_table_new(g_str_hash, g_str_equal); // username  ->  socket
 
    if(argv == 2){
        port = atoi(args[1]);
    }
    int sockListen = CreateServer(port,10);
    printf("listened: %d      sockfd: %d\n",port,sockListen);
    struct sockaddr_in clientaddr;
    int client;
    socklen_t addrsize;
    while((client = accept(sockListen,(struct sockaddr*)&clientaddr, &addrsize)) != -1){                 
        printf("receive client conn %d\n\n",client);
        checkloop(client);
    }  
    close(sockListen);  
    printf("test\n");  
    return 0;  
}  
void response(int sockfd,char mode,char succ,void* datap,int size){
    struct HEAD_RETURN returndata;
    returndata.mode = mode;
    returndata.succ = succ;
    returndata.datalen = size;
    Send(sockfd,&returndata,sizeof(returndata),0);  
    if(size != 0){
        Send(sockfd,datap,size,0);        
    }
}
void checkloop(int sockfd){
    struct HEAD_DATA_ALL data_data;
    while(1){
        bzero(&data_data,sizeof(data_data));
        struct HEAD_USER_ALL* data_user = (struct HEAD_USER_ALL*)&data_data;
        Recv(sockfd,&data_data,sizeof(data_data),MSG_WAITALL);
        print16((char*)&data_data,sizeof(data_data));
        if(data_data.main.mode == 0){//登录注册模式
            if(data_user->user.logmode == 0){//sign up
                server_signup(sockfd,&(data_user->user));
                response(sockfd,(char)0,(char)0,NULL,0);
            }else if(data_user->user.logmode == 1){//log in
                server_login(sockfd,&(data_user->user));
                response(sockfd,(char)0,(char)0,NULL,0);
            }else{// error
                printf("mode==0  logmode==%d   error. \n",data_user->user.logmode);
            }
        } else{//消息传输模式

        }
    }
}
void server_signup(int sockfd,struct HEAD_USER* data){
    printf("signup:\n");
    printf("username:`%s` password:`%s` nickname:`%s`\n",data->username,data->password,data->nickname);
    if(sql_createUser(db,data->username,data->password,data->nickname)){
        response(sockfd,12,0,NULL,0);
    }else{
        response(sockfd,12,1,NULL,0);
    }
}


void server_login(int sockfd,struct HEAD_USER* data){
    printf("login:\n");
    printf("username:`%s` password:`%s`\n",data->username,data->password);
    char* nickname;
    if(sql_login(db,data->username,data->password,&nickname)){
        g_hash_table_insert(SockTable, data->username, itoa(sockfd));
        g_hash_table_insert(UserDataTable, data->username, nickname);
        char* token = createToken(32);
        g_hash_table_insert(TokenTable, token, data->username);
        struct server_login_return rdata;
        bzero(&rdata,sizeof(rdata));
        strcpy(rdata.nickname,data->username);
        strcpy(rdata.token,token);
        response(sockfd,11,0,&rdata,sizeof(rdata));
        free(nickname);
        free(token);
    }else{
        response(sockfd,11,1,NULL,0);
    }
}



