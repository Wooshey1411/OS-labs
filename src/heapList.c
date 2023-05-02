#include "heapList.h"
#include <string.h>

size_t getSizeOfNode(){
    return sizeof(struct HeapNode);
}

struct HeapNode* createNode(void* begin, struct HeapNode* prev, unsigned int isStart, size_t countOfBytes){
    struct HeapNode node;
    for (int i = 0; i < 4; ++i) {
        node.preamble[i] = PREAMBLE_VALUE;
    }
    if(isStart){
        node.isAllocated = 1;
        node.countOfBytes = countOfBytes;
    } else{
        node.isAllocated = 0;
        node.countOfBytes = 0;
    }
    node.next = NULL;
    if(prev != NULL) {
        node.prev = prev;
        prev->next = begin;
    } else{
        node.prev = NULL;
    }
    memcpy(begin,&node,getSizeOfNode());
    return begin;
}