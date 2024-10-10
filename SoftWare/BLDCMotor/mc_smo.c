/**
 * 滑模观测器
 */
#include "mc_smo.h"
#include "motorctrl_common.h"
#include "math.h"
#include "mc_utils.h"
#define Rs     0.14f
#define Lq     25.0f
#define CUR_Ts 0.0001f
#define RATED_VOL 12.0f
#define RATED_CUR 10.0f

static float limit_value(float val, float up, float low);


void smo_observer(smo_t *smo)
{
    /*	Sliding mode current observer	*/	
    smo->EstIalpha = smo->Fsmopos * smo->EstIalpha + smo->Gsmopos*(smo->Valpha - smo->Ealpha - smo->Zalpha);
    smo->EstIbeta  = smo->Fsmopos * smo->EstIbeta  + smo->Gsmopos*(smo->Vbeta  - smo->Ebeta -  smo->Zbeta);

    /*	Current errors	*/	
    smo->IalphaError = smo->EstIalpha - smo->Ialpha;
    smo->IbetaError = smo->EstIbeta - smo->Ibeta;

    /*  Sliding control calculator	*/																	
	smo->IalphaError = limit_value (smo->IalphaError, smo->E0, -smo->E0);
    smo->IbetaError  = limit_value (smo->IbetaError, smo->E0, -smo->E0);
    smo->Zalpha = smo->IalphaError * 2 * smo->Kslide;
    smo->Zbeta  = smo->IbetaError  * 2 * smo->Kslide;

    /*	Sliding control filter -> back EMF calculator	*/
    smo->Ealpha = smo->Ealpha + smo->Kslf * (smo->Zalpha - smo->Ealpha);
    smo->Ebeta = smo->Ebeta + smo->Kslf * (smo->Zbeta - smo->Ebeta);

    /*	Rotor angle calculator -> Theta = atan(-Ealpha,Ebeta)	*/	
    float temp = atan2f(-smo->Ealpha,  smo->Ebeta);
    smo->Theta = wrap_pm_pi(temp);
}

void smo_init(smo_t *smo)
{
    smo->Fsmopos = exp((Rs/Lq)*CUR_Ts );
    smo->Gsmopos = (RATED_VOL/RATED_CUR )*(1/Rs) *(1-smo->Fsmopos);
    smo->Kslf = 58.0f;//1600.0f/60.0f * 7.0f *6.28f * 5.0f * CUR_Ts;
    smo->Kslide = 0.02f;
    smo->E0 = 0.5f;

}

static float limit_value(float val, float up, float low)
{
    if (val > up)       return up;
    else if (val < low) return low;
    else                return val;

}


