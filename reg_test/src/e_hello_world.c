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

#include <e_regs.h>

//#define _RegAdd E_CTIMER0



#include "e_lib.h"

unsigned time = 0xffffffff;

char outbuf[4096] SECTION("shared_dram");

//initiate
//void check(unsigned int *argu, unsigned int time, unsigned int mask0, unsigned int mask1);
void check(e_core_reg_id_t argu, unsigned int time, unsigned int mask0, unsigned int mask1);
void check_ILAT(unsigned int num);
char reg[23][20];


int main(void) {


	//e_irq_state_t irqstate;
	//irqstate = e_gid();


//	unsigned int reg_list[] = {
	e_core_reg_id_t reg_list[] = {
				/*E_CONFIG,*/ E_REG_IRET, E_REG_IMASK, E_REG_IPEND, E_REG_DMA0STRIDE, E_REG_DMA1STRIDE, E_REG_DMA0COUNT,
				E_REG_DMA1COUNT, E_REG_DMA0SRCADDR, E_REG_DMA1SRCADDR, E_REG_DMA0DSTADDR, E_REG_DMA1DSTADDR,
				E_REG_DMA0AUTODMA0, E_REG_DMA1AUTODMA0, E_REG_DMA0AUTODMA1, E_REG_DMA1AUTODMA1, E_REG_CTIMER0,
				E_REG_CTIMER1};

	//set up the reg list table

	strcpy(reg[0], "IRET");
	strcpy(reg[1], "IMASK");	
	strcpy(reg[2], "IPEND");
	strcpy(reg[3], "DMA0STRIDE");	
	strcpy(reg[4], "DMA1STRIDE");
	strcpy(reg[5], "DMA0COUNT");	
	strcpy(reg[6], "DMA1COUNT");
	strcpy(reg[7], "DMA0SRCADDR");	
	strcpy(reg[8], "DMA1SRCADDR");
	strcpy(reg[9], "DMA0DSTADDR");
	strcpy(reg[10], "DMA1DSTADDR");
	strcpy(reg[11], "DMA0AUTODMA0");
	strcpy(reg[12], "DMA1AUTODMA0");
	strcpy(reg[13], "DMA0AUTODMA1");
	strcpy(reg[14], "DMA1AUTODMA1");
	strcpy(reg[15], "CTIMER0");
	strcpy(reg[16], "CTIMER1");
	strcpy(reg[17], "STATUS");
	strcpy(reg[18], "DMA0STATUS");
	strcpy(reg[19], "DMA1STATUS");
	strcpy(reg[20], "DMA0CONFIG");
	strcpy(reg[21], "DMA1CONFIG");
	strcpy(reg[22], "ILAT");

	unsigned int reg_num;
	reg_num = 17;
//	unsigned int *_regadd;
	e_core_reg_id_t _regadd;
	int k;

	//define the mask
	unsigned int mask0, mask1;

	//clear the buffer
	sprintf(outbuf, "\n");


	for (k = 0; k< reg_num; k++)
	{
//		_regadd = (unsigned *)reg_list[k];
		_regadd = reg_list[k];
		mask0 = 0x0;
		mask1 = 0xffffffff;
		//mask0 = 0x0041f000;
		//mask1 = 0xffbe0fff;
		//check the reg
		check(_regadd, k, mask0, mask1);
	}

	//special cases
	_regadd = E_REG_STATUS;
	mask0 = 0x7000f;
	mask1 = 0xfff8fff0;
	check(_regadd, reg_num, mask0, mask1);
	
	_regadd = E_REG_DMA0STATUS;
	mask0 = 0xf;
	mask1 = 0xfffffff0;
	check(_regadd, reg_num+1, mask0, mask1);

	_regadd = E_REG_DMA1STATUS;
	mask0 = 0xf;
	mask1 = 0xfffffff0;
	check(_regadd, reg_num+2, mask0, mask1);

	_regadd = E_REG_DMA0CONFIG;
	mask0 = 0x9;
	mask1 = 0xfffffff6;
	check(_regadd, reg_num+3, mask0, mask1);

	_regadd = E_REG_DMA1CONFIG;
	mask0 = 0x9;
	mask1 = 0xfffffff6;
	check(_regadd, reg_num+4, mask0, mask1);	

	//check the ILAT
	check_ILAT(reg_num+5);

	return EXIT_SUCCESS;
}


//void check(unsigned int *argu, unsigned int time, unsigned int mask0, unsigned int mask1){
void check(e_core_reg_id_t argu, unsigned int time, unsigned int mask0, unsigned int mask1){
	
	unsigned output = 0x0;
	unsigned set0 = 0;
	unsigned set1 = 0;
	unsigned int m0, m1, current, t;
	//unsigned int  _RegAdd;
	e_core_reg_id_t _RegAdd;
	t = time;
	_RegAdd = argu;
	m0 = mask0;
	m1 = mask1;

	//get core id
	e_coreid_t coreid;
	coreid = e_get_coreid();

	//save the current data
	current = e_reg_read(_RegAdd);

	//clear to 0
	e_reg_write(_RegAdd, current & m0);
	output = e_reg_read(_RegAdd);
	if(output != (current & m0)){
		set0 = 1;
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s TO 0 FAILED! The current value is 0x%08x! Expecting 0x%08x!\n", 			coreid, reg[t], output, current & m0);
	}

	//set to 1
	e_reg_write(_RegAdd, current | m1);
	output = e_reg_read(_RegAdd);
	if(output != (current | m1)){
		set1 = 1;
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s TO 1 FAILED! The current value is 0x%08x! Expecting 0x%08x!\n", 			coreid, reg[t], output, current | m1);
	}

	//clear to 0
	e_reg_write(_RegAdd, current & m0);
	output = e_reg_read(_RegAdd);
	if(output != (current & m0)){
		set0 = 1;
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s TO 0 FAILED! The current value is 0x%08x! Expecting 0x%08x!\n", 			coreid, reg[t], output, current & m0);
	}

	if(set1 == 0 & set0 == 0)
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s HAS  PASSED!\n", coreid, reg[t]);

	//store the old data
	e_reg_write(_RegAdd, current);

}

void check_ILAT(unsigned int num){
	unsigned output, current, t;
	unsigned set0 = 0;
	unsigned set1 = 0;
	e_core_reg_id_t _RegAdd;
	t = num;
	_RegAdd = E_REG_ILAT;

	//get core id
	e_coreid_t coreid;
	coreid = e_get_coreid();

	//save the current data
	current = e_reg_read(_RegAdd);

	//clear to 0
	e_reg_write(E_REG_ILATCL, 0xffffffff);
	output = e_reg_read(_RegAdd);
	if(output != 0x0){
		set0 = 1;
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s TO 0 FAILED! The current value is 0x%08x! Expecting 0x%08x!\n", 			coreid, reg[t], output, 0x0);
	}

	//set to 1
	e_reg_write(E_REG_ILATST, 0xffffffff);
	output = e_reg_read(_RegAdd);
	if(output != 0xffffffff){
		set1 = 1;
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s TO 1 FAILED! The current value is 0x%08x! Expecting 0x%08x!\n", 			coreid, reg[t], output, 0xffffffff);
	}

	//clear to 0
	e_reg_write(E_REG_ILATCL, 0xffffffff);
	output = e_reg_read(_RegAdd);
	if(output != 0x0){
		set0 = 1;
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s TO 0 FAILED! The current value is 0x%08x! Expecting 0x%08x!\n", 			coreid, reg[t], output, 0x0);
	}

	if(set1 == 0 & set0 == 0)
		sprintf(outbuf + strlen(outbuf), "Core 0x%03x set Reg %-20s HAS  PASSED!\n", coreid, reg[t]);

	//store the old data
	e_reg_write(_RegAdd, current);
}

