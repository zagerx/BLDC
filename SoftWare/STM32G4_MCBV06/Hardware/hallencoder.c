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
// #define ENCODER_STEP                  (0.077)       
#define ENCODER_STEP                  (0.00153f/4.0f)       

#define RADE   0.0174532925f
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
// static float sg_hall_section[7] = {0.0f,210.0f * RADE,330.0f*RADE ,270.0f*RADE,90.0f*RADE,150.0f*RADE,30.0f*RADE};
// static float sg_hall_section[7] = {0.0f,3.6652f,5.7595f ,4.7123f,1.5707f,2.6179,0.5235f};
static float sg_hall_section[7] = {0.0f,3.6652f,5.7595f ,4.7123f,1.5707f,2.6179,0.12f};

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
unsigned short test_cnt =0 ;
float test_rawangle =0 ;
float test_refangle =0 ;
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
    test_cnt = tim_encode_readcnt();
    test_refangle = sg_hall_section[sg_cur_hallsection];
    test_rawangle = test_refangle + (float)test_cnt*ENCODER_STEP;
    // test_rawangle = fmod(test_rawangle,6.28f);
    // sg_rawangle = sg_hall_section[sg_cur_hallsection] + test_cnt*ENCODER_STEP;
    // test_rawangle = sg_rawangle;
    sg_covangle = (int32_t)(test_rawangle * (1<<22));
    return (void *)(&sg_hcdata);
#endif

}
unsigned char test_section = 0;
static unsigned char read_hallsection(unsigned char u,unsigned char v,unsigned char w)
{
    unsigned char section = 0;
    section = u | (v<<1) | (w<<2);
    test_section = section;
    return section;
}

