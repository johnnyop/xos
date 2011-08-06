#include "xos.h"
#include "stcad.h"


void ad_init(u8_t ch)
{
	ad_setio(ch);
	ADC_RES = 0;
	ad_set_hres();
	ADC_CONTR = AD_POWER | AD_SPEED(AD_SPEED_180);
}

u8_t ad_poll_get(u8_t ch)
{
	ADC_RES = 0;
	AD_START_CONVERT(AD_SPEED_360, ch);
	ad_poll_end();
	return ADC_RES;
}
