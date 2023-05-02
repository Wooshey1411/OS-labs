#include <stdio.h>

// 170(10) = 10101010(2)
#define PREAMBLE_VALUE 170

//#pragma pack(push,1)
struct HeapNode{
    unsigned char preamble[4];
    unsigned char isAllocated;
    size_t countOfBytes;
    struct HeapNode* next;
    struct HeapNode* prev;
};
//#pragma pack(pop)

size_t getSizeOfNode();

struct HeapNode* createNode(void* begin, struct HeapNode* prev, unsigned int isStart, size_t countOfBytes);