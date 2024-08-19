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
      fc = (taskcall_t) (*fn_entry);
      fc();
   }
}
