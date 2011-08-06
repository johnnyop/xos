#ifndef SEM_H
#define SEM_H
#include "xos.h"
#ifdef SEMPHORE

/**
 * \brief 信号量结构体
 */
typedef struct {
/** count 信号量的值,当\a count为0,如果有任务想获取这个信号量,则该任务阻塞 */
	u8_t count;		// 计数器
/** var wait_task 标志有哪些任务在等待这个信号量,一位对应一个任务 */
	u8_t wait_task;	// 等待该信号量的任务，　XOS最大任务数为8,  
					// 任务0~7,1bit对应一个任务.
}semphore;

extern void sem_down(semphore *sem);
extern void sem_up(semphore *sem, u8_t n);
extern u8_t sem_try_down(semphore *sem);

#endif //SEMPHORE
#endif /* SEM_H*/
