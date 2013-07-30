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

#define _BufSize   	(0x00000200)
#define _BufOffset 	(0x01000000)
#define mailbox     (0x00006f00)


//---------------------------------reset group------------------------------------//
#define diag(vN) if (1)

ssize_t ee_write_reg(e_epiphany_t *dev, unsigned row, unsigned col, off_t to_addr, int data);
int e_reset_group(e_epiphany_t *dev);


// Reset a workgroup
int e_reset_group(e_epiphany_t *dev)
{
    int RESET0 = 0x0;
    int RESET1 = 0x1;
    int CONFIG = 0x01000000;
    int row, col;

    diag(H_D1) { fprintf(stderr, "e_reset_group(): halting core...\n"); }
    for (row=0; row<dev->rows; row++)
        for (col=0; col<dev->cols; col++)
            e_halt(dev, row, col);
    diag(H_D1) { fprintf(stderr, "e_reset_group(): pausing DMAs.\n"); }

    for (row=0; row<dev->rows; row++)
        for (col=0; col<dev->cols; col++)
            e_write(dev, row, col, E_REG_CONFIG, &CONFIG, sizeof(unsigned));

    usleep(100000);
/*
    diag(H_D1) { fprintf(stderr, "e_reset_group(): resetting cores...\n"); }
    for (row=0; row<dev->rows; row++)
        for (col=0; col<dev->cols; col++)
        {
            ee_write_reg(dev, row, col, E_REG_CORE_RESET, RESET1);
            ee_write_reg(dev, row, col, E_REG_CORE_RESET, RESET0);
        }

    diag(H_D1) { fprintf(stderr, "e_reset_group(): done.\n"); }
*/
    return E_OK;
}
//---------------------------------reset group------------------------------------//



int main(int argc, char *argv[])
{
	unsigned rows, cols, coreid, i, j, k;
	e_platform_t platform;
	e_epiphany_t dev;
	e_mem_t emem;
	unsigned result[32];


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
	
	usleep(1000);
	
	e_load("idle.srec", &dev, 1, 1, E_FALSE);
	e_start(&dev, 1, 1);
	
	usleep(1000);
	
	e_start_group(&dev);
	
	fprintf(stderr,"========================================>>><<<================================================================================\n");


	for (k=0; k<10; k++)
	{

		e_read(&emem, i, j, 0x0, &result[0], 32*sizeof(unsigned));
		
		for (i=0;i<16;i++)
		{
				fprintf(stderr, "0x%04x  ", result[i]);
		}
	
	fprintf(stderr,"\n");
	//insert reset
	if (k == 5)
	{
		//fprintf(stderr,"===============================>>>reset core(1,1)<<<==========================================================================\n");
		//e_reset_core(&dev, 1, 1);
		fprintf(stderr,"===================================>>>reset group<<<==========================================================================\n");
		e_reset_group(&dev);
	}
	
	usleep(100000);
	}
	
	
	fprintf(stderr,"========================================>>><<<================================================================================\n");
	
	usleep(10000000);


	e_read(&emem, i, j, 0x0, &result[0], 32*sizeof(unsigned));
		
	for (i=0;i<16;i++)
	{
			fprintf(stderr, "0x%04x  ", result[i]);
	}

	
	fprintf(stderr,"\n");

	//check if finished
	for (i=16;i<32;i++)
	{
		if (result[i] == 0xdeadbeef)
			fprintf(stderr,"Finish  ");
		else
			fprintf(stderr,"Incmpt  ");
	}
	
	fprintf(stderr,"\n");

/*
//-----------------------------------------second round-----------------------------------------------//
//----------------------------------------------------------------------------------------------------//
	//load the device program on the board
	e_load_group("emain.srec", &dev, 0, 0, rows, cols, E_FALSE);
	
	usleep(1000);
	
	e_load("idle.srec", &dev, 1, 1, E_FALSE);
	e_start(&dev, 1, 1);
	
	usleep(1000);
	
	e_start_group(&dev);
	
	fprintf(stderr,"========================================>>><<<================================================================================\n");


	for (k=0; k<10; k++)
	{

		e_read(&emem, i, j, 0x0, &result[0], 32*sizeof(unsigned));
		
		for (i=0;i<16;i++)
		{
				fprintf(stderr, "0x%04x  ", result[i]);
		}
	
	fprintf(stderr,"\n");
	
	//insert reset
	if (k == 5)
	{
		//fprintf(stderr,"===============================>>>reset core(1,1)<<<==========================================================================\n");
		//e_reset_core(&dev, 1, 1);
		//fprintf(stderr,"===================================>>>reset group<<<==========================================================================\n");
		//e_reset_group(&dev);
	}
	
	usleep(100000);
	}
	
	
	fprintf(stderr,"========================================>>><<<================================================================================\n");
	
	usleep(10000000);


	e_read(&emem, i, j, 0x0, &result[0], 32*sizeof(unsigned));
		
	for (i=0;i<16;i++)
	{
			fprintf(stderr, "0x%04x  ", result[i]);
	}

	
	fprintf(stderr,"\n");

	//check if finished
	for (i=16;i<32;i++)
	{
		if (result[i] == 0xdeadbeef)
			fprintf(stderr,"Finish  ");
		else
			fprintf(stderr,"Incmpt  ");
	}
	
	fprintf(stderr,"\n");
*/


	e_close(&dev);
	e_free(&emem);
	e_finalize();

	return 0;
}

