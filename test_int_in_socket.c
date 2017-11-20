#include "mysocket.c"
#include "encode.c"
int main(){
    int a = 0;
    int sockfd = CreateClient("0.0.0.0",9000);
    while(1){
        Recv(sockfd,&a,sizeof(a),MSG_WAITALL);
        printf("recv data of a:\n");
        print16((char*)&a,sizeof(a));
        printf("int:`%d`\n",a);   
        a = ntohs(a);  
        printf("change a ntohs:\n");
        print16((char*)&a,sizeof(a));
        printf("int:`%d`\n",a);             
    }
    close(sockfd);
}