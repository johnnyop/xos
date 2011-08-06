github: git@github.com:johnnyop/xos.git
git remote add origin git@github.com:johnnyop/xos.git
git push -u origin master
Existing Git Repo?
  cd existing_git_repo
  git remote add origin git@github.com:johnnyop/xos.git
  git push -u origin master
/************************************************************/
PS:时间片循环任务调度
1.	可重入函数, 这里不一定是编译后有警告的函数才有重入问题(WARNING L15: MULTIPLE CALL TO SEGMENT), 
	由于KEIL C51对函数的参数处理不是用栈, 所以如果函数有引用全局变量(!太多变量或有指针,都会汇编出全局变量),
	就应该处理这种情况, 关中断或用reentrant关键词(对自己定义的全局变量无效)
	还可能用OVERLAY (* ! sfname)或全部函数NOOVERLAY
2.	C15汇编(函数重入问题), 在同一文件里不会互相调用的的函数, 某些变量可能会共用地址,
	 那就不能有两个任务"同时"调用这个函数,
	 因为如果一个任务调用了这个函数,突然发生任务调度,失去CPU, 
	 另一个任务在这时也调用这个函数的话,共用地址的变量会被正在运行的任务改变
	 数值, 所以要防止这种情况出现,可以用信号量,关中断(disable_irq),等
3.	关中断(或有中断优先级是高)时,要用is_kernel()确定是否内核正在运行时被打断,
	如果是, 则在关中断期间不能调用会进行任务调度,失去CPU的API,否则系统可能崩溃.

4.	数据覆盖.
	While this technique is mostly automatic, there are a few situations that require special attention.
		Indirect Function Calls (Function Pointers). 
		Tables or Arrays of Function Pointers. 
		Recursive Functions (functions that call themselves). 
		Reentrant Functions (functions invoked by more than one thread of execution — for example, a function called by main and an interrupt service routine. 
	
/************************************************************/


/**/
