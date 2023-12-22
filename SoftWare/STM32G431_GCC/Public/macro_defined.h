#ifndef __MACRO_DEFINE__H
#define __MACRO_DEFINE__H

/*------------------------------------���õĺ궨��------------------------------------------------*/



/*--------------------------------------�ֽڽ���-------------------------------------------------*/
#define __SWP16(A)   (( ((unsigned short)(A) & 0xff00) >> 8)    | \
                                        (( (unsigned short)(A) & 0x00ff) << 8))
#define __SWP32(A)   ((( (unsigned int)(A) & 0xff000000) >> 24) | \
                                        (( (unsigned int)(A) & 0x00ff0000) >> 8)   | \
                                        (( (unsigned int)(A) & 0x0000ff00) << 8)   | \
                                        (( (unsigned int)(A) & 0x000000ff) << 24))
#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))


/*----------------------------------���������Ԫ�ظ���---------------------------------------------*/
#ifndef dimof
#define   dimof(__array)     (sizeof(__array)/sizeof(__array[0]))
#endif

/*-----------------------------------------����λ����----------------------------------------------*/
#define USER_CLEAR_BIT(x, n) ( (x) &= ~(1 << (n)) )
#define USER_SET_BIT(x, n) ( (x) |= (1 << (n)) )
#define USER_GET_BIT(x, n) ( ((x)>>(n)) & 1 )


#endif
