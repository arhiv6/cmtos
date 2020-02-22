#ifndef MPOOL_H
#define MPOOL_H

#include <stddef.h>

typedef struct
{
    void  *head;

#ifdef SAFE // TODO
    size_t size;
    size_t chunk_size;
    void *mem;
#endif
}
mpool_t;

void *mp_alloc(mpool_t *pool);
void mp_free(mpool_t *pool, void *ptr);
void mp_init(mpool_t *pool, size_t size, size_t chunk_size, void *mem);

#endif // MPOOL_H
