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

#define sizeN (5)

int main(int argc, char *argv[])
{
	unsigned _rows, _cols, coreid, _i, _j;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	unsigned time[sizeN];
	int result[sizeN];

	// initialize system, read platform params from
	// default HDF. Then, reset the platform and
	// get the actual system parameters.
	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	//open the workgroup
	_rows = platform.rows;
	_cols = platform.cols;
	e_open(&dev, 0, 0, _rows, _cols);

	//load the device program on the board
	e_load_group("emain.srec", &dev, 0, 0, _rows, _cols, E_FALSE);

	for (_i=0; _i<_rows*0+1; _i++)
	{
		for (_j=0; _j<_cols*0+1; _j++)
		{
			coreid = (_i + platform.row) * 64 + _j + platform.col;
			fprintf(stderr, "Message from eCore 0x%03x (%2d,%2d): \n", coreid, _i, _j);
			e_start(&dev, _i, _j);

			//wait for core to execute the program
			usleep(200000);

			e_read(&dev, 0, 0, 0x5100, &result[0], sizeN*sizeof(int));
			e_read(&dev, 0, 0, 0x5200, &time[0], sizeN*sizeof(unsigned));
			fprintf(stderr, "\"result:\tauto = %d! hw = %d! hwtwo = %d! sf = %d!\"\n", result[0],result[1],result[2],result[3]);
			fprintf(stderr, "\"time:\t\tauto = %d cycles! hw = %d cycles! hwtwo = %d! sf = %d cycles!\"\n", time[0],time[1],time[2],time[3]);
			
			//fprintf(stderr, "\"result[4] = 0x%08x!\"\n", result[4]);
		}	
	}	


	// Release the allocated buffer and finalize the
	// e-platform connection.
	e_close(&dev);
	e_free(&emem);
	e_finalize();

	

	return 0;
}

