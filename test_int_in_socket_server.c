#include "mysocket.c"
#include "encode.c"

#define NUM 48
int main(){
    int a = NUM;
    int sockfd = CreateServer(9000,10);
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    int clientfd = Accept(sockfd,(struct sockaddr*)&clientaddr,&addrlen);
    Send(clientfd,&a,sizeof(a),0);
    printf("send data of a:\n");
    print16((char*)&a,sizeof(a));
    printf("int:`%d`\n",a);
    a = htons(NUM);
    Send(clientfd,&a,sizeof(a),0);
    printf("send #htons# data of a:\n");
    print16((char*)&a,sizeof(a));
    printf("int:`%d`\n",a);    
    close(sockfd);
}