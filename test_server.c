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

void checkloop(int sockfd);
void server_signup(int sockfd,struct HEAD_USER* data);
void server_login(int sockfd,struct HEAD_USER* data);

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

 
    if(argv == 2){
        port = atoi(args[1]);
    }
    int sockListen = CreateServer(port,10);
    printf("listened: %d      sockfd: %d\n",port,sockListen);
    struct sockaddr_in clientaddr;
    while(int client = accept(sockListen,(struct sockaddr*)&clientaddr,sizeof(clientaddr))){                 
        printf("receive client conn %d\n\n",client);
        checkloop(client);
    }  
    close(sockListen);  
    printf("test\n");  
    return 0;  
}  

void checkloop(int sockfd){
    char* data = (char*)malloc(sizeof(struct HEAD_DATA_ALL));
    struct HEAD_DATA_ALL* data_data = data;
    struct HEAD_USER_ALL* data_user = data;
    read(sockfd,&data,sizeof(data));
    if(data.main.mode == 0){//登录注册模式
        if(data_user->user.logmode == 0){//sign up
            server_signup(sockfd,data_user.user);
        }else if(data_user->user.logmode == 1){//log in
            server_login(sockfd,data_user.user);
        }else{// error
            printf("mode==0  logmode==%d   error. \n",data_user->user.logmode);
        }
    } else{//消息传输模式

    }
    free(data);
}
void server_signup(int sockfd,struct HEAD_USER* data){
    printf("signup:\n");
    printf("username:`%s` password:`%s` nickname:`%s`\n",data->username,data->password,data->nickname);
    /**********/
}
void server_login(int sockfd,struct HEAD_USER* data){
    printf("login:\n");
    printf("username:`%s` password:`%s`\n",data->username,data->password);
    /**********/
}



