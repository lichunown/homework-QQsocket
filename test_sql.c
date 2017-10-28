#include "sql.c"

int main(){
	sqlite3*db = databaseInit();
	//sql_createUser(db,"qwer","qwer","qwer");
	char* nickname;//need free
	sql_login(db,"qwer","qwer",&nickname);
	printf("nickname = %s      #",nickname);
	char* token = createToken(32);
	printf("token = %s",token);
	puts("");
	free(token);
	free(nickname);
	sqlite3_close(db);
	return 0;
}