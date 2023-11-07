#include "_assert.h"
#include "printf_log.h"

#ifdef  ENABLE_ASSERT
    void user_assert_fail(uint8_t *file, uint32_t line)
    {
        USER_DEBUG("Wrong parameters value: file %s on line %d\r\n", file, line);
    }
#else

#endif

