#ifndef KERNEL_H
#define KERNEL_H
extern unsigned char os_sp;	

#if (SLEEPED_FIRST != 0)
	extern unsigned char sleeped_task;
#endif /* SLEEPED_FIRST*/	    

extern void _pop_task();
extern u8 find_next();
extern void do_schedule();
extern void decreas_sleep_time();  
extern void pop_task(u8 id);

#endif /* KERNEL_H*/
