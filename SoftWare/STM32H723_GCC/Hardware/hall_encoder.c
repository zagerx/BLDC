#include "hall_encoder.h"
#undef ABS(x)
#define ABS(x) x>0?x:-x
#define ABZ_STEP    0.18f
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




void* mt6816_read(void)
{

    return 0;
}