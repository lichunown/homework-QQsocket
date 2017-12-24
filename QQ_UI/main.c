#include "my_cli_head.h" 
#include "log.c"
#include "../mysocket.c"
#include "get_sock.c"
/*******************************************************************
函    数:    main
功    能:    gtk 初始化
传入参数:    无
传出参数:    无
返    回:    0:程序正常退出
             1:程序错误退出
********************************************************************/


int main(int argc,char **argv)
{
    if(!g_thread_supported())
    g_thread_init(NULL);

    gtk_init(&argc,&argv);      //初始化
    sockfd = CreateClient("0.0.0.0",8001);
    gtk_init(&argc,&argv);      //初始化
	int arg = argc;
    char ** agv = argv;
    log_user_interface(arg,agv);  //调用登录界面函数
    g_thread_create((GThreadFunc)get_sock, NULL, FALSE, NULL); 
    //g_thread_create((GThreadFunc)send_sock, NULL, FALSE, NULL); 
    //fork();
    //gdk_threads_enter();
    gtk_main(); 
    //gdk_threads_leave();
	return 0;
}
