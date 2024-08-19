
#ifndef __UTILS_LIB_H
#define __UTILS_LIB_H

// #include "stm32f4xx.h"
#include "math.h"


/****************************************************************************/
// #define M_PI  3.14159265358979323846f
#define one_by_sqrt3  0.57735026919f
#define two_by_sqrt3  1.15470053838f
#define sqrt3_by_2    0.86602540378f
/****************************************************************************/
#define min(a,b)  (a<b?a:b)
#define max(a,b)  (a>b?a:b)
#define clamp(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define has_value(a)  (a==NULL ? 0:1)
/****************************************************************************/
/****************************************************************************/
static inline float SQ(float x)
{
	return x * x;
}
// Wrap value to range.
// With default rounding mode (round to nearest),
// the result will be in range -y/2 to y/2
static inline float wrap_pm(float x, float y) {
#ifdef FPU_FPV4
	float intval = (float)round(x / y);  //round_int
#else
	float intval = nearbyintf(x / y);
#endif
	return x - intval * y;
}

// Same as fmodf but result is positive and y must be positive
static inline float fmodf_pos(float x, float y) {
	float res = wrap_pm(x, y);
	if (res < 0) res += y;
	return res;
}



// Modulo (as opposed to remainder), per https://stackoverflow.com/a/19288271
static inline int mod(const int dividend, const int divisor)
{
	int r = dividend % divisor;
	if (r < 0) r += divisor;
	return r;
}


static inline float wrap_pm_pi(float x) {
	return wrap_pm(x, 2 * M_PI);
}
/****************************************************************************/

/****************************************************************************/


#endif

