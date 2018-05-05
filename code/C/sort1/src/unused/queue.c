#include "queue.h"
#include "sort.h"
#include <assert.h>
#include <bsd/bsd.h>
#include <errno.h>
#include <stdlib.h>


#define ITEM_SIZE 3

struct queue_t
init_queue(int capacity)
{
        char **str_array = die_malloc(capacity * sizeof(char *));
        for (int i = 0; i < capacity; ++i)
                str_array[i] = NULL;

        struct queue_t queue = {
                str_array,
                capacity,
                0, 0, 0
        };

        return queue;
}


void
destroy_queue(struct queue_t *queue)
{
        for (int i = 0; i < queue->capacity; ++i) {
                if (queue->items[i] != NULL) {
                        free(queue->items[i]);
                        queue->items[i] = NULL;
                }
        }
        free(queue->items);
}


void
enqueue(struct queue_t *queue, char *item)
{
        assert(queue->count < queue->capacity);
        char *cpy;

#ifdef ITEM_SIZE
        int size = ITEM_SIZE;
#else
        int size = strlen(item) + 1;
#endif
        cpy = die_malloc(size);
        strlcpy(cpy, item, size);

        queue->items[queue->tail] = cpy;
        queue->tail = modulo(queue->tail + 1, queue->capacity);
        ++queue->count;
}


void
dequeue(struct queue_t *queue, char *dest)
{
        assert(queue->count > 0);

#ifdef ITEM_SIZE
        int size = ITEM_SIZE;
#else
        int size = strlen(queue->items[queue->head]);
#endif
        strlcpy(dest, queue->items[queue->head], size);

        free(queue->items[queue->head]);
        queue->items[queue->head] = NULL;

        queue->head = modulo(queue->head + 1, queue->capacity);
        --queue->count;
}
