#include "mainUtils.h"
#include <stdio.h>
#include <unistd.h>
#include "config.h"

void printSpaces(unsigned int countOfSpaces) {
    for (int i = 0; i < countOfSpaces; ++i) {
        printf(" ");
    }
}

void printHelp() {
    for (int i = 0; i < 90; ++i) {
        printf("-");
    }
    printf("\n");
    printf("NAME\n");
    printSpaces(10);
    printf("FileManager - utility for working with files\n");
    printf("\n");

    printf("SYNOPSIS\n");
    printSpaces(10);
    printf("FileManager [-c]\n");
    printf("\n");

    printf("DESCRIPTION\n");
    printSpaces(10);
    printf("Utility works uses hard links for designation of operation\n");
    printSpaces(10);
    printf("Every operation have one hard link with current name\n");
    printf("\n");
    printSpaces(10);
    printf("-c\n");
    printSpaces(15);
    printf("creates hard links for all operations\n");
    printf("\n");

    printf("OPERATIONS\n");
    for (int i = 0; i < COUNT_OF_OPERATORS; ++i) {
        printSpaces(10);
        printf("%s\n", ALL_OPERATORS[i].description1);
        printSpaces(15);
        printf("%s\n", ALL_OPERATORS[i].description2);
        printf("\n");
    }
    printf("\n");

    for (int i = 0; i < 90; ++i) {
        printf("-");
    }
    printf("\n");
}

int createHardLinkToOperator(const char *mainPath, const char *name) {
    if (link(mainPath, name) == ERROR_CODE) {
        perror("link");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}

int createHardLinks(char *mainPath) {
    for (int i = 0; i < COUNT_OF_OPERATORS; ++i) {
        if (createHardLinkToOperator(mainPath, ALL_OPERATORS[i].nameOfOperation) == ERROR_CODE) {
            return ERROR_CODE;
        }
    }
    return SUCCESS_CODE;
}
