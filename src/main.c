#include <stdio.h>
#include "heap.h"
#include <string.h>

int main() {
    char* string = myMalloc(100*sizeof(char));
    memset(string,'a',100);
    strcpy(string,"Hello world!");
    printf("%s\n",string);
    char* string2 = myMalloc(100*sizeof(char));
    memset(string2,'b',100);
    strcpy(string2,"Hello world!");
    printf("%s\n",string2);
    char* string3 = myMalloc(100*sizeof(char));
    memset(string3,'c',100);
    strcpy(string3,"Hello world!");
    printf("%s\n",string3);
    char* string4 = myMalloc(100*sizeof(char));
    memset(string4,'d',100);
    strcpy(string4,"Hello world!");
    printf("%s\n",string4);

    myFree(string2);
    myFree(string3);
    char* string5 = myMalloc(100*sizeof(char));
    memset(string5,'e',100);
    strcpy(string5,"Hello world!");
    printf("%s\n",string5);

    destroyHeap();
    return 0;
}
