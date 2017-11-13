//test_hash
#include <glib.h>
#include <stdio.h>
#include "mystring.c"
//EventTable = NULL;
int main(){
	printf("%s",ptoa((void*)94827128201452));
	printf("-----------\n");
	GHashTable* EventTable = g_hash_table_new(g_str_hash, g_str_equal);
	char* data = "tttttt";
	int *d = malloc(4);
	unsigned long a;size_t b; int c;
	printf("-----------\n");
	printf("unsigned long:%ld  size_t:%ld int:%ld pointer:%ld",sizeof(a),sizeof(b),sizeof(c),sizeof(d));
	printf("\n`%ld`\n",(unsigned long)data);
	printf("`%s`",ptoa((void*)data));
	printf("-----------\n");
	g_hash_table_insert(EventTable, ptoa((void*)data), data);
	char* r = (char*)g_hash_table_lookup(EventTable, ptoa((void*)data));
	printf("%s",r);

	return 0;
}