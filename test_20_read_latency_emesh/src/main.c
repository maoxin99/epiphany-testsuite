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

#define _BufSize   (4096)
#define _BufOffset (0x01000000)

int main(int argc, char *argv[])
{
	unsigned _row, _col, coreid, _i, _j;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	char emsg[_BufSize];
	unsigned _pc;
	int result;


	srand(1);

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
	_row = platform.rows;
	_col = platform.cols;
	e_open(&dev, 0, 0, _row, _col);

	//reset the workgroup
	for (_i = 0; _i < _row; _i++){
		for (_j = 0; _j < _col; _j++){
			e_reset_core(&dev, _i, _j);
		}
	}

	//load the device program on the board
	e_load_group("emain.srec", &dev, 0, 0, _row, _col, E_FALSE);
	

	for (_i=0; _i<platform.rows; _i++)
	{
		for (_j=0; _j<platform.cols; _j++)
		{

			coreid = (_i + platform.row) * 64 + _j + platform.col;
			fprintf(stderr, "Message from eCore 0x%03x (%2d,%2d): \n", coreid, _i, _j);
			e_start(&dev, _i, _j);
			usleep(100000);
			e_read(&dev, _i, _j, 0x5200, emsg, _BufSize);
			result = emsg[0];


			if(result == 0)
				fprintf(stderr, "\"test20 emesh_read_latency\t\tPassed!\"\n");	
			else
				fprintf(stderr, "\"test20 emesh_read_latency\t\tfailed!\t\t\tWarnning, test failed!\"\n");
		}	
	}	



/*

	//for one core
	_i = 0; _j = 0;
	coreid = (_i + platform.row) * 64 + _j + platform.col;
	fprintf(stderr, "Message from eCore 0x%03x (%2d,%2d): \n", coreid, _i, _j);
	e_start(&dev, _i, _j);
	usleep(100000);
	//e_read(&dev, 0, 0, 0x5200, emsg, _BufSize);
	e_read(&emem, 0, 0, 0x0, emsg, _BufSize);
	fprintf(stderr, "\"%s\"\n", emsg);
*/

/*
	int result;
	result = emsg[0];
	if(result == 0)
		fprintf(stderr, "\"test MULTICAST	passed!\"\n");	
	else
		fprintf(stderr, "\"test MULTICAST	failed!\t\t\tWarnning, test failed! Num of fualt is %d!\"\n", result);	
*/



	e_free(&emem);
	e_finalize();

	

	return 0;
}

