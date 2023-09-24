#include <malloc.h>
#include "mem.h"

static uint8_t heap[MEM_HEAP_SIZE];

void mem_init()
{
    InitHeap3((uint32_t*) heap, MEM_HEAP_SIZE);
}

void* mem_alloc(size_t mem_size)
{
    malloc3(mem_size);
}

void mem_free(void* ptr)
{
    free3(ptr);
}
