clear;
Pn = 2;
Rs = 0.2;
Lq = 0.8e-3/2;
Ld = 0.8e-3/2;
Ls = 0.8e-3/2;
Flux= 0.171;
Tm = 0.33;
Jm = 1.73e-5;%Kg.m^2
Bm = 1.349e-5;%摩擦系数
Ke = 6.6;%峰峰值

UDC = 24;
fpwm = 10000;%10KHz
Tpwm = 1/fpwm;


Kp = 0.45;
Ki = 450;
Speed_Kp = 0.00867;
Speed_Ki = 0.0633;
Speed_PI_OutputMax = 6;
Speed_PI_OutputMin = -6;


LBG_K1 = -4000;
LBG_K2 = 14000;
LBG_PLL_W = 50;
SpeedFilter_Fn = 0.3;
OPL_Torque = 3;
OPL_Time = 2;
OPL_Speed = 1200;
G1 = -4000;
G2 = 14000;
lbg_speedfilter = 0.3;
lbg_pll_w = 50;
