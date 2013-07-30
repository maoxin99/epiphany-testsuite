/*
  e_hello_world.c

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

// This is the DEVICE side of the Hello World example.
// The host may load this program to any eCore. When
// launched, the program queries the CoreID and prints
// a message identifying itself to the shared external
// memory buffer.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "e_lib.h"

#define reg_num (13)
#define membase (0x8f000000)
#define total_num (24)


//initiate
void check(e_core_reg_id_t regname, int index, unsigned mask0, unsigned mask1);
void check_ILAT(int index);



//char reg[24][20];
unsigned *result_0;
unsigned *result_1;
unsigned *result_2;


int main(void) {

/*
	e_core_reg_id_t reg_list[] = {
				 E_REG_IRET, E_REG_IMASK, E_REG_IPEND, E_REG_DMA0STRIDE, E_REG_DMA1STRIDE, E_REG_DMA0COUNT,
				E_REG_DMA1COUNT, E_REG_DMA0SRCADDR, E_REG_DMA1SRCADDR, E_REG_DMA0DSTADDR, E_REG_DMA1DSTADDR,
				E_REG_DMA0AUTODMA0, E_REG_DMA1AUTODMA0, E_REG_DMA0AUTODMA1, E_REG_DMA1AUTODMA1, E_REG_CTIMER0,
				E_REG_CTIMER1};
*/			

	e_core_reg_id_t reg_list[] = 
	{
		E_REG_IRET, E_REG_DMA0STRIDE, E_REG_DMA1STRIDE, E_REG_DMA0COUNT, E_REG_DMA1COUNT, 
		E_REG_DMA0SRCADDR, E_REG_DMA1SRCADDR, E_REG_DMA0DSTADDR, E_REG_DMA1DSTADDR, E_REG_CTIMER0, 
		E_REG_CTIMER1, E_REG_IMASK, E_REG_IPEND
	};
	
	//e_core_reg_id_t dma_auto[] = {E_REG_DMA0AUTODMA0, E_REG_DMA1AUTODMA0, E_REG_DMA0AUTODMA1, E_REG_DMA1AUTODMA1};
	//E_REG_DMA0AUTODMA0, E_REG_DMA1AUTODMA0, E_REG_DMA0AUTODMA1, E_REG_DMA1AUTODMA1, 


	//set up the reg list table
/*
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
*/

	e_core_reg_id_t reg;
	unsigned mask0, mask1;
	int k;


	

	result_0 = (unsigned *) (membase + 0x0);
	result_1 = (unsigned *) (membase + 0x200);
	result_2 = (unsigned *) (membase + 0x400);
	

	
	//regular regs
	for (k = 0; k< reg_num; k++)
	{
		reg = reg_list[k];
		mask0 = 0x0;
		mask1 = 0xffffffff;
		check(reg, k, mask0, mask1);
	}

	//check the ILAT
	check_ILAT(reg_num+0);


	//special cases
	
	reg = E_REG_DMA0STATUS;
	mask0 = 0xf;
	mask1 = 0xfffffff0;
	check(reg, (reg_num+1), mask0, mask1);

	reg = E_REG_DMA1STATUS;
	mask0 = 0xf;
	mask1 = 0xfffffff0;
	check(reg, (reg_num+2), mask0, mask1);

	reg = E_REG_DMA0CONFIG;
	mask0 = 0x9;
	mask1 = 0xfffffff6;
	check(reg, (reg_num+3), mask0, mask1);

	reg = E_REG_DMA1CONFIG;
	mask0 = 0x9;
	mask1 = 0xfffffff6;
	check(reg, (reg_num+4), mask0, mask1);



	//result_1[0] = 0xdeadbeef;
	//unsigned *pt;
	//pt = (unsigned *) (0x8f000000);
	//*pt = &result_0[16];
	return EXIT_SUCCESS;
}


void check(e_core_reg_id_t regname,int index, unsigned mask0, unsigned mask1)
{
	unsigned current;

	//save the current data
	current = e_reg_read(regname);

	//clear the reg
	e_reg_write(regname, current & mask0);
	result_0[index] = e_reg_read(regname);

	//set to 1
	e_reg_write(regname, current | mask1);
	result_1[index] = e_reg_read(regname);

	//clear to 0
	e_reg_write(regname, current & mask0);
	result_2[index] = e_reg_read(regname);

	//store the old data
	e_reg_write(regname, current);
	return;
}



void check_ILAT(int index){
	unsigned current;
	e_core_reg_id_t regname;
	regname = E_REG_ILAT;

	//save the current data
	current = e_reg_read(regname);

	//clear to 0
	e_reg_write(E_REG_ILATCL, 0xffffffff);
	result_0[index] = e_reg_read(regname);

	//set to 1
	e_reg_write(E_REG_ILATST, 0xffffffff);
	result_1[index] = e_reg_read(regname);

	//clear to 0
	e_reg_write(E_REG_ILATCL, 0xffffffff);
	result_2[index] = e_reg_read(regname);

	e_reg_write(regname, current);
	return;
}

