#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <e-lib.h>


char outbuf[4096] SECTION("shared_dram");





int main(void) {
	//initialize
	unsigned time_s, time_e, time;
	unsigned expect;
	int *p0, *p1;
	unsigned ctimer;
	int i, j;
	int a;	
	int fault;
	int row, col;

	sprintf(outbuf,"\t");

	row = e_group_config.core_row;
	col = e_group_config.core_col;
	ctimer = (unsigned)e_get_global_address(row,col,(void *)0xf0438);
	fault = 0x0;
	//ctimer = 0x849f0438;
	p0 = (int *)0x5200;
	*p0 = 0x0;
	//*p0 = 0x10;
	//p1 = (int *)0x80a05200;

	for(i=0;i<4;i++)
	{	
		for(j=0;j<4;j++)
		{	

			p1 = e_get_global_address(i,j,(void *)0x5100);
			//p1 = e_get_global_address(0,0,(void *)0x5200);
			*p1 = 0x210;
			a = 0x0;
	
	
			e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX);
			e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);
			//record the ctimer
			__asm__ __volatile__("ldr %0, [%1]":"=r"(time_s):"r"(ctimer):);
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
				
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	
			__asm__ __volatile__("ldr %0, [%1]":"=r"(a):"r"(p1):);	

			__asm__ __volatile__("ldr %0, [%1]":"=r"(time_e):"r"(ctimer):);


			

			//time_e = e_ctimer_get(E_CTIMER_0);
			e_ctimer_stop(E_CTIMER_0);
			time = time_s - time_e;
			//if(time = (i+j)*)
			expect = ((abs(row - i) + abs(col - j)) * 3 + 17)*10 + 17;
			if(time != expect)
				*p0++;
			
			
			//sprintf(outbuf,"time = %d cycles!", fault);
		}
	}

/*
	if(fault == 0)
		sprintf(outbuf+strlen(outbuf),"test20 emesh_read_latency Passed!");
	else
		sprintf(outbuf+strlen(outbuf),"test20 emesh_read_latency failed!\t\t\tWarnning, failed!");
*/
	
	return EXIT_SUCCESS;
}




