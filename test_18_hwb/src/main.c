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

#define _BufSize   (100)
#define _BufOffset (0x01000000)

int main(int argc, char *argv[])
{
	unsigned rows, cols, coreid, _i, _j;
	int result[16];
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	int fault;

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
	e_start_group(&dev);
	
	fault = 0x0;
	for (_i=0;_i<100;_i++)
	{
		usleep(100000);
		e_read(&emem,0, 0, 0x0, &result, sizeof(int)*16);
		
		for (_j=0;_j<16;_j++)
		{
			if(result[_j] != result[0])
				fault++;
		}
		
		for (_j=0;_j<4;_j++)
		{
			fprintf(stderr,"core%d: 0x%08x  ",_j, result[_j]);
		}
		
		fprintf(stderr,"\n");
	}		
	
	if(fault == 0)
		fprintf(stderr,"\ntest20: Hardware Barrier Passed!\n");
	else
		fprintf(stderr,"\ntest20: Hardware Barrier Failed! Fault is 0x%08x!\n",fault);

	e_close(&dev);
	e_free(&emem);
	e_finalize();

	return 0;
}

