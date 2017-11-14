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
#include <unistd.h>  
#include <fcntl.h>  
#include <limits.h>  
#include <sys/stat.h>  
#include <sys/shm.h> 
#include <pthread.h>
void checkloop(int sockfd);
void server_signup(int sockfd,struct HEAD_USER* data);
void server_login(int sockfd,struct HEAD_USER* data);

int sendto_by_Token(char* token,void* data,size_t len);
int sendto_by_User(char* username,void* data,size_t len);
int sendto_by_sockfd(int sockfd,void* data,size_t len);



sqlite3* db = NULL;

struct SHMDATA{
    GHashTable* User2Sock;
    GHashTable* Sock2Fifo;
    GHashTable* Token2User;
    GHashTable* User2Nick;
}SHMDATA;

struct SHMDATA* shmdata;
int shmid;

void Init(){
    assert(sizeof(struct HEAD_USER_ALL)==sizeof(struct HEAD_DATA_ALL));
    db = databaseInit();
    shmid = shmget((key_t)1234, sizeof(struct SHMDATA), 0666|IPC_CREAT);
    if(shmid == -1){
        printf("shmget error.\n");
        exit(1);
    }
    shmdata = shmat(shmid, 0, 0);
    shmdata->User2Sock = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
    shmdata->Sock2Fifo = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
    shmdata->Token2User = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free); 
    shmdata->User2Nick = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free); 
    shmdt(shmdata);
}
int main(int argv,char* args[]){   
    Init();
    int port = 8001;
    if(argv == 2){
        port = atoi(args[1]);
    }
    int sockListen = CreateServer(port,10);
    printf("listened: %d      sockfd: %d\n",port,sockListen);
    struct sockaddr_in clientaddr;
    int client;
    socklen_t addrsize;
    while(1){    
        client = accept(sockListen,(struct sockaddr*)&clientaddr, &addrsize);
        if(client != -1){
            printf("receive client conn %d\n\n",client);
            int pid = fork();
            if(pid==0){
                close(sockListen);
                checkloop(client);
            }else{
                close(client);
            }
        } else{
            printf("receive client conn error.\n");
        }            
    }  
    close(sockListen);  
    printf("test\n");  
    return 0;  
}  
void response(int sockfd,char mode,char succ,void* datap,int size){
    struct HEAD_RETURN returndata;
    bzero(&returndata,sizeof(returndata));
    returndata.mode = mode;
    returndata.succ = succ;
    returndata.datalen = size;
    //Send(sockfd,&returndata,sizeof(returndata),0);  
    int n = sendto_by_sockfd(sockfd,&returndata,sizeof(returndata));
    printf("sendto_by_sockfd send:   %d/%ld\n",n,sizeof(returndata));
    if(size != 0){
        printf("size!=0:   send:%s\n",(char*)datap);
        //Send(sockfd,datap,size,0);
        n = sendto_by_sockfd(sockfd,datap,size); 
        printf("sendto_by_sockfd send:   %d/%d\n",n,size); 
    }
}
void checkloop(int sockfd){
    int pid = fork();
    if(pid != 0){ //read & change
        struct HEAD_DATA_ALL data_data;
        while(1){
            bzero(&data_data,sizeof(data_data));
            struct HEAD_USER_ALL* data_user = (struct HEAD_USER_ALL*)&data_data;
            int p_res = Recv(sockfd,&data_data,sizeof(data_data),MSG_WAITALL);
            if(p_res == 0){
                kill(0,9);
                close(sockfd);
                exit(0);
            }
            print16((char*)&data_data,sizeof(data_data));
            if(data_data.main.mode == 0){//登录注册模式
                if(data_user->user.logmode == 0){//sign up
                    server_signup(sockfd,&(data_user->user));
                }else if(data_user->user.logmode == 1){//log in
                    server_login(sockfd,&(data_user->user));
                }else{// error
                    printf("mode==0  logmode==%d   error. \n",data_user->user.logmode);
                }
            } else{//消息传输模式

            }
        }
    }else{ // write to client
        printf("start write pid\n");
        char* fifo_name = (char*)malloc(50);
        sprintf(fifo_name,"my_server_fifo_%d",sockfd);
        if(access(fifo_name, F_OK) == -1){  //管道文件不存在  
            int res = mkfifo(fifo_name, 0777);  
            if(res != 0){  
                fprintf(stderr, "Could not create fifo %s\n", fifo_name);  
                exit(EXIT_FAILURE);  
            }  
        }
        printf("create fifo_name:%s\n",fifo_name);
        shmdata = shmat(shmid, 0, 0);
        g_hash_table_insert(shmdata->Sock2Fifo, itoa(sockfd), fifo_name);
        shmdt(shmdata);
        printf("insert fifo_name  sockfd:%d  fifo_name:%s\n",sockfd,fifo_name);
        int read_fd = open(fifo_name, O_RDONLY);      
        printf("create sock2fifo\n");
        while(1){
            struct HEAD_SEND_BY_FIFO data;
            read(read_fd, &data, sizeof(struct HEAD_SEND_BY_FIFO));
            void* senddata = malloc(data.len);
            read(read_fd, &senddata,data.len);
            Send(sockfd, &senddata, data.len, 0);
            free(senddata);
        }
    }
}

int sendto_by_Token(char* token,void* data,size_t len){
    shmdata = shmat(shmid, 0, 0);
    char* username = g_hash_table_lookup(shmdata->Token2User,token);
    shmdt(shmdata);
    if(username==NULL)return -1;
    return sendto_by_User(username,data,len);
}
int sendto_by_User(char* username,void* data,size_t len){
    shmdata = shmat(shmid, 0, 0);
    char* sockfd_s = g_hash_table_lookup(shmdata->User2Sock,username);
    shmdt(shmdata);
    if(sockfd_s == NULL)return -1;
    int sockfd = atoi(sockfd_s);
    return sendto_by_sockfd(sockfd,data,len);
}

int sendto_by_sockfd(int sockfd,void* data,size_t len){
    
    printf("sendto_by_sockfd:  sockfd:%d \n",sockfd);
    char* sockfd_s = itoa(sockfd);
    shmdata = shmat(shmid, 0, 0);
    char* fifo_name = g_hash_table_lookup(shmdata->Sock2Fifo,sockfd_s);
    shmdt(shmdata);
    if(fifo_name==NULL){
        printf("[error] fifo_name not exist in hashtable.\t sockfd: %d\n",sockfd);
        return -1;
    }
    free(sockfd_s);
    int write_fd = open(fifo_name, O_WRONLY);
    struct HEAD_SEND_BY_FIFO datahead;
    datahead.len = len;
    write(write_fd,&datahead,sizeof(struct HEAD_SEND_BY_FIFO));
    return write(write_fd,data,len);
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
        char* username1 = (char*)malloc(16);
        strcpy(username1,data->username);      
        char* username2 = (char*)malloc(16);
        strcpy(username2,data->username);
        char* token = createToken(32);
        char* username3 = (char*)malloc(16);
        strcpy(username3,data->username);  
        shmdata = shmat(shmid, 0, 0);
        g_hash_table_insert(shmdata->User2Sock, username1, itoa(sockfd));
        g_hash_table_insert(shmdata->User2Nick, username2, nickname);      
        g_hash_table_insert(shmdata->Token2User, token, username3);
        shmdt(shmdata);
        struct server_login_return rdata;
        bzero(&rdata,sizeof(rdata));
        strcpy(rdata.nickname,data->username);
        strcpy(rdata.token,token);
        response(sockfd,11,0,&rdata,sizeof(rdata));
        //free(nickname);
        //free(token);
    }else{
        response(sockfd,11,1,NULL,0);
    }
}



