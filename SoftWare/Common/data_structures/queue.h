#ifndef QUEUE_H
#define QUEUE_H

#define CREATE_QUEUE_WITH_TYPE(name, item) \
typedef struct { \
    unsigned char head; \
    unsigned char tail; \
    unsigned char size; \
    unsigned char remain; \
    item *array; \
} name##_queue_t; \
\
void name##queue_init(name##_queue_t *q, item *array, unsigned char size) { \
    q->array = array; \
    q->head = 0; \
    q->tail = 0; \
    q->remain = 0; \
    q->size = size; \
} \
\
int name##queue_write(name##_queue_t *q, item temp) { \
    if (q->remain == q->size) { \
        return 0; \
    } \
    q->array[q->tail] = temp; \
    q->tail = (q->tail + 1) % q->size; \
    q->remain++; \
    return 1; \
} \
\
item name##queue_read(name##_queue_t *q) { \
    item temp = q->array[q->head]; \
    q->head = (q->head + 1) % q->size; \
    q->remain--; \
    return temp; \
} \
\
unsigned char name##queue_remain(name##_queue_t *q) { \
    return q->remain; \
}

#endif /* QUEUE_H */
