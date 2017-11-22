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

#### 客户端到服务器附加数据

- senddata（向某一用户发送消息）

`struct client_to_server_send_to_user_head`

| discript       | 结构体属性    | 大小   |
| -------------- | -------- | ---- |
| direction user | username | 16   |
| data len       | len      | int  |
| data           | ???      | ???  |

### 服务器到客户端

`struct HEAD_RETURN`

```c
struct HEAD_RETURN{
	char mode;
	char succ;//是否成功
	unsigned int datalen;
}HEAD_RETURN;
```

| discript                | mode | succ   | datalen |
| ----------------------- | ---- | ------ | ------- |
| login（登录）               | 11   | 0 or 1 | `len`   |
| signup（注册）              | 12   | 0 or 1 | 0       |
| logout（登出）              | 21   | 0 or 1 | 0       |
| showlist（显示在线用户列表）      | 22   | 0 or 1 | `len`   |
| senddata（自己的消息是否成功）     | 20   | 0 or 1 | `len`   |
| receive data（其他人发送来的消息） | 99   | 0      | `len`   |
| token error             | 50   | 1      | 0       |

**succ==0 successful; succ!=0 unsuccessful（0为成功）**

#### 服务器返回附加数据

- server_login_return（登录成功返回附加数据）

```c
struct server_login_return{
    char nickname[16];
    char token[32];
}server_login_return;
```

| discript  | 结构体属性    | 大小   |
| --------- | -------- | ---- |
| 登录用户的昵称   | nickname | 16   |
| 标识用户身份的令牌 | token    | 32   |

- list_per_user（显示已登录用户  返回附加数据）

```c
struct list_per_user{
	char username[16];
	char nickname[16];
}list_per_user;
```

| discript    | 结构体属性    | 大小   |
| ----------- | -------- | ---- |
| 第一个登录用户的用户名 | username | 16   |
| 第一个用户的昵称    | nickname | 16   |
| 第二个登录用户的用户名 | username | 16   |
| 第二个用户的昵称    | nickname | 16   |
| ...         | ...      | 16   |

**HEAD_RETURN的datalen为     登录用户数量\*sizeof(list_per_user)**







# 函数说明
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