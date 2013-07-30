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

#define _BufSize   (0x100)
#define _BufOffset (0x01000000)

int main(int argc, char *argv[])
{
	unsigned rows, cols, coreid, i, j;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	int result;


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
	e_load_group("emainorigin.srec", &dev, 0, 0, rows, cols, E_FALSE);
	

//	for (i=0; i<rows; i++)
//	{
//		for (j=0; j<cols; j++)
//		{

//			i = 3; j = 3;
			
	for (i=0; i<1; i++)
	{
		for (j=0; j<2; j++)
		{
			
			
			coreid = (i + platform.row) * 64 + j + platform.col;
			fprintf(stderr, "Message from eCore 0x%03x (%2d,%2d): \n", coreid, i, j);
			e_start(&dev, i, j);
			usleep(100000);

			e_read(&dev, i, j, 0x5200, &result, sizeof(int));
			if(result == 0)
				fprintf(stderr, "\"test MULTICAST	passed!\"\n");	
			else
				fprintf(stderr, "\"test MULTICAST	failed!\t\t\tWarnning, test failed! Num of fualt is %d!\"\n", result);
			usleep(100000);	
		}	
	}	



	e_close(&dev);
	e_free(&emem);
	e_finalize();

	

	return 0;
}

