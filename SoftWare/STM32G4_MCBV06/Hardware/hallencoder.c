// #include "tim.h"
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
#define ENCODER_STEP                  (0.00153f)       

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

// static float sg_hall_section[7] = {0.0f,3.6652f,5.7595f ,4.7123f,1.5707f,2.6179,0.166f};
static float sg_hall_section[7] = {0.0f,3.6652f,5.7595f ,4.7123f,1.5707f,2.6179,-0.32f};
static unsigned char read_hallsection(unsigned char u,unsigned char v,unsigned char w);

 short test_cnt =0 ;
float test_rawangle =0 ;
float test_refangle =0 ;
char test_dir = 0;


/*
* hall中断调用
*/
static unsigned char test_hallcnt = 0;
void hallsection_update(unsigned char u,unsigned char v,unsigned char w)
{
#if(HALLSECTION_UPDATE_MODE == HALLSECTION_UPDATE_TI_MODE)
    test_hallcnt++;
    /*返回扇区编号*/
    sg_cur_hallsection = read_hallsection(u,v,w);
    /*清零编码器*/
    // tim_encode_writecnt(4095);
    if (test_dir)
    {
        // tim_encode_writecnt(4395);
    }else{
        // tim_encode_writecnt(3980);
    }
    
#endif
}

void hallencoder_init(void)
{
    gpio_setencoder_power();
    // tim_encode_writecnt(4095);
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
    // test_cnt = tim_encode_readcnt();
    // test_refangle = sg_hall_section[sg_cur_hallsection];
    // test_rawangle = test_refangle + (float)test_cnt*ENCODER_STEP;
    // // test_rawangle = fmod(test_rawangle,6.28f);
    // // sg_rawangle = sg_hall_section[sg_cur_hallsection] + test_cnt*ENCODER_STEP;
    // // test_rawangle = sg_rawangle;
    // sg_covangle = (int32_t)(test_rawangle * (1<<22));

    // static short pre_cnt = 0;
    // // short cur_cnt = tim_encode_readcnt();
    // short delt_cnt = cur_cnt - pre_cnt;
    // pre_cnt = cur_cnt;test_cnt = cur_cnt;
    // static float  pre_theta = 0.0f;
    // float cur_theta = pre_theta + -1*delt_cnt*ENCODER_STEP;
    // pre_theta = cur_theta;
    // test_rawangle = cur_theta;
    // if (delt_cnt > 0)
    // {
    //     test_dir = 1;
    // }else{
    //     test_dir = 0;
    // }
    // sg_covangle = (int32_t)(test_rawangle * (1<<22));
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


/*====================================================================================*/
uint8_t hall_get_sectionnumb(void)
{
    uint8_t  u,v,w;

    //u = HAL_GPIO_ReadPin();
    //v = HAL_GPIO_ReadPin();
    //w = HAL_GPIO_ReadPin();
    return (u | (v<<1) | (w<<1));
}

/*6-4-5-1-3-2*/
uint8_t last_section = 0;
uint8_t hall_dir = 0xFF;
float hall_baseAngle = 0.0f;
float hall_baseBuff[7] = {0.0f,3.6652f,5.7595f ,4.7123f,1.5707f,2.6179,-0.32f};

/*
*   被HALL中断调用
*/
float hall_get_baseAngle(uint8_t section)
{
    switch (last_section)
    {
    case 6:
        if (section == 4)
        {
            hall_dir = 0;//方向为正
            hall_baseAngle = hall_baseBuff[section];
            last_section = 4;
        }else if (section == 2)
        {
            hall_baseAngle = hall_baseBuff[section];
            hall_dir = 1;
            last_section = 2;
        }else{
            //err
        }
        
        break;

    case 4:
        
        break;
    case 5:
        
        break;
    case 1:
        
        break;
    case 3:
        
        break;
    case 2:
        
        break;
    case 0:
        
        break;

    default:
        break;
    }
}


