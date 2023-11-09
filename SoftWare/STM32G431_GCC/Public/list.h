#ifndef __LIST__H
#define __LIST__H

/*------------------�ڵ�������--------------------------*/
typedef struct items node_item_t;
struct items{
    unsigned short len;
    void *pdata;
};

/*--------------------�ڵ�-----------------------------*/
typedef struct _node _node_t;
struct _node
{
    _node_t *next;
    unsigned short id;//�ڵ�ID
    const char *name;//�ڵ�����
    node_item_t *pitem;//�ڵ�Я������������
};

/*--------------------����-----------------------------*/
typedef struct _list{
    _node_t *head;
    _node_t *tail;
    unsigned short node_numb;
}_list_t;

extern _list_t* list_creat(void);
extern node_item_t *list_creat_nodeitem(void *pdata,unsigned short len);
extern _node_t *list_creatnode(void *pdata,unsigned short len);
extern void list_insert_node(_list_t *pthis, _node_t *node);
extern void list_delete_node(_list_t *pthis,_node_t *node);
extern _node_t* list_read_node_vale(_list_t *pthis,_node_t *node);
extern _node_t* list_read_node(_list_t *pthis);
extern void list_free(_list_t *pthis, _node_t *node);
#endif
