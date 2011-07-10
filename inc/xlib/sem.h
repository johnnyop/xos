#ifndef SEM_H
#define SEM_H
#include "xos.h"
#ifdef SEMPHORE

typedef struct {
	u8 count;		// 计数器
	u8 wait_task;	// 等待该信号量的任务，　XOS最大任务数为8,  
					// 任务0~7,1bit对应一个任务.
}semphore;

extern void sem_down(semphore *sem);
extern void sem_up(semphore *sem, unsigned char n);
extern u8 sem_try_down(semphore *sem);

#endif //SEMPHORE
#endif /* SEM_H*/
