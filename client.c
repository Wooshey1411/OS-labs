#define LOCALHOST "127.0.0.1"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFF_LEN 2048
#define SERVER_PORT 8888

void closeSocket(int fd){
    if(close(fd) < 0){
        perror("close");
    }
}

int main(){
    int socketFd;
    struct sockaddr_in servAddr;
    if((socketFd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("socket");
        return -1;
    }
    printf("TCP Client Socket created\n");
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    servAddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    if(connect(socketFd,(struct sockaddr*)&servAddr,sizeof(servAddr)) < 0){
        perror("connect");
        return -1;
    }

    char buff[BUFF_LEN] = {0};
    printf("Enter message\n");

    while (1){
        if(scanf("%s",buff) < 0){
            closeSocket(socketFd);
            return 0;
        }

        if((send(socketFd,buff, strlen(buff),0)) < 0){
            perror("write");
            closeSocket(socketFd);
            fprintf(stderr,"Critical error! Connection was closed\n");
            return -1;
        }

        if(!strcmp(buff,"quit")){
            closeSocket(socketFd);
            printf("Connection closed\n");
            return 0;
        }

        if((recv(socketFd,buff,BUFF_LEN,0) < 0)){
            perror("read");
            closeSocket(socketFd);
            fprintf(stderr,"Critical error! Connection was closed\n");
            return -1;
        }
        printf("Server answer:%s\n",buff);
    }
}