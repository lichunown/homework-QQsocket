# This is our Web Homework

make a QQ using TCP.

## head for per message

### client to server

| HEAD_MAIN | HEAD_USER                                |
| --------- | ---------------------------------------- |
| char mode | char logmode; char username[16];char password[16];char nickname[16]; |
| 1         | 49                                       |

| discript                    | HEAD_MAIN mode | HEAD_USER logmode | char username | char password | char nickname |
| --------------------------- | -------------- | ----------------- | ------------- | ------------- | ------------- |
| login                       | 0              | 1                 | `username`    | `password`    | []            |
| signup                      | 0              | 0                 | `username`    | `password`    | `nickname`    |


| HEAD_MAIN | HEAD_DATA                                |
| --------- | ---------------------------------------- |
| char mode | char token[44];char datamode;int datalen; |
| 1         | 49                                       |

| discript | HEAD_MAIN mode | token   | HEAD_DATA datamode | int datalen |
| -------- | -------------- | ------- | ------------------ | ----------- |
| logout   | 0              | `token` | 0                  | `num`       |
| senddata | 0              | `token` | 1                  | `num`       |
| showlist | 0              | `token` | 2                  | `num`       |

### server to client

| discript | mode       | succ | datalen | 
|----------| ---------- | ---- | ---- |
| login  | 11     |    0 or 1     | 0    |
| signup        | 12     |    0 or 1     | 0    |
| logout          | 21    |    0 or 1     | 0   |
| showlist          | 22    |    0 or 1     | `len`    |
| senddata          | 20    |    0 or 1     | `len`    |

**succ==0 successful; succ!=0 unsuccessful**


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
