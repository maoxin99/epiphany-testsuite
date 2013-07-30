/*
  hello_world.c

  Copyright (C) 2012 Adapteva, Inc.
  Contributed by Yaniv Sapir <yaniv@adapteva.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program, see the file COPYING.  If not, see
  <http://www.gnu.org/licenses/>.
*/

// This is the HOST side of the Hello World example.
// The program initializes the Epiphany system,
// randomly draws an eCore and then loads and launches
// the device program on that eCore. It then reads the
// shared external memory buffer for the core's output
// message.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <e-hal.h>

#define _BufSize   (0x1000)
#define _BufOffset (0x01000000)
#define total_num (24)

char reg[total_num][20];
unsigned result_0[total_num], result_1[total_num], result_2[total_num];


void check_result();
void check_dma(int *f);
void init_reg_list();


int main(int argc, char *argv[])
{
	unsigned rows, cols, coreid, i, j;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;

	

	init_reg_list();

	
	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	// Allocate a buffer in shared external memory
	// for message passing from eCore to host.
	e_alloc(&emem, _BufOffset, _BufSize);

	//open the work group
	rows = platform.rows;
	cols = platform.cols;
	e_open(&dev, 0, 0, rows, cols);
	
	//load the device program on the board
	e_load_group("emain.srec", &dev, 0, 0, rows, cols, E_FALSE);

	//start the test
	for (i=0;i<rows*0+1;i++)
	{
		for (j=0;j<cols*0+1;j++)
		{
			coreid = (i + platform.row)*64 + j + platform.col;
			fprintf(stderr, "Message from eCore 0x%03x (%2d,%2d): \n", coreid, i, j);
			e_start(&dev, i, j);

			//wait for core to execute the program
			usleep(200000);
			
			
			e_read(&emem, 0, 0, 0x0, &result_0, sizeof(unsigned)*total_num);
			e_read(&emem, 0, 0, 0x200, &result_1, sizeof(unsigned)*total_num);
			e_read(&emem, 0, 0, 0x400, &result_2, sizeof(unsigned)*total_num);
			
			check_result();
		}
	}
	


	e_close(&dev);
	e_free(&emem);
	e_finalize();

	return 0;
}

void check_result()
{
	int index, fault;
	fault = 0;
	
	//test regular regs
	for (index=0;index<11;index++)
	{
		//if((result_0[index]==0x0) && (result_1[index]==0xffffffff) && (result_2[index]==0x0))
		//	;
		if((result_0[index]!=0x0) || (result_2[index]!=0x0))
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0x0!\n", reg[index]);
			fault++;
		}
		if(result_1[index]!=0xffffffff)
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0xffffffff! Current value is 0x%08x!\n", reg[index], result_1[index]);
			fault++;
		}
	}
	
	//check imask, ipend and ilat
	for (index=11;index<14;index++)
	{
		//if((result_0[index]==0x0) && (result_1[index]==0xffffffff) && (result_2[index]==0x0))
		//	;
		if((result_0[index]!=0x0) || (result_2[index]!=0x0))
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0x0!\n", reg[index]);
			fault++;
		}
		if(result_1[index]!=0x3ff)
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0x3ff! Current value is 0x%08x!\n", reg[index], result_1[index]);
			fault++;
		}
	}
	
	int *pt;
	check_dma(&fault);
	
	if(fault==0)
		fprintf(stderr,"test01: Reg Access passed!\n\n");
	else
		fprintf(stderr,"test01: Reg Access failed! Number of faults is %d!\n\n", fault);
	
	return;
}

void check_dma(int *fault)
{
	int index;
	
	//check	dma status, leave [3:0] untouched
	for (index=14;index<16;index++)
	{
		if(((result_0[index]&0xfffffff0)!=0x0) || ((result_2[index]&0xfffffff0)!=0x0))
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0x0!\n", reg[index]);
			(*fault)++;
		}
		if((result_1[index]|0xf)!=0xffffffff)
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0xfffffff*! Current value is 0x%08x!\n", reg[index], result_1[index]);
			(*fault)++;
		}
	}

	//check dma config, leave [3] and [0] untouched
	for (index=16;index<18;index++)
	{
		if(((result_0[index]&0xfffffff6)!=0x0) || ((result_2[index]&0xfffffff6)!=0x0))
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0x0!\n", reg[index]);
			(*fault)++;
		}
		if((result_1[index]|0x9)!=0xffffffff)
		{
			fprintf(stderr,"test01: Reg Access failed! Failed to set Reg %s to 0xfffffff*! Current value is 0x%08x!\n", reg[index], result_1[index]);
			(*fault)++;
		}
	}

	return;
}


void init_reg_list()
{
	//set up the reg list table
	strcpy(reg[0], "IRET");
	strcpy(reg[1], "DMA0STRIDE");	
	strcpy(reg[2], "DMA1STRIDE");
	strcpy(reg[3], "DMA0COUNT");	
	strcpy(reg[4], "DMA1COUNT");
	
	strcpy(reg[5], "DMA0SRCADDR");	
	strcpy(reg[6], "DMA1SRCADDR");
	strcpy(reg[7], "DMA0DSTADDR");
	strcpy(reg[8], "DMA1DSTADDR");
	strcpy(reg[9], "CTIMER0");
	
	strcpy(reg[10], "CTIMER1");
	strcpy(reg[11], "IMASK");	
	strcpy(reg[12], "IPEND");
	strcpy(reg[13], "ILAT");
	//reg_num = 13
	strcpy(reg[14], "DMA0STATUS");
	strcpy(reg[15], "DMA1STATUS");
	strcpy(reg[16], "DMA0CONFIG");
	strcpy(reg[17], "DMA1CONFIG");



	strcpy(reg[18], "DMA0AUTODMA0");
	strcpy(reg[19], "DMA1AUTODMA0");
	strcpy(reg[20], "DMA0AUTODMA1");
	strcpy(reg[21], "DMA1AUTODMA1");
	strcpy(reg[22], "STATUS");

	strcpy(reg[23],"E_REG_CONFIG");

	return;
}
