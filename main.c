#include <stdio.h>
#include "sys/stat.h"
#include <dirent.h>
#include <malloc.h>
#include <string.h>

#define K_BYTE 1024

void reverseFile(FILE* inputFile, FILE* outputFile, long sizeofFile){

    long offset = -K_BYTE;
    if(sizeofFile == 0){
        return;
    }
    long rest = sizeofFile;
    long countOfElementsInBuff = 0;
    char buffer[K_BYTE+1];
    char writeBuffer[K_BYTE+1];
    while(1) {
            if(rest >= K_BYTE) {
                fseek(inputFile, offset, SEEK_END);
            } else{
                fseek(inputFile, 0, SEEK_SET);
            }
        if (rest < K_BYTE) {
            fread(buffer, sizeof(char), rest, inputFile);
            countOfElementsInBuff = rest;
        } else {
            fread(buffer, sizeof(char), K_BYTE, inputFile);
            countOfElementsInBuff = K_BYTE;
        }
        for (long i = countOfElementsInBuff - 1; i >= 0; --i) {
            writeBuffer[countOfElementsInBuff - 1 - i] = buffer[i];
        }
        fwrite(writeBuffer, sizeof(char), countOfElementsInBuff, outputFile);
        offset -= countOfElementsInBuff;
        rest -= countOfElementsInBuff;
        if (rest == 0) {
            return;
        }
    }
}

char* reversePath(char* string){
    size_t len = strlen(string);
    char* rev = malloc(sizeof(char)*len + 1);
    if(rev == NULL){
        return NULL;
    }

    size_t leftP=0;
    size_t rightP=0;

    for (int i = 0; i < len; ++i) {
        if(string[i] == '/' || i == len-1){
            if(i == len-1) {
                rightP = len;
            } else{
                rightP = i;
            }
            for (size_t j = leftP; j < rightP; ++j) {
                rev[j] = string[rightP-1-j+leftP];
            }
            if(i != len -1 || string[len-1] != '/') {
                rev[rightP] = '/';
            }
            leftP = rightP+1;
        }
    }
    rev[len] = '\0';
    return rev;
}

void reverseDir(char* currDirectory){
    if(currDirectory == NULL){
        return;
    }
    char* revDirPath = reversePath(currDirectory);
    FILE* dirF = NULL;
    if((dirF = fopen(currDirectory,"rb")) == NULL){
        printf("%s: Cannot open directory\n",currDirectory);
        free(revDirPath);
        return;
    }
    struct stat dirBuffer;
    fstat(fileno(dirF),&dirBuffer);
    mkdir(revDirPath, dirBuffer.st_mode);
    free(revDirPath);

    DIR *dir = NULL;
    struct dirent* A;
    if((dir = opendir(currDirectory)) != NULL){
        while ((A = readdir(dir)) != NULL){
            size_t dirLen = strlen(currDirectory);
            size_t fileLen = strlen(A->d_name);
            if(!strcmp(A->d_name,".") || !strcmp(A->d_name,"..")){
                continue;
            }
            char* path = malloc(sizeof(char) * (dirLen + fileLen + 2));
            if(path == NULL){
                printf("failed to malloc\n");
                continue;
            }

            for (size_t i = 0; i < dirLen; ++i) {
                path[i] = currDirectory[i];
            }
            path[dirLen] = '/';
            for (size_t i = 0; i < fileLen; ++i) {
                path[i+1+dirLen] = A->d_name[i];
            }
            path[dirLen+fileLen+1] = '\0';
            struct stat buffer;
            FILE* file;
            if((file = fopen(path,"rb")) == NULL){
                printf("%s: Cannot open file\n",path);
                free(path);
                continue;
            }
            fstat(fileno(file),&buffer);

            if(S_ISREG(buffer.st_mode)){
                printf("file is regular: %s\n",path);
                char* revPath = reversePath(path);
                if(revPath == NULL){
                    printf("failed malloc\n");
                    free(path);
                    fclose(file);
                    continue;
                }
                FILE* outputFile = NULL;
                if((outputFile = fopen(revPath,"wb")) == NULL){
                    printf("%s: failed to open file\n",revPath);
                    free(path);
                    free(revPath);
                    fclose(file);
                    continue;
                }
                reverseFile(file,outputFile,buffer.st_size);
                fchmod(fileno(outputFile),buffer.st_mode);
                fclose(outputFile);
                free(revPath);
            }
            if(S_ISDIR(buffer.st_mode)) {
                printf("file is directory: %s\n",path);
                reverseDir(path);
            }
            fclose(file);
            free(path);
        }
        closedir(dir);
    } else{
        printf("%s: cannot open the directory\n",currDirectory);
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        printf("count of arguments must be 1 \n");
        return 0;
    }
    reverseDir(argv[1]);
    return 0;
}
