#include "stdio.h"


void* myMalloc(size_t size);

void myFree(void* p);

void dumpHeap(char* path);

void destroyHeap();