#include "operators.h"
#include "mainUtils.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

int main(int argc, char *argv[]) {

    char* name;
    name = basename(argv[0]);

    for (int i = 0; i < COUNT_OF_OPERATORS; ++i) {
        if (!strcmp(name, allOperators[i].nameOfOperation)) {
            return allOperators[i].operator(argc, argv);
        }
    }

    int opt;
    while((opt = getopt(argc,argv,"hc")) != -1){
        switch (opt) {
            case 'h':
                printHelp();
                return SUCCESS_CODE;
            case 'c':
                return createHardLinks(argv[0]);
        }
    }

    printf("No operator. Use key [-h] for watch a information\n");
    return SUCCESS_CODE;
}
