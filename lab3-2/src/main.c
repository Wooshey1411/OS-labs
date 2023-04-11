#include "operators.h"
#include "mainUtils.h"
#include "config.h"
#include <stdio.h>
#include "dirent.h"
#include <string.h>

int main(int argc, char *argv[]) {

    char name[MAXNAMLEN + 1];
    unsigned int pointerToSlash = 0;
    size_t lenOfPath = strlen(argv[0]);
    for (int i = 0; i < lenOfPath; ++i) {
        if (argv[0][i] == '/') {
            pointerToSlash = i;
        }
    }
    if (pointerToSlash == 0) {
        strcpy(name, &argv[0][pointerToSlash]);
    } else {
        strcpy(name, &argv[0][pointerToSlash + 1]);
    }

    for (int i = 0; i < COUNT_OF_OPERATORS; ++i) {
        if (!strcmp(name, ALL_OPERATORS[i].nameOfOperation)) {
            return ALL_OPERATORS[i].operator(argc, argv);
        }
    }

    if (argc == 2) {
        if (!strcmp(argv[1], "-h")) {
            printHelp();
            return SUCCESS_CODE;
        }
        if (!strcmp(argv[1], "-c")) {
            return createHardLinks(argv[0]);
        }
    }
    printf("No operator. Use key [-h] for watch a information\n");
    return SUCCESS_CODE;
}
