/*! 
 * \file kernel.c
 * \brief 内核处理, 任务调度, 时间片和睡眠处理,
 * @author xjf cexowginui@163.com
 */
#include "xos.h"
#include "kernel.h"

#ifdef XOS_REENTRANT

	/* 任务栈底指针 */
	u8_t data bp[NR_TASK];  
	#pragma ASM
		extrn data (?C_IBP) /*  */
	#pragma ENDASM

#endif 
/*
#pragma ASM

#pragma ENDASM		
*/
/**
 * 
 */
unsigned char xpcon;	/* 休眠时,PCON会复位,所以要保存 */
/**
 * 任务调试, 找出空闲且有时间片的任务运行,没有可运行的任务则<br>
 * 进入休眠状态,直到下一个时间片到来或其它中断.
 */
void do_schedule()
{
	//_u8 id;
TRY_AGAIN:
	//id = find_next();
	if (find_next() == NR_TASK) {
		SP = os_sp;
		EA = 1;
		xpcon = PCON;
		PCON |= 1;			//休眠　
		PCON = xpcon;
		goto TRY_AGAIN;
	}
	//quit_os(id);	

#ifdef XOS_REENTRANT	
#pragma ASM	
		// if (current != findnext()) //r7 = findnext();  {
		MOV  	A,current
		XRL  	A,r7
		JZ   	POPREGS

		// bp[current] = C_BP
		mov a, #bp
		add a, current
		mov r0, a
		mov @r0, C_BP 
		// C_BP = bp[r7] // 
		mov a, #bp
		add a, r7
		mov r0, a
		mov C_BP, @r0
		// }  
POPREGS:
#pragma ENDASM

#endif /* XOS_REENTRANT*/

	//current = find_next();
	
#pragma ASM		
	mov current, r7	
#pragma ENDASM 
	SP = stack_bottom[current];
#pragma ASM
		pop ar7;
		pop ar6;
		pop ar5;
		pop ar4;
		pop ar3;
		pop ar2;
		pop ar1;
		pop ar0;
#pragma ENDASM	 
	_pop_(PSW);    
	_pop_(DPL);
	_pop_(DPH);	  
	_pop_(B);	
	_pop_(ACC); 
	clr_os();
}




/**
 * 查找下一个将要执行的任务
 * @return下一个要执行的任务, 如果返回NR_TASK,则全部都休眠或其它不能运行的状态.
 */
u8_t find_next()
{
	u8_t id, tmp;
	if (id_timeslice[current] != 0)
		id_timeslice[current]--;
#if (SLEEPED_FIRST != 0)
	//查找休眠时间结束的任务, sleeped_task每一位代表一个任务，
	//任务休眠，则相应的sleeped_task位置位
	if (sleeped_task != 0) {
		for (id = 0; id < NR_TASK; id++) {
			tmp = (1 << id);
			if (sleeped_task & tmp) {
				sleeped_task &= tmp;
				if (task_status[id] == 0) {
					goto retback;	 
				}
			}
		}		
	}
#endif /* SLEEPED_FIRST*/
	id = current;
	while ((id_timeslice[id] & 0xf) == 0
		|| (task_status[id] != 0)) {	
		if (++id == NR_TASK) {
			id = 0;		
		}
		/* 所有任务的时间片都用完, 时间片+5, 重新开始*/
		if (id == current) {
			for (id = 0; id < NR_TASK; id++) {
				id_timeslice[id] &= 0xf0;
				id_timeslice[id] += TIME_SLICE;
			}				 
			id = 0;
			while (task_status[id] != 0) {
				id++;
				if (id == NR_TASK)
					goto retback;//return id;
			}
			break;
		}
	}
retback:
	return id;
}

/**
 * 任务休眠时间递减
 * */
static void decreas_sleep_time()
{
	u8_t i = 0;
	for (; i < NR_TASK; i++) {
		if (sleep_time[i] != 0) {
			sleep_time[i]--;
			if (sleep_time[i] == 0) {
				task_status[i] &= (~TASK_SLEEP);
#if (SLEEPED_FIRST != 0)
				sleeped_task |= (1 << i);
#endif // SLEEPED_FIRST
			}
		}
	}
}

/**
 * 系统定时器中断函数<br>
 * 用了定时器1, 处理任务事件,调度
 * @see do_schedule()
 * @see find_next()
 * */
void kernel_tick() interrupt 1
{	 
	THN = THX;
	TLN = TLX;
	if (!is_kernel())
		stack_bottom[current] = SP;
	SP = os_sp;
	set_os();
/* 清中断状态*/
#pragma ASM
	call irq_back
#pragma ENDASM
	decreas_sleep_time();
	do_schedule();

#pragma ASM
irq_back:
	reti
#pragma ENDASM	
}
/**/
