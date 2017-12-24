#ifndef MY_H
#define MY_H 0

#define DEBUG true//是否debug（目前这一定义没用。。）

#define PERSTRLENGTH 200 //分割字符串的最大长度
#define TOKENSIZE 32//token大小

#define FILEPERLEN 1024//文件传输，分块大小
#define SAVEFILEPATH "data"//服务器文件保存目录
#define CLIENTDATAPATH "clientdata"//客户端文件保存目录

#define MAX_EVENTS 100 // 服务器每次循环event上限
#define DATABASENAME "data.sqlite3"//数据库服务器保存位置


#endif