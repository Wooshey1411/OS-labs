#include "operators.h"
#include "utils.h"
#include "config.h"
#include "sys/stat.h"
#include <stdio.h>

int makeDirectory(int argc, char *argv[]) {
    if (argc != 2) {
        printErrorOfArgs(1);
        return ERROR_CODE;
    }
    // mode: user - read and write.
    if (mkdir(argv[1], S_IRUSR | S_IWUSR) != 0) {
        perror("mkdir");
        return ERROR_CODE;
    }
    return SUCCESS_CODE;
}