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
    struct sockaddr_in servAddr, clientAddr;
    int socketFd;
    if((socketFd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
        perror("socket");
        return -1;
    }
    printf("UDP Server Socket created\n");
    bzero(&servAddr,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(socketFd,(struct sockaddr*)&servAddr,sizeof(servAddr)) < 0){
        perror("Error binding socket");
        closeSocket(socketFd);
        return -1;
    }
    printf("UDP Server Socket binded\n");
    char msg[BUFF_LEN] = {0};
    socklen_t len = sizeof(clientAddr);

    while (1){
        ssize_t received;
        if((received = recvfrom(socketFd,msg,BUFF_LEN,0,(struct sockaddr*)&clientAddr,&len)) < 0){
            perror("recv");
            closeSocket(socketFd);
            fprintf(stderr,"critical error: server shutdown\n");
            return -1;
        }
        msg[received] = '\0';
        printf("%s:%d sent:%s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port),msg);

        if((sendto(socketFd,msg,received,0,(struct sockaddr*)&clientAddr,len)) < 0){
            perror("recv");
            closeSocket(socketFd);
            fprintf(stderr,"critical error: server shutdown\n");
            return -1;
        }
    }
}