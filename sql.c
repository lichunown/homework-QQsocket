#ifndef SQL_C
#define SQL_C 0

#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <stdio.h>
#define DATABASENAME "data.sqlite3"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

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
	int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
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
#endif