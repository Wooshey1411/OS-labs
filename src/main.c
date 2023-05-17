#include <stdio.h>
#include "heap.h"
#include <string.h>

int main() {
    char* string = myMalloc(100*sizeof(char));
    strcpy(string,"abc");
    printf("%s\n",string);
    myFree(string);
    char* newString = myMalloc(100*sizeof(char));
    strcpy(newString,"hello world");
    printf("%s\n",newString);
    dumpHeap("heap.dump");
    myFree(newString);
    destroyHeap();
    return 0;
}
