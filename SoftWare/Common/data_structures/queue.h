#ifndef QUEUE__H
#define QUEUE__h

#include <stdbool.h>
#include <stdlib.h>

// 定义队列节点
typedef struct queue_node {
    // 数据
    void* data;
    // 指向下一个节点的指针
    struct queue_node* next;
} queue_node_t;

// 定义队列结构体
typedef struct {
    // 队列头指针
    queue_node_t* front;
    // 队列尾指针
    queue_node_t* rear;
    // 队列大小
    size_t size;
} queue_t;

// 创建队列
#define CREATE_QUEUE_WITH_TYPE(type) \
    typedef struct { \
        queue_t queue; \
    } type##_queue_t; \
\
    bool create_##type##_queue(type##_queue_t* queue) { \
        if (queue == NULL) return false; \
        queue->queue.front = NULL; \
        queue->queue.rear = NULL; \
        queue->queue.size = 0; \
        return true; \
    } \

// 销毁队列
#define DELETE_QUEUE_WITH_TYPE(type) \
    void delete_##type##_queue(type##_queue_t* queue) { \
        if (queue == NULL) return; \
        while (queue->queue.front != NULL) { \
            queue_node_t* temp = queue->queue.front; \
            queue->queue.front = queue->queue.front->next; \
            free(temp); \
        } \
        queue->queue.rear = NULL; \
        queue->queue.size = 0; \
    } \

// 入队操作
#define ENQUEUE_WITH_TYPE(type) \
    bool enqueue_##type(type##_queue_t* queue, void* data) { \
        if (queue == NULL || data == NULL) return false; \
        queue_node_t* new_node = (queue_node_t*)malloc(sizeof(queue_node_t)); \
        if (new_node == NULL) return false; \
        new_node->data = data; \
        new_node->next = NULL; \
        if (queue->queue.front == NULL) { \
            queue->queue.front = new_node; \
        } else { \
            queue->queue.rear->next = new_node; \
        } \
        queue->queue.rear = new_node; \
        queue->queue.size++; \
        return true; \
    } \

// 出队操作
#define DEQUEUE_WITH_TYPE(type) \
    bool dequeue_##type(type##_queue_t* queue, void** data) { \
        if (queue == NULL || data == NULL || queue->queue.front == NULL) return false; \
        *data = queue->queue.front->data; \
        queue_node_t* temp = queue->queue.front; \
        queue->queue.front = queue->queue.front->next; \
        free(temp); \
        if (queue->queue.front == NULL) { \
            queue->queue.rear = NULL; \
        } \
        queue->queue.size--; \
        return true; \
    } \

// 获取队列大小
#define GET_QUEUE_SIZE_WITH_TYPE(type) \
    size_t get_##type##_queue_size(type##_queue_t* queue) { \
        if (queue == NULL) return 0; \
        return queue->queue.size; \
    } \

// 检查队列是否为空
#define IS_QUEUE_EMPTY_WITH_TYPE(type) \
    bool is_##type##_queue_empty(type##_queue_t* queue) { \
        if (queue == NULL) return true; \
        return (queue->queue.front == NULL); \
    } \


#endif
