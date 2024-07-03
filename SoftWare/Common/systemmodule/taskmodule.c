#include "taskmodule.h"
#include "stdint.h"
extern uint32_t  __taskcall_start;
extern uint32_t  __taskcall_end;


void do_taskcalls(void)
{

   taskcall_t fc;
   uint32_t *fn_entry;
   // do loop call call taskcall functions;
   for( fn_entry =   &__taskcall_start  ; fn_entry <  &__taskcall_end ; fn_entry++)
   {
	fc = (taskcall_t *) (*fn_entry);
	fc();
   }
}


// #include "debuglog.h"

// void test_task1(void)
// {
//    USER_DEBUG_NORMAL("test task 01\n");
// }
// void test_task2(void)
// {
//    USER_DEBUG_NORMAL("test task 02\n");
// }
// void test_task3(void)
// {
//    USER_DEBUG_NORMAL("test task 03\n");
// }

// low_level_task(test_task1)
// low_level_task(test_task2)
// low_level_task(test_task3)
