#include "my_cli_head.h" 
#include "log.c"
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
	
    gtk_init(&argc,&argv);      //初始化
	int arg = argc;
	char ** agv = argv;
    log_user_interface(arg,agv);  //调用登录界面函数
    gtk_main(); 
	return 0;
}
