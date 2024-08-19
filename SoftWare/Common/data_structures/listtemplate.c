
// #include "linkedlist.h"
// #include "debuglog.h"
// #include "string.h"
// // 定义任务结构体
// typedef struct task_node {
//     int task_id;
//     char task_name[50];
//     void (*pfunc)(void);
//     struct task_node* next_node;
// } task_node_t;

// // 创建任务链表
// CREAT_LIST_WITH_TYPE(task, task_node_t)
// #define CREAT_LIST(list)                    create_##list(list)
// #define INSERT_LIST_TAIL(list, node)        insert_##list##_tail(list, node)
// #define IS_LIST_EMPTY(list)                 is_##list##_empty(list)
// #define DELETE_LIST(list)                   delete_##list(list)
// #define DELETE_LIST_NODE(list, node)        delete_##list##_node(list, node)
// #define GET_LIST_NODE(list)                 get_##list##_head(list)
// #define TRAVERSE_LIST(list)                 traverse_##list(list)
// void tasklist_process(void)
// {
//     // 创建任务链表
//     task_list_t* task_list = 0;
//     CREAT_LIST(task_list);
    
//     // 在链表中插入任务节点
//     task_node_t* task1 = (task_node_t*)malloc(sizeof(task_node_t));
//     task1->task_id = 1;
//     strcpy(task1->task_name, "Task 1");
//     INSERT_LIST_TAIL(task_list, task1);
    
//     // 检查任务链表是否为空
//     if (IS_LIST_EMPTY(task_list)) {
//         USER_DEBUG_NORMAL("Task list is empty.\n");
//     } else {
//         USER_DEBUG_NORMAL("Task list is not empty.\n");
//         task_node_t *cur_node;
//         do
//         {
//             cur_node = TRAVERSE_LIST(task_list);
//             cur_node->pfunc();
//         } while (cur_node);
        
//         task_node_t *cur_task_node = GET_LIST_NODE(task_list);
//     }
    
//     // 在链表中删除任务节点
//     DELETE_LIST_NODE(task_list, task1);
    
//     // 销毁任务链表
//     DELETE_LIST(task_list);
    
//     return;
// }
