#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <e-lib.h>


char outbuf[4096] SECTION("shared_dram");


void swexception_isr(int signum);
void memfault_isr(int signum);
void timer0_isr(int signum);
void timer1_isr(int signum);
void dma0_isr(int signum);
void dma1_isr(int signum);
void user_isr(int signum);

int main(void) {
	int i;
	int *pt;

	e_irq_attach(E_SW_EXCEPTION, swexception_isr);
	e_irq_attach(E_MEM_FAULT, memfault_isr);
	e_irq_attach(E_TIMER0_INT, timer0_isr);

	e_irq_attach(E_TIMER1_INT, timer1_isr);
	e_irq_attach(E_DMA0_INT, dma0_isr);
	e_irq_attach(E_DMA1_INT, dma1_isr);
	
	e_irq_attach(E_USER_INT, user_isr);

	e_irq_global_mask(E_FALSE);
	e_reg_write(E_REG_IMASK,0x0);

	//reset the target mem
	
	pt = (int *)0x5200;
	for(i=0;i<7;i++)
	{
		pt[i] = 0x0;
	}
	
	while(1) ;

	return EXIT_SUCCESS;
}



void __attribute__((interrupt)) swexception_isr(int signum)
{
	int *p0;
	int a,b,c;
	int count;

	p0 = (int *)0x5200;
	*p0 = 0x0;
	a = 1; b = 1;
	for(count=0;count<20;count++)
	{
		c = a+b;
		a = b;
		b = c;
	}
	//b+=0x100000;
	*p0 = b;
	return;
}

void __attribute__((interrupt)) memfault_isr(int signum)
{
	int *p0;
	int a,b,c;
	int count;

	p0 = (int *)0x5204;
	*p0 = 0x0;
	a = 1; b = 1;
	for(count=0;count<20;count++)
	{
		c = a+b;
		a = b;
		b = c;
	}
	
	//b+=0x200000;
	*p0 = b;
	return;
}

void __attribute__((interrupt)) timer0_isr(int signum)
{
	int *p0;
	int a,b,c;
	int count;

	p0 = (int *)0x5208;
	*p0 = 0x0;
	a = 1; b = 1;
	for(count=0;count<20;count++)
	{
		c = a+b;
		a = b;
		b = c;
	}
	
	//b+=0x300000;
	*p0 = b;
	return;
}

void __attribute__((interrupt)) timer1_isr(int signum)
{
	int *p0;
	int a,b,c;
	int count;

	p0 = (int *)0x520c;
	*p0 = 0x0;
	a = 1; b = 1;
	for(count=0;count<20;count++)
	{
		c = a+b;
		a = b;
		b = c;
	}
	
	//b+=0x400000;
	*p0 = b;
	return;
}

void __attribute__((interrupt)) dma0_isr(int signum)
{
	int *p0;
	int a,b,c;
	int count;

	p0 = (int *)0x5210;
	*p0 = 0x0;
	a = 1; b = 1;
	for(count=0;count<20;count++)
	{
		c = a+b;
		a = b;
		b = c;
	}
	
	//b+=0x600000;
	*p0 = b;
	return;
}


void __attribute__((interrupt)) dma1_isr(int signum)
{
	int *p0;
	int a,b,c;
	int count;

	p0 = (int *)0x5214;
	*p0 = 0x0;
	a = 1; b = 1;
	for(count=0;count<20;count++)
	{
		c = a+b;
		a = b;
		b = c;
	}
	
	//b+=0x700000;
	*p0 = b;
	return;
}


void __attribute__((interrupt)) user_isr(int signum)
{
	int *p0;
	int a,b,c;
	int count;

	p0 = (int *)0x5218;
	*p0 = 0x0;
	a = 1; b = 1;
	for(count=0;count<20;count++)
	{
		c = a+b;
		a = b;
		b = c;
	}
	
	//b+=0x900000;
	*p0 = b;
	return;
}

