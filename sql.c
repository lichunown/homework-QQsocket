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

int Sqlite3_open(char* name,sqlite3** db);// open database
sqlite3* createDatabase(sqlite3* db);// create new database
sqlite3* databaseInit();//auto open or create database
int sql_createUser(sqlite3* db,char* username,char* password,char* nickname);
int sql_login(sqlite3* db,char* username,char* password,char** gettingnick);
int sql_changeNickname(sqlite3* db,char* username,char* newnickname);
int sql_all(sqlite3* db,char*** data,int* row,int* col);
// sql_all(db,NULL,NULL,NULL); auto printf database data;
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
	 	printf("Table user created successfully\n");
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
	char* sql = (char*)malloc(1024); 
	char* zErrMsg;
	sprintf(sql,"INSERT INTO user(username,password,nickname)"\
				" VALUES ('%s','%s','%s');",username,password,nickname);
	//puts(sql);
	int rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if( rc != SQLITE_OK ){
		printf("SQL error: %s\n", zErrMsg);
		free(sql);
		sqlite3_free(zErrMsg);
		return 0;
	}else{
		printf("Records created successfully\n");
		free(sql);
		sqlite3_free(zErrMsg);
		return 1;
	}
}

int sql_login(sqlite3* db,char* username,char* password,char** gettingnick){
	int result = 0;
	char* sql = (char*)malloc(1024); 
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
	free(sql);
	sqlite3_free(zErrMsg);
	sqlite3_free_table(azResult);	
	return result;
}

int sql_changeNickname(sqlite3* db,char* username,char* newnickname){
	char* sql = (char*)malloc(1024); 
	sprintf(sql,"update user set nickname='%s'" \
				"where username='%s';",newnickname,username);
	char* zErrMsg;
	int rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	free(sql);
	if( rc != SQLITE_OK ){
		printf("SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
	}else{
		printf("nickname change successfully\n");
		sqlite3_free(zErrMsg);
		return 1;
	}
}

int sql_all(sqlite3* db,char*** data,int* row,int* col){
	int result = 0;
	char** azResult;
	int nrow,ncolumn;
	char* zErrMsg;
	int rc = sqlite3_get_table(db,"select * from user;",&azResult,
								&nrow,&ncolumn,&zErrMsg); 
	printf("row: %d    col: %d\n",nrow,ncolumn);
	if( rc != SQLITE_OK ){
		printf("sql_login: SQL error: %s\n", zErrMsg);
	}else{
		if(data!=NULL && row!=NULL && col != NULL){// transport value
			*data = azResult;
			*row = nrow;
			*col = ncolumn;
		}else{//print
			for(int i=0;i<nrow+1;i++){
				for(int j=0;j<ncolumn;j++){
					printf("%s\t\t",azResult[j+i*ncolumn]);
				}
				printf("\n");
			}
		}
		sqlite3_free_table(azResult);
		result = 1;			
	}	
	sqlite3_free(zErrMsg);	
	return result;
}
#endif