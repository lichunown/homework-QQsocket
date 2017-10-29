#include "sql.c"
#include <string.h>

int main(int argv,char* args[]){
	sqlite3*db = databaseInit();
	//sql_createUser(db,"qwer","qwer","qwer");
	if(argv <= 1){ printf("please use `createuser` `login` cmds.\n");return 0; }
	if(strcmp(args[1],"createuser")==0){
		if(argv==5){
			sql_createUser(db,args[2],args[3],args[4]);
		}else{
			printf("please enter:\n\tcreateuser [username] [password] [nickname]\n");
		}
	}else if(strcmp(args[1],"login")==0){
		if(argv != 4){
			printf("please enter:\n\tcreateuser [username] [password]\n");
		}
		char* nickname;//need free
		int r = sql_login(db,args[2],args[3],&nickname);
		if(r){
			printf("nickname = %s\n",nickname);
			char* token = createToken(32);
			printf("token = %s\n",token);	
			puts("");
			free(token);
			free(nickname);
		}else{
			printf("user don't exists.\n");
		}		
	}else{
		printf("please use `createuser` `login` cmds.\n");
	}

	sqlite3_close(db);
	return 0;
}