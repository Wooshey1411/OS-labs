#include "utils.h"
#include <stdio.h>

void printErrorOfArgs(int countOfArguments){
    fprintf(stderr,"count of arguments must be %d\n",countOfArguments);
}