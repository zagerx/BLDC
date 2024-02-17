# SVPWM
通过之前的梳理，已经从宏观上解释了SVPWM的作用以及含义。
# SVPWM的推导
## 已知条件如下：
- 基矢量，$\frac{2}{3}U_{DC}$
- 周期，$T_s$
- 电源电压,$U_{DC}$
## 最大矢量圆的半径
- $\frac{\sqrt3}{3}U_{DC}$
具体的推导过程如下：
根据伏秒平衡原理有:
- $V_{ref}T_s = V_4T_4+V_6T_6+V_0T_0$
- $T_s = T_4+T_6+T_0$
- $V_0$是零矢量，其大小对$V_{ref}$没有任何影响，也就是说要是$V_{ref}$最大，$T_0$应该等于0。
将$V_{ref}$分解到alpha，beta轴：
- $T_sV_{ref}*cos\theta = V_4T_4+V_6cos(60-\theta)T_6$
- $T_sV_{ref}*sin\theta = V_6sin(60 - \theta)T_6$

$T_6$
通过EXCEL表格也可观察出，若$V_{ref}>\frac{2}{3}U_{DC}$,则$T_0,T_1,T_2$有负值出现。
### 主副矢量的大小

### 换相矢量点的大小

excel表格

simulink模型

