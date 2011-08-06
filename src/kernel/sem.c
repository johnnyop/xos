/**
 * \file sem.c
 * \brief 信号量
 * 为多任务的同步提供工具
 * */
#include "sem.h"
#ifdef SEMPHORE
/**
 * number of semphore the task wait for.<br>
 * 任务正在等待的信号量的个,
 * 当任务等待的信号量为0时才可能唤醒 
 * */
u8_t data sem_count[NR_TASK];
/**
 * \fn void sem_down(semphore *sem)
 * \brief down \a sem
 * 获取\a sem信号量, 信号量不为0则,信号量减1,正常返回.<br>
 * 否则, sem_count[current]+1,
 * \param sem 要获取的信号量
 * \sa sem_try_down
 */
void sem_down(semphore *sem)
{
	disable_irq();
	if (sem->count == 0) {
		++sem_count[current];
		task_status[current] |= TASK_SEM_WAIT;
		/* 置等待这个信号量的任务的位 */
		sem->wait_task |= (1 << current);
		enable_irq();
		schedule();
	} else {
		sem->count--;
		enable_irq();
	}
}


/**
 * \brief 尝试获取信号量, 不阻塞任务
 * \param sem 要获取的信号量
 * \return 若信号量为0,则返回为-1,任务不阻塞. 否则返回0,且信号量减1
 * \sa sem_down
 */
u8_t sem_try_down(semphore *sem)
{
	u8_t ret = -1;
	disable_irq();
	if (sem->count == 0)
		goto sem_quit;
	sem->count--;
	ret = 0;
sem_quit:
	enable_irq();
	return ret;
}
/**
 * \brief 增加信号量\a sem的值\a n
 * 在增加的同时,处理正在等这个信号量的任务,sem_count减1,清信号量的等待位
 * \param sem 要获取的信号量
 * \param n 增加的数值 
 * \sa sem_count, semphore, task_status, TASK_SEM_WAIT
 */
void sem_up(semphore *sem, u8_t n)
{
	u8_t id, s_id;
	if (n == 0)
		return;
	disable_irq();
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
	enable_irq();
}

#endif //SEMPHORE
