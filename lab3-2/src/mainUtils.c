#include "mainUtils.h"
#include <stdio.h>
#include <unistd.h>
#include "config.h"
#include <string.h>

#define COUNT_OF_DELIMITER_SYM 90
#define COUNT_OF_SPACES_SM 10
#define COUNT_OF_SPACES_BIG 15

void printHelp() {
    char delimiters[COUNT_OF_DELIMITER_SYM+1] = {'\0'};
    memset(delimiters,'-',COUNT_OF_DELIMITER_SYM);

    char spacesSm[COUNT_OF_SPACES_SM+1] = {'\0'};
    memset(spacesSm,' ',COUNT_OF_SPACES_SM);

    char spacesBig[COUNT_OF_SPACES_BIG+1] = {'\0'};
    memset(spacesBig,' ',COUNT_OF_SPACES_BIG);

    printf("%s\n",delimiters);


    printf("NAME\n");
    printf("%s",spacesSm);
    printf("FileManager - utility for working with files\n");
    printf("\n");

    printf("SYNOPSIS\n");
    printf("%s",spacesSm);
    printf("FileManager [-c]\n");
    printf("\n");

    printf("DESCRIPTION\n");
    printf("%s",spacesSm);
    printf("Utility works uses hard links for designation of operation\n");
    printf("%s",spacesSm);
    printf("Every operation have one hard link with current name\n");
    printf("\n");
    printf("%s",spacesSm);
    printf("-c\n");
    printf("%s",spacesBig);
    printf("creates hard links for all operations\n");
    printf("\n");

    printf("OPERATIONS\n");
    for (int i = 0; i < COUNT_OF_OPERATORS; ++i) {
        printf("%s",spacesSm);
        printf("%s\n", allOperators[i].description1);
        printf("%s",spacesBig);
        printf("%s\n", allOperators[i].description2);
        printf("\n");
    }
    printf("\n");

    printf("%s\n",delimiters);
}

int createHardLinkToOperator(const char *mainPath, const char *name) {
    if (link(mainPath, name) != SUCCESS_CODE) {
        perror("link");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int createHardLinks(char *mainPath) {
    for (int i = 0; i < COUNT_OF_OPERATORS; ++i) {
        if (createHardLinkToOperator(mainPath, allOperators[i].nameOfOperation) != SUCCESS_CODE) {
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}
