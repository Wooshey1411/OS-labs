#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define BUFFER_SIZE 2048
#define SERVER_PORT 8888

void closeSocket(int fd) {
    if (close(fd) < 0) {
        perror("close");
    }
}

void sigchld_handler() {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {

    struct sockaddr_in servAddr, clientAddr;
    int socketFd;
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    printf("TCP Server Socket created\n");

    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("Error binding socket");
        closeSocket(socketFd);
        return -1;
    }

    printf("TCP Server Socket binded\n");

    if (listen(socketFd, 10) < 0) {
        perror("listen");
        closeSocket(socketFd);
        return -1;
    }

    printf("Server is listening on %d\n", SERVER_PORT);

    struct sigaction sig_a;
    sig_a.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sig_a.sa_mask);
    sig_a.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sig_a, NULL) == -1) {
        fprintf(stderr,"register signal handler failed.\n");
        return -1;
    }

    int clientFd;
    socklen_t len;
    char buffer[BUFFER_SIZE] = {0};
    pid_t clientPid;
    while (1) {
        if ((clientFd = accept(socketFd, (struct sockaddr *) &clientAddr, &len)) < 0) {
            perror("accept");
            continue; // or exit ?
        }
        if ((clientPid = fork()) < 0) {
            perror("fork");
            closeSocket(socketFd);
            closeSocket(clientFd);
            return -1;
        } else if (clientPid == 0) { // child
            closeSocket(socketFd);


            while (1) {
                if ((recv(clientFd, buffer, BUFFER_SIZE, 0) < 0)) {
                    perror("recv");
                    closeSocket(clientFd);
                    fprintf(stderr, "critical error: session shutdown\n");
                    return -1;
                }


                if (!strcmp(buffer, "quit") || strlen(buffer) == 0) {
                    closeSocket(clientFd);
                    printf("Session finished\n");
                    return 0;
                }

                printf("%s:%d sent:%s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);

                if ((send(clientFd, buffer, strlen(buffer), 0)) < 0) {
                    perror("send");
                    closeSocket(clientFd);
                    fprintf(stderr, "critical error: session shutdown\n");
                    return -1;
                }
            }
        }

        printf("child process: %d\n", clientPid);
        closeSocket(clientFd);
    }

}