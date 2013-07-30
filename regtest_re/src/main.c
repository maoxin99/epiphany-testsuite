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

#define _BufSize       (0x1000)
#define _BufOffset     (0x01000000)
#define total_num      (24)

#define sigaddress     (0x00005100)
#define mailaddress    (0x00005200)


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


char reg[total_num][20];
unsigned result_0[total_num], result_1[total_num], result_2[total_num];


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



int main(int argc, char *argv[])
{
	unsigned rows, cols, coreid, i, j, k;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	
	unsigned signalbox, mailbox[10];

	

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
			//initialize
			signalbox = 0xdeadbeef;
			mailbox[0]   = 0xdeadbeef;
			e_write(&dev, i, j, sigaddress, &signalbox, sizeof(unsigned));
			e_write(&dev, i, j, mailaddress, &mailbox, 10*sizeof(unsigned));
			e_start(&dev, i, j);

			//wait for core to execute the program
			for (k=0;k<17;k++)
			{
				usleep(2000);
				switch (k) 
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
			}
			
			
//			e_write(&dev, i, j, sigaddress, &signalbox, sizeof(unsigned));
			usleep(2000);
			
//			e_read(&emem, 0, 0, 0x0, &result_0, sizeof(unsigned)*total_num);
//			e_read(&emem, 0, 0, 0x200, &result_1, sizeof(unsigned)*total_num);
//			e_read(&emem, 0, 0, 0x400, &result_2, sizeof(unsigned)*total_num);
			
		}
	}
	


	e_close(&dev);
	e_free(&emem);
	e_finalize();

	return 0;
}

void test_active()
{
	return;
}


void test_gid()
{
/*
	unsigned signalbox;
	signalbox = s_gid;
	e_write(&dev, i, j, sigaddress, &signalbox, sizeof(unsigned));
	
	usleep(10000);
	
	e_read(&dev, 0, 0, 0x0, &result_0, sizeof(unsigned)*total_num);
*/	
	
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



