#ifndef LINKEDLIST__h
#define LINKEDLIST__h

#include "stdbool.h"
#include "stdint.h"
#include "heap.h"
#define CREAT_LIST_WITH_TYPE(type, node_type) \
    type##_list_t* create_##type##_list() {  \
        type##_list_t* list = (type##_list_t*)heap_malloc(sizeof(type##_list_t)); \
        if (list == NULL) {  \
            return NULL;\
        }  \
        list->head = NULL;  \
        list->tail = NULL;  \
        list->cur = NULL;  \
        list->node_num = 0;  \
        return list; \
    }\
    \
    void delete_##type##_list(type##_list_t* pthis) { \
        node_type *current = pthis->head; \
        while (current != NULL) { \
            node_type *temp = current; \
            current = current->next_node; \
            heap_free(temp); \
        } \
        heap_free(pthis); \
    } \
    \
    bool insert_##type##_list_tail(type##_list_t* pthis, node_type* node) { \
        if (pthis == NULL || node == NULL) { \
            return false; \
        } \
        \
        if (pthis->head == NULL) { \
            pthis->head = node; \
        } else { \
            pthis->tail->next_node = node; \
        } \
        \
        pthis->tail = node; \
        node->next_node = NULL; \
        pthis->node_num++; \
        return true; \
    } \
    \
    bool delete_##type##_list_node(type##_list_t* pthis, node_type* node) { \
        if (pthis == NULL || node == NULL) { \
            return false; \
        } \
        \
        node_type *current = pthis->head; \
        node_type *prev = NULL; \
        \
        while (current != NULL) { \
            if (current == node) { \
                if (prev == NULL) { \
                    pthis->head = current->next_node; \
                } else { \
                    prev->next_node = current->next_node; \
                } \
                \
                if (current == pthis->tail) { \
                    pthis->tail = prev; \
                } \
                \
                heap_free(current); \
                pthis->node_num--; \
                return true; \
            } \
            prev = current; \
            current = current->next_node; \
        } \
        \
        return false; \
    } \
    \
    bool is_##type##_list_empty(type##_list_t* pthis) { \
        return pthis == NULL || pthis->head == NULL; \
    } \
    \
node_type* get_##type##_list_head(type##_list_t* pthis) { \
    if (pthis == NULL || pthis->head == NULL) { \
        return NULL; \
    } \
    return pthis->head; \
    } \
    \
node_type* traverse_##type##_list(type##_list_t* pthis) { \
    if (pthis == NULL) {\
        pthis->cur = NULL;\
        return NULL;\
    }\
    if (pthis->cur == NULL) {\
        pthis->cur = pthis->head;\
    } else {\
        pthis->cur = pthis->cur->next_node;\
    }\
    return pthis->cur;\
}

#endif