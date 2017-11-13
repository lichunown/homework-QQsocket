//test_mystring
#include "mystring.c"
#include <stdio.h>
int main(int argc,char* argv[]){
	// char* data1 = "qwer qwer2 qwer3 qwer4";
	// char** sp1 = split(data1);
	// printf("split:\n\tstr1:`%s`\n\tstr2:`%s`\n",sp1[0],sp1[1]);
	// char** sp2 = split_num(data1,4);
	// printf("split_num:\n\tstr1:`%s`\n\tstr2:`%s`\n\tstr3:`%s`\n\tstr4:`%s`",sp2[0],sp2[1],sp2[2],sp2[3]);
	// free_splitdata(sp1);
	// free_splitdata_num(sp2,4);
	while(1){
		char inputs[80];
		bzero(inputs,80);
		printf(">>> ");
		fgets(inputs,80,stdin);
		printf("\nyou input:\n`%s`\n",inputs);
		for(int spliti=2;spliti<5;spliti++){
			char** sp = split_num(inputs,spliti);
			printf("split %d\n",spliti);
			for(int i = 0;i < spliti;i++){
				printf("\t%d: `%s`\n",i,sp[i]);
			}
			free_splitdata_num(sp,spliti);
		}	
	}
	return 0;
}