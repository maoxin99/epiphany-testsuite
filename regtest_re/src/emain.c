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
#include "e_lib.h"

#define membase   (0x8f000000)
#define signalbox (0x00005100)
#define mailbox   ((int *)0x00005200)

typedef enum{
	s_active   =  0,
	s_gid      =  1,
	s_az       =  4,
	s_an       =  5,
	s_ac       =  6,
	s_av       =  7,
	s_bz       =  8,
	s_bn       =  9,
	s_bv       = 10,
	s_avs      = 12,
	s_bis      = 13,
	s_bvs      = 14,
	s_bus      = 15,
	s_excause  = 16,
}status_bit;


void user_isr(int signum);

void test_active();
void test_gid();
void test_az();
void test_an();
void test_ac();
void test_av();
void test_bz();
void test_bn();
void test_bv();
void test_avs();
void test_bis();
void test_bvs();
void test_bus();
void test_excause();


int main(void) {
	int *pt;
	unsigned row, col;

	//initialize	
	e_irq_global_mask(E_FALSE);
	e_irq_mask(E_USER_INT, E_FALSE);
	row = e_group_config.core_row;
	col = e_group_config.core_col;
	pt = (int *)signalbox;
	
	while (1)
	{
		if (*pt == 0xdeadbeef)
			continue;
		else
			e_irq_set(row, col, E_USER_INT);
	}

	return EXIT_SUCCESS;
}


void __attribute__((interrupt)) user_isr(int signum)
{
	int *ps;
	ps = (int *)signalbox;
	
	switch (*ps) 
	{
		case s_active     :  test_active(); break;
		case s_gid        :     test_gid(); break;
		case s_az         :      test_az(); break;
		case s_an         :      test_an(); break;
		case s_ac         :      test_ac(); break;
		case s_av         :      test_av(); break;
		case s_bz         :      test_bz(); break;
		case s_bn         :      test_bn(); break;
		case s_bv         :      test_bv(); break;
		case s_avs        :     test_avs(); break;
		case s_bis        :     test_bis(); break;
		case s_bvs        :     test_bvs(); break;
		case s_bus        :     test_bus(); break;
		case s_excause    : test_excause(); break;
		default           :                 break;		
	}
	
	return;
}



void test_active()
{
	return;
}


void test_gid()
{
	unsigned current;
	current = e_reg_read(E_REG_STATUS);
	
	e_irq_global_mask(E_FALSE);
	mailbox[0] = 0x2 & e_reg_read(E_REG_STATUS);
	
	e_irq_global_mask(E_TRUE);
	mailbox[1] = 0x2 & e_reg_read(E_REG_STATUS);
	
	e_irq_global_mask(E_FALSE);
	mailbox[2] = 0x2 & e_reg_read(E_REG_STATUS);
	
	e_reg_write(E_REG_STATUS, current);
	
	return;
}


void test_az()
{
	return;
}


void test_an()
{
	return;
}


void test_ac()
{
	return;
}


void test_av()
{
	return;
}



void test_bz()
{
	return;
}



void test_bn()
{
	return;
}



void test_bv()
{
	return;
}



void test_avs()
{
	return;
}



void test_bis()
{
	return;
}



void test_bvs()
{
	return;
}



void test_bus()
{
	return;
}



void test_excause()
{
	return;
}




