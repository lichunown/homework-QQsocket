//test_hash
#include <glib.h>
#include <stdio.h>
#include "mystring.c"
//EventTable = NULL;

void print_key_value(gpointer key, gpointer value, gpointer user_data)
{
    printf("%s ---> %s\n", (char*)key, (char*)value);
}

void display_hash_table(GHashTable *table)
{
    g_hash_table_foreach(table, print_key_value, NULL);
    printf("\n");
}


int main(){
	GHashTable* Table = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,g_free);
	for(int i=0;i<10;i++){
		char* data = malloc(20);
		char* id = malloc(20);
		sprintf(id,"id%d",i);
		sprintf(data,"outdata%d",i);		
		g_hash_table_insert(Table,id,data);
		//free(data);
		//free(id);
	}
	display_hash_table(Table);
	void* n = g_hash_table_lookup(Table, "id2");
	printf("found %s\n",(char*)n);
	g_hash_table_remove(Table, "id2");
	display_hash_table(Table);
	display_hash_table(Table);
	return 0;
}