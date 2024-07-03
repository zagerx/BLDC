#ifndef __TASK_MODULE__H
#define __TASK_MODULE__H

typedef void (*taskcall_t)(void);

#define __taskcall_name(fn, level)      __taskcall_##fn##level
#define TASK_CALL_SECTION(level) 	    __attribute__((__used__,__section__(".taskcall." #level ".task")))

#define __define_taskcall(fn,level) \
	static taskcall_t TASK_CALL_SECTION(level) __taskcall_name(fn,level) = fn;

#define  low_level_task(fn)   __define_taskcall(fn,0)
#define  arch_task(fn)        __define_taskcall(fn,1)
#define  dev_task(fn)         __define_taskcall(fn,2)
#define  board_task(fn)       __define_taskcall(fn,3)
#define  os_task(fn)          __define_taskcall(fn,4)
#define  app_task(fn)         __define_taskcall(fn,5)

void do_taskcalls(void);

#endif
