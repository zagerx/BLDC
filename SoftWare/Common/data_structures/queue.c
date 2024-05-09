
#if 0
typedef struct byte {
    unsigned char byte;
} byte_t;
#define byte_QUEUE_SIZE  20
byte_t byte_queue[byte_QUEUE_SIZE];



typedef struct byte_queue
{
    unsigned char head;
    unsigned char tail;
    unsigned char size;
    unsigned char remain;
    byte_t *arry;
}byte_queue_t;

void bytequeue_init(byte_queue_t *this, byte_t *byte_arry,unsigned char arry_size)
{
    this->arry = byte_arry;
    this->head = 0;
    this->tail = 0;
    this->remain = 0;
    this->size = arry_size;
}

void bytequeue_write(byte_queue_t *this,byte_t byte)
{

}

byte_t bytequeue_read(byte_queue_t *this)
{

}

unsigned char bytequeue_remain(byte_queue_t *this)
{
    return this->remain;
}



typedef struct msg {
    char *msg;
    unsigned char msg_size;
    void (*pfunc)(void);
} msg_t;
#define MSG_QUEUE_SIZE  5
msg_t msg_queue[MSG_QUEUE_SIZE];



typedef struct msg_queue
{
    unsigned char head;
    unsigned char tail;
    unsigned char size;
    unsigned char remain;
    msg_t *arry;
}msg_queue_t;

void msgqueue_init(msg_queue_t *this, msg_t *msg_arry,unsigned char arry_size)
{
    this->arry = msg_arry;
    this->head = 0;
    this->tail = 0;
    this->remain = 0;
    this->size = arry_size;
}

void msgqueue_write(msg_queue_t *this,msg_t msg)
{

}

msg_t msgqueue_read(msg_queue_t *this)
{

}

unsigned char msgqueue_remain(msg_queue_t *this)
{
    return this->remain;
}


#endif