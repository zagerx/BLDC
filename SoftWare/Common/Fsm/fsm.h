#ifndef __FSM__H
#define __FSM__H


typedef enum {
    fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
    fsm_rt_cpl          = 0,     //!< fsm complete
    fsm_rt_on_going     = 1,     //!< fsm on-going
    fsm_rt_wait_for_obj = 2,     //!< fsm wait for object
    fsm_rt_asyn         = 3,     //!< fsm asynchronose mode, you can check it later.
} fsm_rt_t;

enum {
    ENTER=0,
    EXIT,      
    USER
};


typedef struct fsm_cb fsm_cb_t;
typedef fsm_rt_t *fsm_t(fsm_cb_t*);

struct fsm_cb{
    unsigned short cycle;
    unsigned char chState;
    unsigned int count;
    void* pdata;
    fsm_t *fsm;
};

#define DISPATCH_FSM(me_) ((fsm_t*)(me_)->fsm)((me_))
#define TRAN_STATE(me,targe) do{\
                            (me)->chState = EXIT;\
                            (me)->fsm(me);\
                            (me)->fsm = (fsm_t *)(targe);\
                            (me)->chState = ENTER;\
                            (me)->fsm(me);\
                                  }while(0)


void fsm_init(fsm_cb_t *pThis, unsigned short cycle,fsm_t *machine);
fsm_cb_t *fsm_creat(fsm_t *initstat,unsigned short cycel);
char fsm_destructor(fsm_cb_t *pthis);
#endif
