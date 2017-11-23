#ifndef G_HASH_EXTEND_C
#define G_HASH_EXTEND_C 0

#include <stdio.h>
struct find_key_by_value_s{
	char* value;
	char* result;
};
void my_g_hash_existed(gpointer key,gpointer value,struct find_key_by_value_s* data){
	char* findvalue = (char*)(data->value);
	if(strcmp(findvalue,value)==0){
		printf("my_g_hash_existed: `%s`?`%s`",(char*)findvalue,(char*)value);
		data->result = key;
	}
	else{
	}
}

gpointer find_g_hash_by_value(GHashTable *hash_table,char* value){
	struct find_key_by_value_s data;
	data.value = value;
	data.result = NULL;
	g_hash_table_foreach(hash_table,(GHFunc)my_g_hash_existed,&data);
	return data.result;
}

#endif