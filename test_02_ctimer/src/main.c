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
#include <string.h>
#include <unistd.h>

#include <e-hal.h>

#define _BufSize   (0x100)
#define _BufOffset (0x01000000)


void check();


//set up the expected expect list

//for internal mem, fast.ldf
//unsigned expect[] = {676, 0, 213, 512, 128, 8, 13, 0, 4, 100000};
//for external mem, fast.ldf
//unsigned expect[] = {26300, 0, 213, 512, 128, 264, 25750, 0, 25300, 100000};
//for external mem, legacy.df
unsigned expect[] = {312000, 0, 213, 512, 128, 992, 54500, 290000, 51000, 86300};

unsigned result[10];
char event[10][20];

int main(int argc, char *argv[])
{
	unsigned rows, cols, coreid, i, j;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	// Allocate a buffer in shared external memory
	// for message passing from eCore to host.
	e_alloc(&emem, _BufOffset, _BufSize);

	//open the workgroup
	rows = platform.rows;
	cols = platform.cols;
	e_open(&dev, 0, 0, rows, cols);
	
	//load the device program on the board
	e_load_group("emain.srec", &dev, 0, 0, rows, cols, E_FALSE);

	//set up the event list table
	strcpy(event[0], "CLK");
	strcpy(event[1], "IDLE");
	strcpy(event[2], "IALU_INST");
	strcpy(event[3], "FPU_INST");
	strcpy(event[4], "DUAL_INST");
	strcpy(event[5], "E1_STALLS");
	strcpy(event[6], "RA_STALLS");
	strcpy(event[7], "EXT_FETCH_STALLS");
	strcpy(event[8], "EXT_LOAD_STALLS");
	strcpy(event[9], "IALU_INST");

	for (i=0; i<rows; i++)
	{
		for (j=0; j<cols; j++)
		{

			coreid = (i + platform.row) * 64 + j + platform.col;
			fprintf(stderr, "Message from eCore 0x%03x (%2d,%2d): \n", coreid, i, j);
			e_start(&dev, i, j);

			//wait for core to execute the program
			usleep(100000);

			e_read(&emem, 0, 0, 0x0, &result, sizeof(unsigned)*10);

			check();
		}	
	}	

	e_close(&dev);
	e_free(&emem);
	e_finalize();

	return 0;
}


void check()
{
	int t, fault;
	
	fault = 0;
	
	for (t=0; t<10; t++)
	{
		if ((result[t]>expect[t]*1.05) || (result[t]<expect[t]*0.95))
		{
			fprintf(stderr, "test02 CTimer failed! cycles spent on event %-18s is 0x%08x cycles! \
			Expecting %-6d cycles!\n", event[t], result[t], expect[t]);
			fault++;
		}
	}

	if (fault == 0)
		fprintf(stderr, "test02 CTimer passed!\n\n");
	else
		fprintf(stderr, "test02 CTimer failed! Number of faults is %d!\n\n", fault);
	
	return;
}

