# This is our Web Homework

make a QQ using TCP.

## head for per message

### client to server

|HEAD_MAIN|HEAD_USER|
|-----|-----|
|char mode|char logmode; char username[16];char password[16];char nickname[16];|
|1|49|

|HEAD_MAIN|HEAD_DATA|
|-----|-----|
|char mode|char token[32];char datamode;int datalen;|
|1|37|


# build

to use sqlite3(for ubuntu)

```bash
sudo apt-get install sqlite3 
sudo apt-get install libsqlite3-dev
```

build test
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

```