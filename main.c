#include <stdio.h>
#include "sys/stat.h"
#include <dirent.h>
#include <malloc.h>
#include <string.h>

#define K_BYTE 1024

int reverseFile(FILE *inputFile, FILE *outputFile, long sizeofFile) {

    if(inputFile == NULL){
        fprintf(stderr, "input file is null");
        return -1;
    }
    if(outputFile == NULL){
        fprintf(stderr, "output file is null");
        return -1;
    }

    if(sizeofFile < 0){
        fprintf(stderr,"size of file less than 0");
        return -1;
    }

    long offset = -K_BYTE;
    if (sizeofFile <= 0) {
        return 0;
    }
    long rest = sizeofFile;
    long countOfElementsInBuff;
    char buffer[K_BYTE + 1];
    char writeBuffer[K_BYTE + 1];
    while (1) {
        if (rest < K_BYTE) {
            if(fseek(inputFile, 0, SEEK_SET) != 0){
                perror("fseek");
                return -1;
            }
            fread(buffer, sizeof(char), rest, inputFile);
            countOfElementsInBuff = rest;
        } else {
            if(fseek(inputFile, offset, SEEK_END) != 0){
                perror("fseek");
                return -1;
            }
            fread(buffer, sizeof(char), K_BYTE, inputFile);
            countOfElementsInBuff = K_BYTE;
        }

        if (ferror(inputFile)) {
            perror("fread");
            return -1;
        }

        for (long i = countOfElementsInBuff - 1; i >= 0; --i) {
            writeBuffer[countOfElementsInBuff - 1 - i] = buffer[i];
        }
        fwrite(writeBuffer, sizeof(char), countOfElementsInBuff, outputFile);
        if (ferror(outputFile)) {
            perror("fwrite");
            return -1;
        }
        offset -= countOfElementsInBuff;
        rest -= countOfElementsInBuff;
        if (rest == 0) {
            return 0;
        }
    }
}

char *reversePath(char const *string, size_t len) {
    if(string == NULL){
        fprintf(stderr,"Input string is null");
        return NULL;
    }
    char *rev = malloc(sizeof(char) * len + 1);
    if (rev == NULL) {
        perror("malloc");
        return NULL;
    }

    size_t leftP = 0;
    size_t rightP;

    for (int i = 0; i < len; ++i) {
        if (string[i] == '/' || i == len - 1) {
            if (i == len - 1) {
                rightP = len;
            } else {
                rightP = i;
            }
            for (size_t j = leftP; j < rightP; ++j) {
                rev[j] = string[rightP - 1 - j + leftP];
            }
            if (i != len - 1 || string[len - 1] != '/') {
                rev[rightP] = '/';
            }
            leftP = rightP + 1;
        }
    }
    rev[len] = '\0';
    return rev;
}

void reverseDir(char *currDirectory, size_t dirLength) {
    if (currDirectory == NULL) {
        fprintf(stderr, "current directory is null");
        return;
    }
    char *revDirPath = reversePath(currDirectory,dirLength);
    if (revDirPath == NULL) {
        return;
    }

    FILE *dirF = NULL;
    if ((dirF = fopen(currDirectory, "rb")) == NULL) {
        perror("fopen");
        free(revDirPath);
        return;
    }
    struct stat dirStat;
    if (fstat(fileno(dirF), &dirStat) != 0) {
        perror("fstat");
        free(revDirPath);
        fclose(dirF);
        return;
    }

    if (mkdir(revDirPath, dirStat.st_mode) != 0) {
        perror("mkdir");
        free(revDirPath);
        fclose(dirF);
        return;
    }

    free(revDirPath);

    DIR *dir = NULL;
    struct dirent *directoryInfo;
    if ((dir = opendir(currDirectory)) == NULL) {
        perror("opendir");
        return;
    }
    while ((directoryInfo = readdir(dir)) != NULL) {
        size_t fileLen = strlen(directoryInfo->d_name);
        if (!strcmp(directoryInfo->d_name, ".") || !strcmp(directoryInfo->d_name, "..")) {
            continue;
        }

        char path[dirLength+fileLen+2];

        strcpy(&path[0], currDirectory);
        path[dirLength] = '/';
        strcpy(&path[dirLength + 1], directoryInfo->d_name);

        struct stat fileStat;
        FILE *file;
        if ((file = fopen(path, "rb")) == NULL) {
            perror("fopen");
            continue;
        }
        if (fstat(fileno(file), &fileStat) != 0) {
            perror("fstat");
            fclose(file);
            continue;
        }


        if (S_ISREG(fileStat.st_mode)) {
            printf("file is regular: %s\n", path);
            char *revPath = reversePath(path,dirLength+fileLen+1);
            if (revPath == NULL) {
                fclose(file);
                continue;
            }
            FILE *outputFile = NULL;
            if ((outputFile = fopen(revPath, "wb")) == NULL) {
                perror("fopen");
                free(revPath);
                fclose(file);
                continue;
            }
            int code = reverseFile(file, outputFile, fileStat.st_size);
            if(code != 0){
                fclose(outputFile);
                fclose(file);
                free(revPath);
                continue;
            }

            if (fchmod(fileno(outputFile), fileStat.st_mode) != 0) {
                perror("chmod");
            }

            fclose(outputFile);
            free(revPath);
        }
        if (S_ISDIR(fileStat.st_mode)) {
            printf("file is directory: %s\n", path);
            reverseDir(path,dirLength+fileLen+1);
        }
        fclose(file);
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("count of arguments must be 1 \n");
        return 0;
    }
    reverseDir(argv[1], strlen(argv[1]));
    return 0;
}
