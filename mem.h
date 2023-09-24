#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>

#define MEM_HEAP_SIZE 1048576

extern void mem_init();
extern void* mem_alloc(size_t mem_size);
extern void mem_free(void* ptr);

#endif /* _MEM_H */