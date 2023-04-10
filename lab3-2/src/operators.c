#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE
#define K_BYTE 1024
#include "operators.h"
#include "utils.h"
#include "config.h"
#include "sys/stat.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>
#include <fcntl.h>
#include <unistd.h>

int makeDirectory(int argc, char *argv[]) {
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    // mode: user - read and write.
    if (mkdir(argv[1], S_IRUSR | S_IWUSR) != SUCCESS_CODE) {
        perror("mkdir");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int printDirectoryContents(int argc, char* argv[]){
    if(argc != 2){
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    DIR *directory = NULL;
    if ((directory = opendir(argv[1])) == NULL){
        perror("opendir");
        return ERROR_CODE;
    }
    struct dirent *directoryInfo;
    char type[20]; // length must be not less than count of symbols at longest type + 1
    while ((directoryInfo = readdir(directory)) != NULL){
        switch (directoryInfo->d_type) {
            case DT_BLK:
                strcpy(type,"block device");
                break;
            case DT_CHR:
                strcpy(type,"char device");
                break;
            case DT_DIR:
                strcpy(type,"directory");
                break;
            case DT_FIFO:
                strcpy(type,"named chanel(FIFO)");
                break;
            case DT_LNK:
                strcpy(type,"symbolic link");
                break;
            case DT_REG:
                strcpy(type,"regular");
                break;
            case DT_SOCK:
                strcpy(type,"domain socket UNIX");
                break;
            case DT_UNKNOWN:
                strcpy(type,"unknown");
                break;
        }
        printf("file %s is %s\n",directoryInfo->d_name,type);
    }
    if(closedir(directory) != SUCCESS_CODE){
        perror("closedir");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int removeFileAtPath(char* path){
    if( remove(path) == ERROR_CODE){
        perror("remove");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int removeDirectory(int argc, char* argv[]){
    if(argc != 2){
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    if(nftw(argv[1],(__nftw_func_t)removeFileAtPath,64, FTW_DEPTH | FTW_PHYS) == ERROR_CODE){
        perror("ftw");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int createFile(int argc, char* argv[]){
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    if(creat(argv[1],S_IREAD | S_IWRITE | S_IRGRP | S_IROTH) == ERROR_CODE){
        perror("creat");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int printFileContent(int argc, char* argv[]){
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    FILE* file = NULL;
    if((file = fopen(argv[1],"rb")) == NULL){
        perror("fopen");
        return ERROR_CODE;
    }
    char buffer[K_BYTE+1];
    while(!feof(file)){
        size_t countOfReadChars = fread(buffer,sizeof(char),K_BYTE,file);
        if(ferror(file)){
            perror("fread");
            return ERROR_CODE;
        }
        buffer[countOfReadChars] = '\0';
        printf("%s",buffer);
    }
    if(fclose(file) == ERROR_CODE){
        perror("fclose");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int removeFile(int argc, char* argv[]){
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    return removeFileAtPath(argv[1]);
}

int createSymbolLink(int argc, char* argv[]){
    if (argc != 3) {
        printErrorOfArgs(2);
        return ERROR_CODE;
    }
    if(symlink(argv[1],argv[2]) == ERROR_CODE){
        perror("symlink");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int printSymbolLinkContent(int argc, char* argv[]){
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    char buffer[MAXNAMLEN + 1];
    size_t countOfReadChars;
    if((countOfReadChars = readlink(argv[1],buffer,MAXNAMLEN)) == ERROR_CODE){
        perror("readlink");
        return ERROR_CODE;
    }
    buffer[countOfReadChars] = '\0';
    printf("%s\n",buffer);
    return SUCCESS_CODE;
}
