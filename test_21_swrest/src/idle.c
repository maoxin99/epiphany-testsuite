#include <stdio.h>
#include <stdlib.h>
#include <e-lib.h>


#define destination (0x80802000)
#define source      (0x80902000)
#define offset      (0x00002000)
#define copy_size		(0x00004000)
//#define mailbox     ((unsigned *)0x00006f00)
#define mailbase    (0x8f000000)

int main(void) 
{


	unsigned time_s, time_e;
	unsigned *dest, *src;
	unsigned i;
	int row, col, num;
	unsigned *mail1, *mail2;
	
	
	row = e_group_config.core_row;
	col = e_group_config.core_col;
	num = row*4 + col;
	mail1 = (unsigned *)(mailbase + num*4);
	mail2 = (unsigned *)(mailbase + (num+16)*4);

	
	dest = e_get_global_address(row, col, (void *)offset);
	src  = e_get_global_address((row+1), (col+1), (void *)offset);
	*mail1 = 0x0;
	*mail2 = 0x0;

		e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX);
		time_s = e_ctimer_start(E_CTIMER_0, E_CTIMER_CLK);

	for (i=0;i<0x8000;i++)
	{

	
		e_dma_copy((void *)dest, (void *)src, copy_size);
		*mail1 = i+1;
	

	}
		
		time_e = e_ctimer_stop(E_CTIMER_0);
		
		*mail2 = 0xdeadbeef;

	
	return EXIT_SUCCESS;
}




