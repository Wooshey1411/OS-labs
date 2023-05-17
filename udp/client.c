#define LOCALHOST "127.0.0.1"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFF_LEN 2048
#define SERVER_PORT 7344

void closeSocket(int fd){
    if(close(fd) < 0){
        perror("close");
    }
}

int main(){
    int socketFd;
    struct sockaddr_in servAddr;
    if((socketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
        perror("socket");
        return -1;
    }
    printf("UDP Client Socket created\n");
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    servAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    char buff[BUFF_LEN] = {0};
    socklen_t len = sizeof(servAddr);
    printf("Enter message\n");

    while (1){
        if(scanf("%s",buff) < 0 || !strcmp(buff,"quit")){
            closeSocket(socketFd);
            return 0;
        }

        ssize_t sent;

        if((sent = sendto(socketFd,buff, strlen(buff),0,(struct  sockaddr*)&servAddr,sizeof(servAddr))) < 0){
            perror("send");
            closeSocket(socketFd);
            fprintf(stderr,"Critical error! Connection was closed\n");
            return -1;
        }

        if((recvfrom(socketFd,buff,sent,0,(struct  sockaddr*)&servAddr,&len)) < 0){
            perror("recvfrom");
            closeSocket(socketFd);
            fprintf(stderr,"Critical error! Connection was closed\n");
            return -1;
        }
        printf("Server answer:%s\n",buff);
    }
}
