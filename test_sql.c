#include "sql.c"

int main(){
	sqlite3*db = databaseInit();
	sql_createUser(db,"qwer","qwer","qwer");
	sqlite3_close(db);
	return 0;
}