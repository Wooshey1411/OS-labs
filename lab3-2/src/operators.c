#define _XOPEN_SOURCE 500
#define _DEFAULT_SOURCE
#define K_BYTE 1024
#include "operators.h"
#include "config.h"
#include "sys/stat.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ftw.h>
#include <fcntl.h>
#include <unistd.h>

void printErrorOfArgs(int countOfArguments){
    fprintf(stderr,"count of arguments must be %d\n",countOfArguments);
}

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

int printDirectoryContent(int argc, char* argv[]){
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
            fclose(file);
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

int printSymbolLinkSourceContent(int argc, char* argv[]){
    return printFileContent(argc,argv);
}

int removeSymbolLink(int argc, char* argv[]){
    return removeFile(argc,argv);
}

int createHardLink(int argc, char* argv[]){
    if (argc != 3) {
        printErrorOfArgs(2);
        return ERROR_CODE;
    }
    if(link(argv[1],argv[2]) == ERROR_CODE){
        perror("link");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int removeHardLink(int argc, char* argv[]){
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    if(unlink(argv[1]) == ERROR_CODE){
        perror("unlink");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int printPermissionsAndCountOfHardLinks(int argc, char* argv[]){
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    FILE* file = NULL;
    if((file = fopen(argv[1],"rb")) == NULL){
        perror("fopen");
        return ERROR_CODE;
    }
    struct stat fileStat;
    if(fstat(fileno(file),&fileStat) == ERROR_CODE){
        perror("fstat");
        fclose(file);
        return ERROR_CODE;
    }
    char rights[10]; // u ---, g ---, o --- + /0 = 10
    // rights : u rwx g rwx o rwx
    rights[0] = fileStat.st_mode & S_IREAD ? 'r' : '-';
    rights[1] = fileStat.st_mode & S_IWRITE ? 'w' : '-';
    rights[2] = fileStat.st_mode & S_IEXEC ? 'x' : '-';

    rights[3] = fileStat.st_mode & S_IRGRP ? 'r' : '-';
    rights[4] = fileStat.st_mode & S_IWGRP ? 'w' : '-';
    rights[5] = fileStat.st_mode & S_IXGRP ? 'x' : '-';

    rights[6] = fileStat.st_mode & S_IROTH ? 'r' : '-';
    rights[7] = fileStat.st_mode & S_IWOTH ? 'w' : '-';
    rights[8] = fileStat.st_mode & S_IXOTH ? 'x' : '-';

    rights[9] = '\0';

    printf("Count of hard links:%li. Rights:%s\n",fileStat.st_nlink,rights);

    if(fclose(file) == ERROR_CODE){
        perror("fclose");
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

int setRights(int argc, char* argv[]){
    if (argc != 3) {
        printErrorOfArgs(2);
        return ERROR_CODE;
    }
    // 9 rights
    if(strlen(argv[2]) != 9){
        fprintf(stderr,"count of rights must be 9\n");
        return ERROR_CODE;
    }
    __mode_t mode = 0; // no rights

    // check of correctness of rights
    for (int i = 0; i < 3; ++i) {
        if(argv[2][i*3] != 'r' && argv[2][i*3] != '-'){
            fprintf(stderr,"incorrect rights\n");
            return ERROR_CODE;
        }
        if(argv[2][i*3 + 1] != 'w' && argv[2][i*3 + 1] != '-'){
            fprintf(stderr,"incorrect rights\n");
            return ERROR_CODE;
        }
        if(argv[2][i*3 + 2] != 'x' && argv[2][i*3 + 2] != '-'){
            fprintf(stderr,"incorrect rights\n");
            return ERROR_CODE;
        }
    }
    // mapping rights: ___ ___ ___ -> mode
    (argv[2][0] == 'r') ? mode |= S_IREAD : mode;
    (argv[2][1] == 'w') ? mode |= S_IWRITE : mode;
    (argv[2][2] == 'x') ? mode |= S_IEXEC : mode;

    (argv[2][3] == 'r') ? mode |= S_IRGRP : mode;
    (argv[2][4] == 'w') ? mode |= S_IWGRP : mode;
    (argv[2][5] == 'x') ? mode |= S_IXGRP : mode;

    (argv[2][6] == 'r') ? mode |= S_IROTH : mode;
    (argv[2][7] == 'w') ? mode |= S_IWOTH : mode;
    (argv[2][8] == 'x') ? mode |= S_IXOTH : mode;

    if(chmod(argv[1],mode) == ERROR_CODE){
        perror("chmod");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}