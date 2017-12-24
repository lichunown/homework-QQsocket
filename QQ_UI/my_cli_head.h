#ifndef MY_CLI_HEAD_H_
#define MY_CLI_HEAD_H_


enum
{
    COLUMN = 0,
    NUM_COLS
};

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include "../mystruct.c"
#include "../mysocket.c"
#include "../client_recv.c"
int lock = 0;
int sockfd;          /*用来存放客户端的通信端口号*/
typedef struct message
{
    int user_id;                       /*服务器分配给新用户的帐号*/
    char user_name[20];                /*用户昵称*/
    char passwd[20];                   /*用户密码*/
    char to_name[20];                  /* 接收消息的用户*/
    char secret_pro[50];
    char answer[100];
    char remb_user_id[10];                 /*记下用户登录的id*/
} MSG;

MSG msg;  //定义一个结构变量

#define SERV_PORT 3333
char rember_user_id[10];           /*再次保存登录用户的id*/
gchar * name;
gchar * gstr[1000];
GtkWidget *label_del;
GtkTreeStore *treestore;
GtkTreeIter toplevel, child;           //迭代器
GtkTreeModel *model;
pthread_t id;
GtkTextBuffer *buffer_c;
char sendbuf[1024],getbuf[1024];
char sendbuf_g[1024],getbuf_g[1024];//发送接受消息窗口
/*保存登录信息*/
gchar * log_username;
gchar * log_password;
gchar * log_token;
/*保存注册信息*/
const gchar *username;
const gchar *password1;
const gchar *password2;
const gchar *answers;
const gchar *set_ques;
GtkWidget * log_win;
GtkWidget * window;
GtkWidget *window_d;     /*删除好友窗口*/
GtkWidget *dialog;
GtkWidget *view_y;
gint count_msg_win ;           /*记录下打开会话窗口的个数*/
void cancel_delete_friends();
void view_onRowActivated (GtkTreeView *treeview,GtkTreePath *path);
void log_user_interface();
char * int2str(int num);
void cli_main();
void main_interface();
void send_message_win(gchar * name);
void send_group_message_win();
void get_message(void);
void display_friends();
void deleted_friends();
void add_success();
void cant_add();
void no_rig_cant_add();
void delete_success();
void change_label(char * buf_t);
void get_sock();
void send_sock();
void dailog_log_win(char * buf_d);
char * time_now();
#endif



