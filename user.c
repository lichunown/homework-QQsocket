#ifndef USER_C
#define USER_C 0

#include <vector>
using namespace std;

struct LOGINUSER{
	char username[16];
	char token[32];
}LOGINUSER;

struct LOGINUSERS{
	struct LOGINUSER user;
	struct LOGINUSER* next;
}LOGINUSERS;
/*
vector<struct LOGINUSER> loginUsersData;
*/
struct LOGINUSERS* user_init(){
	return NULL;
}
int checkrepeatlogin(struct LOGINUSERS* loginUsersData,
					char* username){
	if(loginUsersData==NULL){
		return 1;
	}
}
char* getUsernameFromToken(struct LOGINUSERS* loginUsersData,char* token){

}
char* getTokenFromUsername(struct LOGINUSERS* loginUsersData,char* username){

}
int user_login(struct LOGINUSERS* loginUsersData,
				char* username,char* password,char** returntoken){

}
int user_signup(struct LOGINUSERS* loginUsersData,
				char* username,char* password,char* nickname){

}
int user_signout(struct LOGINUSERS* loginUsersData,
				char* token){

}
int user_changeNickname(struct LOGINUSERS* loginUsersData,
				char* token,char* newnickname){

}
#endif
