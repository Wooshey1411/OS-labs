#include "heap.h"
#include "heapList.h"
#include "sys/mman.h"
#include <unistd.h>
#include <signal.h>
#include <stdint.h>

#define COUNT_OF_PAGES 100

static unsigned char isInited = 0;
static struct HeapNode* startNode;


struct HeapNode* init(size_t size){
    size_t maxSize = getpagesize()*100;
    if(size+2*getSizeOfNode() >= maxSize){
        fprintf(stderr, "not enough memory\n");
        return NULL;
    }


    void* memory = mmap(NULL, maxSize, PROT_READ|PROT_WRITE|!PROT_EXEC,MAP_ANONYMOUS|MAP_PRIVATE,-1,0);
    if(memory == (void*)0xFFFFFFFFFFFFFFFF){
        perror("mmap");
        return NULL;
    }
    if((startNode = createNode(memory,NULL,1,size)) == NULL){
        fprintf(stderr,"not enough memory\n");
        return NULL;
    }
    if(createNode((void*)((uintptr_t)memory+getSizeOfNode()+size),startNode,0,0) == NULL){
        fprintf(stderr,"not enough memory\n");
        return NULL;
    }

    return (void*)((uintptr_t)memory + getSizeOfNode());
}


void* myMalloc(size_t size){

    if(size == 0){
        return NULL;
    }

    if(!isInited){
        void* p = init(size);
        if(p == NULL){
            return NULL;
        }
        isInited = 1;
        return p;
    }

    struct HeapNode* node = startNode;
    while(node != NULL){
        void* posOfNextNode;
        if(node->next == NULL){
            posOfNextNode = (void*)((uintptr_t)startNode + COUNT_OF_PAGES*getSizeOfNode() - getSizeOfNode());
        } else{
            posOfNextNode = node->next;
        }
        if(!node->isAllocated && (uintptr_t)(posOfNextNode - (void*)node - getSizeOfNode()) >= size){

            if(createNode((void*)((uintptr_t)node+size+getSizeOfNode()),node,0,0) == NULL){
                fprintf(stderr,"not enough memory\n");
                return NULL;
            }
            node->isAllocated = 1;
            node->countOfBytes = size;
            node->next = (void*)((uintptr_t)node+size+getSizeOfNode());
            node->next->prev = node;
            return (void*)((uintptr_t)node + getSizeOfNode());
        }
        node = node->next;
    }
    fprintf(stderr,"not enough memory\n");
    return NULL;
}

void destroyHeap(){


    FILE* dump;
    if((dump = fopen("heap.dump","w")) == NULL){
        perror("fopen");
    } else{
        fwrite(startNode,COUNT_OF_PAGES*getpagesize(),1,dump);
        fclose(dump);
    }



    munmap(startNode,COUNT_OF_PAGES*getpagesize());
}

void errorDestroy(){
    fprintf(stderr,"Heap corrupted!\n");
    destroyHeap();
    raise(SIGSEGV);
}

void myFree(void* p){
    struct HeapNode* leftNode = p - getSizeOfNode();

    if(leftNode < startNode || p > (void*)((uintptr_t)startNode + COUNT_OF_PAGES*getSizeOfNode())){
        errorDestroy();
    }

    for (int i = 0; i < 4; ++i) {
        if(leftNode->preamble[i] != PREAMBLE_VALUE || leftNode->next->preamble[i] != PREAMBLE_VALUE){
            errorDestroy();
        }
    }
    if(leftNode->isAllocated == 0 || (void*)((uintptr_t)p + leftNode->countOfBytes) != leftNode->next){
        errorDestroy();
    }
    leftNode->isAllocated = 0;
    leftNode->countOfBytes = 0;
    while(leftNode->prev != NULL && leftNode->prev->isAllocated == 0){
        leftNode->prev->next = leftNode->next;
        leftNode = leftNode->prev;
    }

    while(leftNode->next != NULL){
        leftNode->next = leftNode->next->next;
        if(leftNode-> next != NULL && leftNode->next->isAllocated){
            break;
        }
    }

}
