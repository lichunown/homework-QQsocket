#ifndef FILETRANSPORT_C
#define FILETRANSPORT_C 0

#include "mysocket.c"
#include <stdlib.h>
#include <sys/stat.h>  
#include <stdio.h>  
#include <time.h>  
#include <sys/types.h> 
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include "mystruct.c"
#define FILEPERLEN 1024

struct SEND_FILE* recv_sendfile_head(int sockfd);
void mergeFiles(char* filename, unsigned int maxid, unsigned int perlength, unsigned long filelength);
struct SEND_FILE* sendfile_head(char* filename, unsigned int id, unsigned int datalen, unsigned long filelength);
void client_sendfile(int sockfd,char* token,char* path,char* filename);
int trave_dir(char* path, struct file_list** data);


struct SEND_FILE* recv_sendfile_head(int sockfd){
	struct SEND_FILE* data = malloc(sizeof(struct SEND_FILE));
	Recv(sockfd, data,sizeof(struct SEND_FILE),0);
	return data;
}

void client_startrecv_id(int sockfd,char* token, char* filename,int id,long filelength){
	struct HEAD_MAIN head_main;
	head_main.mode = 1;
	Send(sockfd,&head_main,sizeof(head_main),0);
	struct HEAD_DATA* datahead = data_recvfile(token);
	Send(sockfd,datahead,sizeof(struct HEAD_DATA),0);
	free(datahead);
	struct SEND_FILE* filehead = malloc(sizeof(struct SEND_FILE));
	bzero(filehead,sizeof(struct SEND_FILE));
	strcpy(filehead->filename,filename);
	filehead->id = id;
	filehead->perlength = FILEPERLEN;
	filehead->filelength = filelength;
	Send(sockfd,filehead,sizeof(struct SEND_FILE),0);
}

void client_recvfile(int sockfd,char* path,char* token){
	char savepath[255];
	struct SEND_FILE* filehead = malloc(sizeof(struct SEND_FILE));
	Recv(sockfd,filehead,sizeof(struct SEND_FILE),0);
	sprintf(savepath,"%s/%s_%d",path,filehead->filename,filehead->id);
	int f = open(savepath, O_WRONLY|O_CREAT,S_IRWXU);
	void* data = malloc(filehead->perlength);
	Recv(sockfd,data,filehead->perlength,0);
	if(f<0){
		printf("[recv file]: create file`%s` error.\n",savepath);
	}
	write(f,data,filehead->perlength);
	close(f);
	printf("[recv file]: create file`%s`.\n",savepath);
	free(data);
	if(filehead->perlength * (filehead->id + 1) < filehead->filelength){
		client_startrecv_id(sockfd,token,filehead->filename,filehead->id + 1,filehead->filelength);
	}else{
		sprintf(savepath,"%s/%s",path,filehead->filename);
		mergeFiles(savepath, filehead->id, filehead->perlength, filehead->filelength);
	}
	free(filehead);
}

struct SEND_FILE* sendfile_head(char* filename, unsigned int id, unsigned int datalen, unsigned long filelength){
	struct SEND_FILE* filehead = malloc(sizeof(struct SEND_FILE));
	strcpy(filehead->filename, filename);
	filehead->id = id;
	filehead->perlength = datalen;
	filehead->filelength = filelength;
	return filehead;
}
void client_sendfile(int sockfd,char* token,char* path,char* filename){
	if((access(path, 0)) != -1 ){ //exit
		int f = open(path,O_RDONLY);
		if(f==-1){
			printf("Open file error.\n");
			return;
		}
		 struct stat statbuff;
		 if(stat(path, &statbuff) < 0){
		 	printf("get file msg error.\n");
		 	return;
		 }
		unsigned long filesize = statbuff.st_size;  
		char perdata[FILEPERLEN];
		bzero(perdata,FILEPERLEN);
		unsigned int id = 0;
		struct HEAD_MAIN head_main;
		head_main.mode = 1;		
		struct HEAD_DATA* head = data_sendfile(token);
		while(read(f, perdata, FILEPERLEN) > 0){
			Send(sockfd,&head_main,sizeof(head_main),0);							
			Send(sockfd, head, sizeof(struct HEAD_DATA), 0);
			struct SEND_FILE* sendhead = sendfile_head(filename,id,FILEPERLEN,filesize);
			Send(sockfd, sendhead, sizeof(struct SEND_FILE), 0);
			free(sendhead);
			Send(sockfd, perdata, FILEPERLEN, 0);
			id++;
		}
		free(head);
		close(f);
	}
}


void mergeFiles(char* filename, unsigned int maxid, unsigned int perlength, unsigned long filelength){
	printf("[merge]: start merge `%s` perlength:%d\n",filename,perlength);
	int finalfile = open(filename, O_WRONLY|O_CREAT, S_IRWXU);
	char perfilepath[255];
	char* perdata = malloc(perlength);
	bzero(perdata,perlength);
	for(int i = 0;i < maxid;i++){
		sprintf(perfilepath,"%s_%d",filename,i);
		int perfile = open(perfilepath, O_RDONLY);
		if(perfile < 0){
			printf("[merge] error: can not open %s\n",perfilepath);
			return;
		}
		read(perfile, perdata, perlength);
		close(perfile);
		write(finalfile, perdata, perlength);
	}
	sprintf(perfilepath,"%s_%d",filename,maxid);
	int perfile = open(perfilepath,O_RDONLY);
	if(perfile < 0){
		printf("[merge] error: can not open %s\n",perfilepath);
		return;
	}
	printf("[merge]: the filnal %ld \n",filelength - perlength*maxid);
	read(perfile, perdata, filelength - perlength*maxid);
	close(perfile);
	write(finalfile, perdata, filelength - perlength*maxid);
	close(finalfile);
	free(perdata);
	for(int i = 0;i < maxid + 1;i++){
		sprintf(perfilepath,"%s_%d",filename,i);
		remove(perfilepath);
	}	
	printf("[merge]: <filename:%s> \t\t\t[FINISH]\n",filename);
}

// struct file_list{
// 	char name[32];
// 	int i;
// 	struct file_list* next;
// };

int trave_dir(char* path, struct file_list** data){
    DIR *d;//声明一个句柄
    struct dirent *file;//readdir函数的返回值就存放在这个结构体中
    struct stat statbuf; 
    bzero(&statbuf,sizeof(statbuf)); 
    char allpath[255];
    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\n",path);
        return -1;
    }
    *data = malloc(sizeof(struct file_list));
    struct file_list* p = *data;
    bzero(p,sizeof(struct file_list));
    int i = 0;
    while((file = readdir(d)) != NULL)
    {
       //把当前目录.，上一级目录..及隐藏文件都去掉，避免死循环遍历目录
        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
        if(stat(file->d_name, &statbuf)<0){
        	printf("file:%s getsize error %d.\n",file->d_name,errno);
        }
        strcpy(p->name,file->d_name);
        p->i = i;
        sprintf(allpath,"%s/%s",path,p->name);
        p->size = statbuf.st_size;
        printf("%s:%ld\n",allpath,statbuf.st_size);
        i++;
        p->next = malloc(sizeof(struct file_list));
        bzero(p->next,sizeof(struct file_list));
        p = p->next;
    }
    closedir(d);
    return i;
}

#endif