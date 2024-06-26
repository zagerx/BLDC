#ifndef __INITMODULE__H
#define __INITMODULE__H

typedef void (*initcall_t)(void);

#define __initcall_name(fn, level)      __initcall_##fn##level
#define INIT_CALL_SECTION(level) 	    __attribute__((__used__,__section__(".initcall." #level ".init")))

#define __define_initcall(fn,level) \
	static initcall_t INIT_CALL_SECTION(level) __initcall_name(fn,level) = fn;

#define  low_level_init(fn)   __define_initcall(fn,0)
#define  arch_init(fn)        __define_initcall(fn,1)
#define  dev_init(fn)         __define_initcall(fn,2)
#define  board_init(fn)       __define_initcall(fn,3)
#define  os_init(fn)          __define_initcall(fn,4)
#define  app_init(fn)         __define_initcall(fn,5)

#endif
