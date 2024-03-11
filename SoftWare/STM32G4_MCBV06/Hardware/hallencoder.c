#include "tim.h"
#include "debuglog.h"
#include "main.h"

#define HALLSECTION_UPDATE_TI_MODE    (0)
#define HALLSECTION_UPDATE_POLL_MODE  (1)
#define HALLSECTION_UPDATE_MODE       (HALLSECTION_UPDATE_TI_MODE)

#undef  ABS(x)
#define ABS(x)                        (x>0?x:-x)
#define ABZ_STEP                      (0.18f)

#define ENCODER_LINES                 (_ENCODERLINS)
#define ENCODER_STEP                  (0.001533980f)       //2pi/4096/4

typedef struct hallencoder_data
{
    int32_t *raw_buf;     
    int32_t *covdata_buf;           
    int32_t *filterdata_buf;
    int16_t buf_column;
}hc_data_t;

hc_data_t sg_hcdata = {0};
static int32_t sg_covangle = 0; 

/*6-4-5-1-3-2*/
static float sg_hall_section[7] = {0.0f,210.0f,330.0f,270.0f,90.0f,150.0f,30.0f};

static unsigned char read_hallsection(unsigned char u,unsigned char v,unsigned char w)
{
    unsigned char section = 0;
    section = u | (v<<1) | (w<<2);
    return section;
}

char hallsection_update(unsigned char u,unsigned char v,unsigned char w)
{
#if(HALLSECTION_UPDATE_MODE == HALLSECTION_UPDATE_TI_MODE)
    return (char)read_hallsection(u,v,w);
#endif
}



void hallencoder_init(void)
{
    gpio_setencoder_power();
    tim_encode_start();
    tim_encode_writecnt(500);
    sg_hcdata.covdata_buf = &sg_covangle;
}
void* hallencoder_readangle(void)
{
    static unsigned int pre_cnt = 0;
    unsigned int cur_cnt = 0;
    int diff_cnt = 0;
    float cur_pose = 0.0f;
    static float pre_pose = 0.0f;
    cur_cnt = tim_encode_readcnt();

    diff_cnt = cur_cnt - pre_cnt;
    ABS(diff_cnt);
    pre_cnt = cur_cnt;

    cur_pose = pre_pose + diff_cnt * ENCODER_STEP;
    pre_pose = cur_pose;
    sg_covangle = (int32_t)(cur_pose * (1<<22));
    return (void *)(&sg_hcdata);
}
