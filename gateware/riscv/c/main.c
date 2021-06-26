/*
 * main.c - top level of picorv32 firmware
 * 06-30-19 E. Brombaugh
 */

#include <stdio.h>
#include "up5k_osc.h"
#include "acia.h"
#include "printf.h"
#include "clkcnt.h"

/*
 * main... duh
 */
void main()
{
	uint32_t cnt;
	
	init_printf(0,acia_printf_putc);
	printf("\n\n\rup5k_osc - Diagnostics\n\r");
	
	cnt = 0;
	while(1)
	{
		/* ADC inputs */
		printf("%03X ", gp_in0 & 0xFFF);
		printf("%03X ", gp_in1 & 0xFFF);
		printf("%03X ", gp_in2 & 0xFFF);
		printf("%03X ", gp_in3 & 0xFFF);
		
		/* switch & sync inputs */
		printf("%01X ", (gp_in0>>12) & 0x3);
		printf("%01X ", (gp_in1>>12) & 0x3);
		printf("%01X ", (gp_in2>>12) & 0x3);
		printf("%01X ", (gp_in3>>12) & 0x3);
		
		gp_out = (gp_out&~(0xF<<17))|((cnt&0xF)<<17);
		printf("cnt = %d\n\r", cnt);
		cnt++;
		clkcnt_delayms(1000);
	}
}
