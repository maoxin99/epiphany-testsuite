#include <stdlib.h>
#include <e-lib.h>

#define delay (0x500000)
#define E_WAND_INT (0x8)

char outbuf[4096] SECTION("shared_dram");

void fibonacci();




int main(void) {
	int i, j, k;
	//i = 0; j = 1;
	int *p0, *p1;
	int fault;
	int row, col;
	int a,b;
	row = e_group_config.core_row;
	col = e_group_config.core_col;
	fault = 0;
	int result;
	


	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			if((i==row) && (j==col))
				continue;
			else
			{	


//	i = 0; j = 1;

	//wait for some time
/*	for(a=0;a<1000;a++)
	{
		b++;
	}
*/	//insert interrupt
	//e_wait(E_CTIMER_0, delay);
	e_wait(E_CTIMER_0, 0x5000000);
	e_irq_set(i,j,E_SW_EXCEPTION);

	e_wait(E_CTIMER_0, delay);
	e_irq_set(i,j,E_MEM_FAULT);

	e_wait(E_CTIMER_0, delay);
	e_irq_set(i,j,E_TIMER0_INT);

	e_wait(E_CTIMER_0, delay);
	e_irq_set(i,j,E_TIMER1_INT);

	e_wait(E_CTIMER_0, delay);
	e_irq_set(i,j,E_DMA0_INT);

	e_wait(E_CTIMER_0, delay);
	e_irq_set(i,j,E_DMA1_INT);

	e_wait(E_CTIMER_0, delay);
	e_irq_set(i,j,E_USER_INT);

	e_wait(E_CTIMER_0, delay);


	p0 = e_get_global_address(i,j,(void *)0x5200);
	for(k=0;k<7;k++)
	{

		if(p0[k] != 0x0000452f)
			
			fault = fault | (0x1<<k);
	}
	


			}
		}
	}

	p1 = (int *)0x5000;
	*p1 = fault;
	//*p1 = 2;
	
	return EXIT_SUCCESS;
}







