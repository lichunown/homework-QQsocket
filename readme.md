# This is our Web Homework

make a QQ using TCP.

## head for per message

### client to server

|HEAD_MAIN|HEAD_USER|
|-----|-----|
|char mode|char logmode; char username[16];char password[16];char nickname[16];|
|1|49|
mode = 0; user
mode = 1; data 
logmode = 1; login
logmode = 0; signup

|HEAD_MAIN|HEAD_DATA|
|-----|-----|
|char mode|char token[32];char datamode;int datalen;|
|1|37|

datamode = 0; signout
datamode = 2; show list

# build

- to use sqlite3(for ubuntu)

```bash
sudo apt-get install sqlite3 
sudo apt-get install libsqlite3-dev
```

- use glib
(glib download)[http://ftp.acc.umu.se/pub/GNOME/sources/glib/2.54/]

```
./configure
make
make install
# or #
# ./configure --prefix=/usr && make && make install
```
(ubuntu完整安装glib心得)[http://blog.csdn.net/andylauren/article/details/51346388]

- build test

```bash
make test
```
# test

## sql_test

```bash

# create a new user
./sql_test createuser [username] [password] [nickname]
# login 
./sql_test login [username] [password] 
# changenickname
./sql_test changenickname [username] [newnickname] 
# show all sql data
./sql_test all
```
# document

- sql.c
```c
int Sqlite3_open(char* name,sqlite3** db);
sqlite3* createDatabase(sqlite3* db);
sqlite3* databaseInit();
int sql_createUser(sqlite3* db,char* username,char* password,char* nickname);
int sql_login(sqlite3* db,char* username,char* password,char** gettingnick);
int sql_changeNickname(sqlite* db,char* username,char* newnickname);
sql_all(sqlite3* db,char*** data,int* row,int* col);
/*
	-   sql_all(db,NULL,NULL,NULL);

	-   char** data;
		int row,col;
		sql_all(db,&data,&row,&col);
*/
```
# TODO

turn to use glib.
