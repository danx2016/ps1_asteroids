#include <malloc.h>
#include "mem.h"

void mem_init()
{
    // kindly provided by @spicyjpeg
    // using the linker symbol __heap_start, it's possible 
    // to get the maximum heap available for malloc
    extern uint8_t __heap_start[];
    InitHeap3((void*) __heap_start + 4, (void*) 0x801ffff8 - (void*) __heap_start);    
}

void* mem_alloc(size_t mem_size)
{
    malloc3(mem_size);
}

void mem_free(void* ptr)
{
    free3(ptr);
}
