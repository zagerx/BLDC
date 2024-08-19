#include "queue.h"
typedef struct byte {
    unsigned char byte;
} byte_t;
#define byte_QUEUE_SIZE  20
byte_t byte_queue[byte_QUEUE_SIZE];

CREATE_QUEUE_WITH_TYPE(byte,byte_t)
