
#include "list.h"
#include "heap.h"
#include "debuglog.h"
#undef NULL
#define NULL 0

_list_t* list_creat(void)
{
    _list_t *phead;
    phead = heap_malloc(sizeof(_list_t));
    phead->head = 0;
    phead->tail = 0;
    phead->node_numb = 0;
    return phead;
}

node_item_t *list_creat_nodeitem(void *pdata,unsigned short len)
{
    node_item_t *pitem;
    pitem = heap_malloc(sizeof(node_item_t));
    pitem->pdata = pdata;
    pitem->len = len;
    return pitem;
}
_node_t *list_creatnode(void *pdata,unsigned short len)
{
    _node_t *pnode;
    node_item_t *pitem;
    pnode = heap_malloc(sizeof(_node_t));
    pitem = list_creat_nodeitem(pdata,len);
    pnode->next = 0;
    pnode->pitem = pitem;
    return pnode;
}

/*
**  头插法
*/
void list_insert_node(_list_t *pthis, _node_t *node)
{
    _node_t *tail_node;
    /*当前链表是否为空*/
    if (pthis->head == 0)
    {
        /* code */
        pthis->head = node;
        pthis->tail = node;
    }else{
        tail_node = pthis->tail;
        tail_node->next = node;
        pthis->tail = tail_node->next;//更新尾节点
    }
    pthis->node_numb++;
}


/*--------------根据节点ID/节点本身删除节点-----------------------------------*/
void list_delete_node(_list_t *pthis,_node_t *node)
{
    _node_t *cur_node,*pre_node = NULL;
    cur_node = pthis->head;
    pre_node = pthis->head;
    if (cur_node == NULL)
    {
        /* code */
        return;
    }
    if (node->pitem == NULL)
    {
        return;
    }

    while (cur_node != NULL)
    {
        if ((cur_node->id == node->id) || (cur_node == node))
        {
            /* code */
            pre_node->next = cur_node->next;

            if (pre_node == cur_node)//
            {
                /* code */
                pthis->head = cur_node->next;
            }
            if (cur_node->next == NULL)
            {
                /* code */
                pthis->tail = pre_node;
            }
            pthis->node_numb--;    
            break;
        }
        pre_node = cur_node;
        cur_node = cur_node->next;
    }
    heap_free(cur_node->pitem->pdata);
    heap_free(cur_node->pitem);
    heap_free(cur_node);
}
_node_t* list_read_node_vale(_list_t *pthis,_node_t *node)
{
    _node_t *p_curnode;
    if (pthis->node_numb == 0)
    {
        return (_node_t *)NULL;
    }
    p_curnode = pthis->head;

    while (p_curnode != NULL)
    {
        if (p_curnode->id == node->id)
        {
            /* code */
            break;
        }
        p_curnode = p_curnode->next;
    }
    return p_curnode;
}

_node_t* list_read_node(_list_t *pthis)
{
    _node_t *p_rnode;
    if (pthis->node_numb == 0)
    {
        return (_node_t *)NULL;
    }
    p_rnode = pthis->head;
    return (_node_t *)p_rnode;
}




void list_free(_list_t *pthis, _node_t *node)
{
    _node_t *cur_node,*next_node;
    cur_node = pthis->head;

    while (cur_node !=0 )
    {
        /*记录下一个节点*/
        next_node = cur_node->next;
        heap_free(cur_node);
        cur_node = next_node;
    }
    heap_free(pthis);
}


void _list_printf(_list_t *pthis)
{
    unsigned short len;
    unsigned char* pdata;    
    if (pthis->node_numb==0)
    {
        return;
    }
    _node_t *cur_node;
    cur_node = pthis->head;
    unsigned short id;
    
    while (cur_node != NULL)
    {
        id = cur_node->id;
        len = cur_node->pitem->len;
        pdata = cur_node->pitem->pdata;
        cur_node = cur_node->next;
    }
}

 