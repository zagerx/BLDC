#ifndef __MC_FOCMATH__H 
#define __MC_FOCMATH__H
float _normalize_angle(float angle);
void _2r_2s(float* dq,float theta,float* alpbet);
void _2s_2r(float* alpbet,float theta,float *dq);
void _3s_2s(float* abc,float *alpbet);
void SVM(float alpha, float beta,float *duty);
void _limit_vector_circle(float *dq,float *limit_dq);
#endif