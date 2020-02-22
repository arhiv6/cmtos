#include <stdint.h>
#include <assert.h>

#include "mpool.h"

/*
    Memory pool

    Реализация пула памяти на связанных списках. Идея взята из статьи
    "Alternatives to malloc and new" http://www.spuify.co.uk/?p=454

    Занимает вего один указатель - на голову этого списка. Всё остальное
    (ссылки на следующие элементы) хранится в пустых сегментах пула.
*/

void *mp_alloc(mpool_t *pool)
{
    if (!pool)
    {
        return NULL;
    }

    if (!pool->head)
    {
        return NULL;    // Места в пуле нет
    }

    void *currPtr = pool->head;  // Указатель на голову отдадим
    pool->head = (*((void **)(pool->head)));
    return currPtr;
}

void mp_free(mpool_t *pool, void *ptr)
{
    #ifdef SAFE // TODO
    кратно chunk_size
    в границах mem
#endif

    if (!pool || !ptr)
    {
        return;
    }

    *((void **)ptr) = pool->head;
    pool->head = (void *)ptr;
}

void mp_init(mpool_t *pool, size_t size, size_t chunk_size, void *mem)
{
    assert(chunk_size >= sizeof(void *));

    if (!pool || !mem || !size || !chunk_size)
    {
        return;
    }

    const size_t chunk_count = (size / chunk_size) - 1;

    for (size_t chunk_index = 0; chunk_index < chunk_count; ++chunk_index)
    {
        uint8_t *currChunk = (uint8_t *)mem + (chunk_index * chunk_size);
        *((uint8_t **)currChunk) = currChunk + chunk_size;
    }

    *((void **)&((uint8_t *)mem)[chunk_count * chunk_size]) = NULL; /* terminating NULL */
    pool->head = mem;
}
