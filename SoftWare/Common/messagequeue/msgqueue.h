#ifndef __MSG_QUEUE__H
#define __MSG_QUEUE__H
#include "linkedlist.h"
#include "stdbool.h"
#include "debuglog.h"
#include "string.h"
typedef struct 
{ 
    int msg_id;
    char *msg_name;
    void (*pfunc)(void);
}msg_t;

typedef struct msg_node {
    msg_t msg;
    struct msg_node* next_node;
} msg_node_t;

CREAT_LIST_WITH_TYPE(msg, msg_node_t)
#define CREAT_MSG_LIST()                    create_msg_list()  
#define INSERT_LIST_TAIL(list, node)        insert_##list##_tail(list, node)
#define IS_LIST_EMPTY(list)                 is_##list##_empty(list)
#define DELETE_LIST(list)                   delete_##list(list)
#define DELETE_LIST_NODE(list, node)        delete_##list##_node(list, node)
#define GET_LIST_NODE(list)                 get_##list##_head(list)
#define TRAVERSE_LIST(list)                 traverse_##list(list)

#endif
