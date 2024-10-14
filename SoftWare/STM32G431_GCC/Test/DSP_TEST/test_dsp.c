#include "test_dsp.h"
#include "debuglog.h"
#include "math.h"
#include "arm_math.h"
#include "perf_counter.h"
volatile float fvalue = 1.17f;
volatile double dvalue = 1.17;
void test_dsp(void)
{
      float test1,test2,test3;
    __cycleof__("cosf") {
        test1 = cosf(fvalue);
    } 
    __cycleof__("cosf") {
        test2 = cos(dvalue);
    }     
    __cycleof__("arm_cos_f32") {
        test3 = arm_cos_f32(fvalue);
    }   
    USER_DEBUG_NORMAL("test1 = %f,test2 = %f,test = %f\n",test1,test2,test3);  
}


