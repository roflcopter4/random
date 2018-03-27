#ifndef queue_h
#define queue_h

struct queue_t {
        char **items;
        int capacity;
        int count;
        int head;
        int tail;
};


struct queue_t init_queue(int capacity);
void destroy_queue(struct queue_t *queue);

void enqueue(struct queue_t *queue, char *item);
void dequeue(struct queue_t *queue, char *dest);

#endif /* queue.h */
