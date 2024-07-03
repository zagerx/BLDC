#include "initmodule.h"
#include "stdint.h"


extern uint32_t  __initcall_start;
extern uint32_t  __initcall_end;


void do_initcalls(void)
{

   initcall_t fc;
   uint32_t *fn_entry;
   // do loop call call initcall functions;
   for( fn_entry =   &__initcall_start  ; fn_entry <  &__initcall_end ; fn_entry++)
   {
	fc = (initcall_t *) (*fn_entry);
	fc();
   }
}




