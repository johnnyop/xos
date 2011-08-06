	
//#define C_BP ?C_IBP
#include "xos.h"
void do_stuff(unsigned char u)	reentrant
{
	unsigned char i = 1, c = 2;
	c = i + c + u; 
	while (1) {
		switch (u) {
			case 0: P0 = c + i; break;
			case 1: P1 = c + i; break;
			case 2: P2 = c + i; break;
			case 3: P3 = c + i; break;
		}
		sleep(HZ);
		i++;
	}	  
	c = i + c + u; 
}