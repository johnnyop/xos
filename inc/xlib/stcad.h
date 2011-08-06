#ifndef STCAD_H
#define STCAD_H
//#include "stc12c5a.h"
#include "xos.h"

#include <intrins.h>

#define AD0	0
#define AD1	(1 << 1)
#define AD2	(1 << 2)
#define AD3	(1 << 3)
#define AD4	(1 << 4)
#define AD5	(1 << 5)
#define AD6	(1 << 6)
#define AD7	(1 << 7)
#define ad_setio(n)	P1ASF |= n
#define ad_cleario(n)	P1ASF &= ~(n)
#define ad_power_on()	ADC_CONTR |= (1 << 7)
#define ad_power_off()	ADC_CONTR &= ~(1 << 7)

#define AD_POWER	(1 << 7)
#define AD_SPEED(sp)	(sp << 5)
#define AD_START	(1 << 3)

#define AD_START_CONVERT(sp, ch)	\
	ADC_CONTR = AD_POWER | AD_START | AD_SPEED(sp) | ch

#define AD_SPEED_90		3
#define AD_SPEED_180	2
#define AD_SPEED_360	1
#define AD_SPEED_540	0
#define ad_set_speed(sp)	ADC_CONTR = (ADC_POWER & ~(3 <<5)) | (sp << 5)

#define ad_convert_not_end()	(ADC_CONTR & (1 << 4))
#define ad_clear_end()		ADC_CONTR &= (1 << 4)
#define __ad_poll_end()	\
	do {	\
		while (!ad_convert_not_end()) {	\
			_nop_();_nop_();_nop_();_nop_();_nop_();	\
		}	\
	} while (0)
#define ad_poll_end()	\
	do {	\
		__ad_poll_end();	\
		ad_clear_end();	\
	} while (0)

#define ad_start()	ADC_CONTR |= (1 << 3)

#define AD_CHANNEL0	0
#define AD_CHANNEL1	1
#define AD_CHANNEL2 2
#define AD_CHANNEL3	3
#define AD_CHANNEL4	4
#define AD_CHANNEL5	5
#define AD_CHANNEL6	6
#define AD_CHANNEL7	7

#define ad_set_channel(n)	ADC_POWER = (ADC_POWER & ~(7)) | (n & 7)
#define ad_set_hres()		AUXR1 &= ~(1 << 2)
#define ad_set_lres()		AUXR1 |= (1 << 2)


void ad_init(u8_t ch);
u8_t ad_poll_get(u8_t ch);

#endif /* STCAD_H */
