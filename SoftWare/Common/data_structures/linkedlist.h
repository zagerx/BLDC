#ifndef LINKEDLIST__h
#define LINKEDLIST__h

#include "stdbool.h"
#include "stdint.h"

#define CREAT_LIST_WITH_TYPE(type, node_type) \
    typedef struct type##_list { \
        node_type* head; \
        node_type* tail; \
        int16_t node_num; \
    } type##_list_t; \
    \
    bool create_##type##_list(type##_list_t* list) { \
        list = (type##_list_t*)malloc(sizeof(type##_list_t)); \
        if (list == NULL) { \
            return 1;\
        } \
        list->head = NULL; \
        list->tail = NULL; \
        list->node_num = 0; \
        return 0; \
    } \
    \
    void delete_##type##_list(type##_list_t* pthis) { \
        node_type *current = pthis->head; \
        while (current != NULL) { \
            node_type *temp = current; \
            current = current->next_node; \
            free(temp); \
        } \
        free(pthis); \
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
                free(current); \
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
    }
#endif