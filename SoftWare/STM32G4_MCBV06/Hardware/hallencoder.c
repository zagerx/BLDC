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
static int32_t sg_rawangle = 0; 
/*6-4-5-1-3-2*/
static unsigned char sg_cur_hallsection = 0;
static float sg_hall_section[7] = {0.0f,210.0f,330.0f,270.0f,90.0f,150.0f,30.0f};
static unsigned char read_hallsection(unsigned char u,unsigned char v,unsigned char w);



/*
* hall中断调用
*/
void hallsection_update(unsigned char u,unsigned char v,unsigned char w)
{
#if(HALLSECTION_UPDATE_MODE == HALLSECTION_UPDATE_TI_MODE)
    /*返回扇区编号*/
    sg_cur_hallsection = read_hallsection(u,v,w);
    /*清零编码器*/
    tim_encode_writecnt(0);
#endif
}

void hallencoder_init(void)
{
    gpio_setencoder_power();
    tim_encode_start();
    tim_encode_writecnt(0);
    sg_hcdata.covdata_buf = &sg_covangle;
    sg_hcdata.raw_buf = &sg_rawangle;
}

void* hallencoder_readangle(void)
{
#if 0 //只使用编码   
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
#else //编码器+hall
    sg_rawangle = sg_hall_section[sg_cur_hallsection] + tim_encode_readcnt()*ENCODER_STEP;
    sg_covangle = (int32_t)(sg_rawangle * (1<<22));
#endif

}

static unsigned char read_hallsection(unsigned char u,unsigned char v,unsigned char w)
{
    unsigned char section = 0;
    section = u | (v<<1) | (w<<2);
    return section;
}

