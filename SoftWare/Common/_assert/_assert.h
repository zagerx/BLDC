#ifndef ___ASSERT__H
#define ___ASSERT__H
#include "stdint.h"
#ifdef  ENABLE_ASSERT
    #define user_assert_param(expr) ((expr) ? (void)0U : user_assert_fail((uint8_t *)__FILE__, __LINE__))
    void user_assert_fail(uint8_t *file, uint32_t line);
#else
    #define user_assert_param(expr) ((void)0U)
#endif /* ENABLE_ASSERT */


#endif
