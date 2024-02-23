#include "tim.h"
#include "debuglog.h"
#include "main.h"
#undef ABS(x)
#define ABS(x) x>0?x:-x
#define ABZ_STEP    0.18f//

#define ENCODER_LINES        (_ENCODERLINS)
#define ENCODER_STEP         (0.0003835f)       //2pi/4096
/*6-4-5-1-3-2*/
static float sg_hall_section[7] = {0.0f,210.0f,330.0f,270.0f,90.0f,150.0f,30.0f};
typedef struct _hall
{
    unsigned char cur_section;
    float ref_hall_theta;
}hall_t;

typedef struct _abz
{
    unsigned short pre_cnt;
    float ref_hall_theta;
}abz_t;

typedef struct _encoder
{
    float cur_theta;
    float pre_theta;
}encoder_t;

static encoder_t encoder_01;
static hall_t hall_01;
static abz_t abz_01;


void hall_encoder_process(void)
{
    /*更新hall区域*/
}
char hall_section_update(void)
{   
    unsigned char cur_section = 0;
    static unsigned char pre_section;
    cur_section = read_hall_section();
    if (cur_section == pre_section)
    {
        return 0;
    }
    hall_01.cur_section = cur_section;
    hall_01.ref_hall_theta = sg_hall_section[cur_section];
    return 1;//没有更新HALL值
}

void abz_couterupdate(void)
{

}

void encoder_angleupdate(void)
{
    if (!hall_section_update())
    {
        // encoder_01.pre_theta = hall_01.ref_hall_theta;
    }
    
    unsigned short cur_cnt;
    /*获取当前角度*/
    short diff_cnt;
    cur_cnt = read_abzcnt();
    diff_cnt = cur_cnt - abz_01.pre_cnt;
    // ABS(diff_cnt);
    /*更新当前角度*/
    encoder_01.cur_theta = encoder_01.pre_theta + diff_cnt * ABZ_STEP;
    encoder_01.pre_theta = encoder_01.cur_theta;
}


typedef struct hallencoder_data
{
    int32_t *raw_buf;     //原始数据
    int32_t *covdata_buf;       //转换后的数据    
    int32_t *filterdata_buf;    //滤波后数据
    int16_t buf_column;
}hc_data_t;

hc_data_t sg_hcdata = {0};
static int32_t sg_covangle; 

void hallencoder_init(void)
{
    gpio_setencoder_power();
    tim_encode_start();
    tim_encode_writecnt(500);
    sg_hcdata.covdata_buf = &sg_covangle;
}
float test_curtheta = 0.0f;
unsigned int test_cur_cnt = 0;
void* hallencoder_readangle(void)
{
    static unsigned int pre_cnt = 0;
    unsigned int cur_cnt = 0;
    int diff_cnt = 0;
    float cur_pose = 0.0f;
    static float pre_pose = 0.0f;    
    cur_cnt = tim_encode_readcnt();
    if (cur_cnt<5 || cur_cnt>16379)
    {
        pre_pose = 0.0f;
        pre_cnt = 0;
        cur_cnt = 0;
    }
    test_cur_cnt = cur_cnt;
    diff_cnt = cur_cnt - pre_cnt;
    ABS(diff_cnt);
    pre_cnt = cur_cnt;

    cur_pose = pre_pose + diff_cnt * ENCODER_STEP;
    pre_pose = cur_pose;
    test_curtheta = cur_pose;
    sg_covangle = (int32_t)(cur_pose * (1<<15));
    return (void *)(&sg_hcdata);
}
