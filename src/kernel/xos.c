/**
 * \file xos.c
 * \brief xos API
 * */
#include "xos.h"
#include "kernel.h"
/**当前任务*/
u8_t data current = 0; 
/** 内核栈指针 */
u8_t data os_sp;
//u8_t data _irq;
/** 任务栈底 
 * \sa NR_TASK
 * */
u8_t data stack_bottom[NR_TASK];
/** 任务ID和时间片 */
u8_t data id_timeslice[NR_TASK];						   
/** 任务状态 
 * \sa TASK_SLEEP, TASK_OVER, TASK_SEM_WAIT*/
u8_t data task_status[NR_TASK];
/** 任务睡眠时间 */
u16_t data sleep_time[NR_TASK];

#if (SLEEPED_FIRST != 0)   
	//sleeped_task每一位代表一个任务，
	//任务休眠，则相应的sleeped_task位置位
	u8_t sleeped_task = 0;
#endif /* SLEEPED_FIRST*/	    

#ifdef XOS_REENTRANT  
#pragma ASM		 
	extrn data (?C_IBP) /** 虚拟栈 */
#pragma ENDASM
	extern u8_t data bp[NR_TASK]; 
#endif /* XOS_REENTRANT */
//[0]
#ifdef CUSTOMIZE_STACK
/**
 * \brief 当自定义任务栈时, \a customize_stack 被 \fn xos_init()调用.用来初始化任务栈
 */
static void customize_stack()
{
	stack_bottom[0] = 0x80 + SAVE_REG + 1;
	stack_bottom[1] = 0x90 + SAVE_REG + 1;
	stack_bottom[id] = STACK_START + SAVE_REG + 1;
	/* ... */
}
#endif /* CUSTOMIZE_STACK */ 
#ifdef CUSTOMIZE_C_BP
/**
 * 当自定义任务虚拟栈时(c51机制), /fn customize_c_bp 被 /fn xos_init()调用
 */
static void customize_c_bp()
{
	bp[0] = 0x80;
	bp[1] = 0x76;
	/* ... */
}
#endif	/* CUSTOMIZE_C_BP*/
/*!
 * 初始单片机寄存器,数据. 初始内核资源, 最后进入任务0
 * XOS_INIT:
 * mov OS_SP, sp * ; 保存当前栈位置,,以后重新进入内核都从这个栈开始
 * lcall _init
 * ;sp = 任务0栈,开始任务0
 * mov a, stack_bottom
 * clr c
 * subb a, #SAVE_REG
 * mov sp, a	
 * ret
 */
void xos_init()
{
	os_sp = SP;

	THN = THX;
	TLN = TLX;
	TMOD = (TMOD & (~0x0f)) | 0x01;
	IP = 0x02;
	EA = 1;
	TRN = 1;
	ETN = 1;
	current = 0;

#ifdef CUSTOMIZE_STACK
	customize_stack();
#endif


	SP = stack_bottom[0] - SAVE_REG;

#ifdef XOS_REENTRANT 
#ifdef CUSTOMIZE_C_BP
	customize_c_bp();
#endif
#pragma ASM
	mov C_BP, #C_BP_END
#pragma ENDASM
#endif 
}

/**
 * 把任务添加到XOS.
 * \param func 任务的函数地址
 * */
void add_task(unsigned int func) 
{	
	u8_t id = current;	 
	u8_t *p;
	
	current++;
	id_timeslice[id] = (id << 4) + TIME_SLICE;
	task_status[id] = 0x00;
#ifndef CUSTOMIZE_STACK
	stack_bottom[id] = STACK_SZ * id + STACK_START + SAVE_REG + 1;
#endif
	p = (u8_t *)(STACK_SZ * id + STACK_START);
	*p = (func & 0xff);
	p++;
	*p = (func >> 8); 
#ifdef XOS_REENTRANT 
	#ifndef CUSTOMIZE_C_BP
		bp[id] = C_BP_END - (C_BP_SIZE * id);
	#endif
#endif 		
	//return id;
	/**/
}

/**
 * 进行任务调度, 任务让出CPU, 让出一个任务运行. 任务时间片清0
 * */
void schedule()
{
#pragma ASM			
	push 	acc
	PUSH 	B	   	
	PUSH 	DPH	   	
	PUSH 	DPL	   	
	PUSH 	PSW	   	
	PUSH 	AR0	   	
	PUSH 	AR1	   	
	PUSH 	AR2	   	
	PUSH 	AR3	   	
	PUSH 	AR4	   	
	PUSH 	AR5	   	
	PUSH 	AR6		
	PUSH 	AR7			 
#pragma ENDASM	
	stack_bottom[current] = SP;
	SP = os_sp;
	set_os();
	id_timeslice[current] &= (~0x0f);
	do_schedule();
}

/**
 * 任务休眠
 * 任务主函数如果不是死循环工作,那么最后应该调用sleep(0),让任务不再工作.否则系统跑飞
 * \param n 休眠的时间, 单位为\a HZ,则系统时间片的时间. 如果为\a HZ(宏)则1秒,<br>
 * 		如果为0, 则任务不再工作.
 * \sa HZ
 * */
void sleep(unsigned int n) 
{	
	u8_t tmp;
#pragma ASM
	push 	acc
	PUSH 	B	   	
	PUSH 	DPH	   	
	PUSH 	DPL	   	
	PUSH 	PSW	   	
	PUSH 	AR0	   	
	PUSH 	AR1	   	
	PUSH 	AR2	   	
	PUSH 	AR3	   	
	PUSH 	AR4	   	
	PUSH 	AR5	   	
	PUSH 	AR6		
	PUSH 	AR7			 
#pragma ENDASM	
	stack_bottom[current] = SP;
	SP = os_sp;
	set_os();
	/* 时间片已用超过一半*/
	tmp = (0x100 - THN);
	if (tmp > ((0xff - THX) >> 1))
		++n;
	sleep_time[current] = n;
	tmp = 0;
	tmp |= TASK_SLEEP;
	if (tmp == 0) {
		tmp |= TASK_OVER;
	} 
	task_status[current] |= tmp;
	do_schedule();
}

