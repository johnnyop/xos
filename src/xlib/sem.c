
#include "sem.h"
#ifdef SEMPHORE
u8 data sem_count[NR_TASK];
void sem_down(semphore *sem)
{
	disable_irq_save();
	if (sem->count == 0) {
		sem_count[current]++;
		task_status[current] |= TASK_SEM_WAIT;
		sem->wait_task |= (1 << current);
		enable_irq_restore();
		schedule();
	} else {
		sem->count--;
		enable_irq_restore();
	}
}


u8 sem_try_down(semphore *sem)
{
	u8 ret = 1;
	disable_irq_save();
	if (sem->count == 0)
		goto sem_quit;
	sem->count--;
	ret = 0;
sem_quit:
	enable_irq_restore();
	return ret;
}
void sem_up(semphore *sem, unsigned char n)
{
	u8 id, s_id;
	if (n == 0)
		return;
	disable_irq_save();
	sem->count += n;
	if (sem->count < n)
		sem->count = -1;
	if (sem->wait_task != 0) {
		/**
		 * TL0 & 0X7, 为了不是每次up都是从0开始,所以用TL0来获得一个随机数,从这个随机数开机
		 * */
		id = (TLN) & 7;
		s_id = id;
		do {
			n = (1 << id);
			if (sem->wait_task & n) {
				sem->wait_task &= ~n;
				sem->count--;
				--sem_count[id];
				if (sem_count[id] == 0) {
					task_status[id] &= (~TASK_SEM_WAIT);
				}
				if (sem->count == 0)
					break;
			}
			id++;
			if (id >= MAX_TASK)
				id = 0;
		} while ((s_id != id) && (sem->wait_task != 0));
	}
	enable_irq_restore();
}

#endif //SEMPHORE