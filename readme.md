# This is our Web Homework

做一个类似于QQ的聊天程序，具有用户的登录注册等功能。

分为服务器和客户端两部分。客户端发起连接，服务器响应，建立的TCP连接始终保留。

客户端发起的每次请求，都会先发送一个固定大小的数据报头。如果有其他数据（比如给另一用户发送消息），则在报头发送完成后发送数据段。服务器接受消息并处理后，返回一个是否成功的提示（同时可能有附加数据）。

## 数据报头

### 客户端到服务器

| HEAD_MAIN | HEAD_USER                                |
| --------- | ---------------------------------------- |
| char mode | char logmode; char username[16];char password[16];char nickname[16]; |
| 1         | 49                                       |

|  discript  | HEAD_MAIN mode | HEAD_USER logmode | char username | char password | char nickname |
| :--------: | -------------- | ----------------- | ------------- | ------------- | ------------- |
| login（登录）  | 0              | 1                 | `username`    | `password`    | []            |
| signup（注册） | 0              | 0                 | `username`    | `password`    | `nickname`    |


| HEAD_MAIN | HEAD_DATA                                |
| --------- | ---------------------------------------- |
| char mode | char token[44];char datamode;int datalen; |
| 1         | 49                                       |

| discript            | HEAD_MAIN mode | token   | HEAD_DATA datamode | int datalen |
| ------------------- | -------------- | ------- | ------------------ | ----------- |
| logout（登出）          | 0              | `token` | 0                  | 0           |
| senddata（向某一用户发送消息） | 0              | `token` | 1                  | `len`       |
| showlist（显示在线用户）    | 0              | `token` | 2                  | 0           |

### 服务器到客户端

`struct HEAD_RETURN`

| discript                | mode | succ   | datalen |
| ----------------------- | ---- | ------ | ------- |
| login（登录）               | 11   | 0 or 1 | `len`   |
| signup（注册）              | 12   | 0 or 1 | 0       |
| logout（登出）              | 21   | 0 or 1 | 0       |
| showlist（显示在线用户列表）      | 22   | 0 or 1 | `len`   |
| senddata（自己的消息是否成功）     | 20   | 0 or 1 | `len`   |
| receive data（其他人发送来的消息） | 99   | 0      | `len`   |
| token error                | 50   | 1      | 0   |

**succ==0 successful; succ!=0 unsuccessful（0为成功）**
#### 服务器返回附加数据
- server_login_return（登录成功返回附加数据）
| discript  | 结构体属性    | 大小   |
| --------- | -------- | ---- |
| 登录用户的昵称   | nickname | 16   |
| 标识用户身份的令牌 | token    | 32   |

# build

- to use sqlite3(for ubuntu)

```bash
sudo apt-get install sqlite3 
sudo apt-get install libsqlite3-dev
```

- use glib
  (glib download)[http://ftp.acc.umu.se/pub/GNOME/sources/glib/2.54/]

```
wget http://ftp.gnome.org/pub/gnome/sources/glib/2.54/glib-2.54.2.tar.xz
tar -xvJf ./glib-2.54.2.tar.xz
cd ./glib-2.54.2.tar.xz

sudo apt-get install gcc
sudo apt-get install zlib1g-dev
sudo apt-get install pkg-config
sudo apt-get install libffi-dev
sudo apt-get install gettext
sudo apt-get install libmount-dev
sudo apt-get install libpcre3-dev

make
make install

# show the gcc option 
pkg-config glib-2.0 --libs


sudo cp -r /usr/local/include/glib-2.0/* /usr/include/  
sudo cp /usr/local/lib/glib-2.0/include/glibconfig.h /usr/include/
```
(ubuntu完整安装glib心得)[http://blog.csdn.net/andylauren/article/details/51346388]

- build test

```bash
make test
```
# test

## test_sql(测试数据库)

```bash
make test_sql.out # build
# create a new user
./test_sql.out createuser [username] [password] [nickname]
# login 
./test_sql.out login [username] [password] 
# changenickname
./test_sql.out changenickname [username] [newnickname] 
# show all sql data
./test_sql.out all
```
## test_s(测试服务器与客户端)
```bash
make test_s # build 
./test_server.out 8001 # 可以不输入端口号，默认8001
# another shell
./client.out 0.0.0.0 8001 # 两个参数分别为连接的IP 和端口号
```

# document
- mystring.c

```c
#define PERSTRLENGTH 200 //分割字符串的最大长度

char** split(char* str); // 分割字符串为2段
char** split_num(char* str,int num);//分割字符串为num段
void free_splitdata(char** data);// 释放分割字符串空间
void free_splitdata_num(char** data,int num);// 释放分割字符串空间

char* itoa(int num);
char* ltoa(long num);
char* ptoa(void* num);
```
**可以使用`make test_mysring`测试**
- encode.c
```c
char* createToken(int len); // 创建长度len-1的随机字符串（最后一位是'\0'）[需要手工free]
void print16(char* data,int size);// 每个字节以16进制输出
```

- sql.c
```c
int Sqlite3_open(char* name,sqlite3** db);// open database
sqlite3* createDatabase(sqlite3* db);// create new database
sqlite3* databaseInit();//auto open or create database
int sql_createUser(sqlite3* db,char* username,char* password,char* nickname);
int sql_login(sqlite3* db,char* username,char* password,char** gettingnick);
int sql_changeNickname(sqlite3* db,char* username,char* newnickname);
int sql_all(sqlite3* db,char*** data,int* row,int* col);
// sql_all(db,NULL,NULL,NULL); auto printf database data;
/*
	-   sql_all(db,NULL,NULL,NULL);

	-   char** data;
		int row,col;
		sql_all(db,&data,&row,&col);
*/
```
# TODO

- 密码加密算法

- pthread 使用多线程的理由: 线程间方便的**通信机制**

## bug

- when use `#login` in client. response could receive nickname & token.but only token can read. 
