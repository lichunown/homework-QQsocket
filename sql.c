#ifndef SQL_C
#define SQL_C 0

#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "encode.c"

#define DATABASENAME "data.sqlite3"



int Sqlite3_open(char* name,sqlite3** db){
	int r = sqlite3_open(name, db);
	if(r){
		printf("Can't open database: %s\n", sqlite3_errmsg(*db));
	}
	return r;
}

sqlite3* createDatabase(sqlite3* db){
	char* sql = "CREATE TABLE user("  \
         "username  char(16)  PRIMARY KEY  NOT NULL," \
         "password  char(50)  NOT NULL," \
         "nickname  char(16));";
	char* zErrMsg;
	int rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
	 	printf("Table created successfully\n");
	}
	return db;
}

sqlite3* databaseInit(){
	sqlite3* db;
	if((access(DATABASENAME, 0)) != -1 ){ //exit
		Sqlite3_open(DATABASENAME, &db);
	}else{ //don't exit
		Sqlite3_open(DATABASENAME, &db);
		createDatabase(db);
	}
	return db;
}

int sql_createUser(sqlite3* db,char* username,char* password,char* nickname){
	char sql[1024];
	char* zErrMsg;
	sprintf(sql,"INSERT INTO user(username,password,nickname)"\
				" VALUES ('%s','%s','%s');",username,password,nickname);
	//puts(sql);
	int rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
	}else{
		printf("Records created successfully\n");
		return 1;
	}
}

int sql_login(sqlite3* db,char* username,char* password,char** gettingnick){
	int result = 0;
	char sql[1024];
	password = encodePassword(password);// TODO :may need to delete this point
	sprintf(sql,"select nickname from user"\
				" where username='%s' and password='%s';",username,password);
	char** azResult;
	int nrow,ncolumn;
	char* zErrMsg;
	int rc = sqlite3_get_table(db,sql,&azResult,
								&nrow,&ncolumn,&zErrMsg); 
	printf("row: %d    col: %d\n",nrow,ncolumn);

	if( rc != SQLITE_OK ){
		printf("sql_login: SQL error: %s\n", zErrMsg);
	}else{
		if(nrow != 0){
			char* nickname = (char*)malloc(16);
			strcpy(nickname,azResult[1]);	
			*gettingnick = nickname;
			result = 1;			
		}
	}	
	sqlite3_free(zErrMsg);
	sqlite3_free_table(azResult);	
	return result;
}

#endif