/**
 * \file xos_opt.h
 * \brief xos的系统配置, 移植时的设置.
 * <p>kernel.c和xos.c 要勾选Generate Assembler SRC File 和 Assemble SRC File
 * 右键文件->option for file '*.c'
 * 			->Generate Assembler SRC File
 * 			->Assemble SRC File
 * */

#ifndef XOS_OPT_H
#define XOS_OPT_H

#define STC12C5A
#define CPU_1T	1
#define CPU_12T	12
#define CPU_T	CPU_1T
//#define CPU_12T
/** MUC header file */
//#define CPU_12T
#ifdef STC12C5A
#define MCU_H <stc12c5a.h>
#else
#define MCU_H <reg52.h>
#endif


/**
 * 时间片,系统定时,时间点滴, 默认使用定时器0
 * */				 
/* 该系统运行的任务数,静态值. 最大为8, 不过因为系统性能,资源的问题,最好不超来6*/
#define NR_TASK 2	
/** xos系统定时中断频率, 1秒使用时间片的次数*/
#define HZ 250		
/** 一个时间片的时间(单位ms, 则系统定时器定时时长)*/
#define TICK_TIME 4		
/** 任务时间片, 不能大于15*/
#define TIME_SLICE 15		
// 10ms , 24MHz, 1
/** 晶振频率*/
#define FOSC (11.0592)	
#define VFOSC	(12)
#define FOSC12	(1)
/** 系统定时器,计数器装载值*/
#define THX 0xF1		
#define TLX 0xC0 							//DC 20
/** 系统定时器用的计数器寄存器*/
#define THN TH0
#define TLN TL0
#define TRN	TR0
#define ETN	ET0



/** ------------------------------------------------------------- */
/**若使用信号量则要定义这个宏*/
#define SEMPHORE	
/**
 * 设置任务调度时,是否首先运行休眠时间已到的任务.(sleep(n),n已到).
 * 如果SLEEPED_FIRST 不等于0, 则设置
 *
 * 设置后,当一个任务的一个时间片用完后,如果有休眠时间已到,但未运行的任务,
 * 则即使该任务还有时间片,还会让出CPU给休眠时间已到的的任务.
 * 该设置是为了休眠时间精度更高.
 * */
#define SLEEPED_FIRST 5

/**
 * \brief 任务堆栈
 * STACK_SZ = ((ALL_STACK_SZ) / NR_TASK)
 * 任务运行的栈的大小, 单片机留给任务的总栈大小 / 任务数
 * !!!要求
 * 	任务栈的大小最少要:　任务运行时可能嵌套层数*2 + 13(寄存器) +　2(内核)
 *  模拟栈大小(reentrant关键词): 可重入函数的使用大小.
 *  内核栈: 6. 则编译后data的使用大小, 到模拟栈开始地址或任务栈开始地址(没用模拟栈)之间
 * */							
/** 总栈起始地址. 默认为idata开始地址*/								 
#define STACK_START	(0x80)		
/** END + 1*/
#define STACK_END	(0x100)		
/** 任务PDATA的栈大小,自己手算结果,可以加快速度和减少代码体积*/
#define STACK_SZ	((STACK_END - STACK_START) / NR_TASK)	

/** ------------------------------------------------------------- */
/**
 * 如何定义了下面这个宏,可以到xos.c文件的customize_stack函数里设置自定义大小的任务栈.
 * 只需在customize_stack函数添加任务的栈起始地址则可.栈地址必须在0x00~0xff范围里.
 * */
//#define CUSTOMIZE_STACK
/** reentrant keyword */
#define XOS_REENTRANT
#define C_BP ?C_IBP
#define C_BP_SIZE	10
#define C_BP_END	0x80
#define C_BP_START	(C_BP_END - C_BP_SIZE * NR_TASK)
/**
 * 类似CUSTOMIZE_C_BP
 * */
//#define CUSTOMIZE_C_BP




/**
 * 下面是与内核有关的定义,,如果无修改内核,则无需更改
 * */

/** XOS最大任务数*/
#define MAX_TASK 8		
/** 任务调度保存的寄存器数*/
#define SAVE_REG 13		

/** sizeof(task_desc),任务描述符结构体大小,不再用结构体，这个宏也不用了 */
#define TASK_SZ 6	

/* 不再用 */
#define NREG  8
#define USEDREG 1







#endif /* XOS_OPT_H**/
