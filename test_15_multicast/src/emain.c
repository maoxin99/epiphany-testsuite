#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <e-lib.h>


char outbuf[4096] SECTION("shared_dram");

#define E_REG_MULTICAST 0xf0708
#define _X 0x011
#define _Y 0x012
#define _Z 0x013
#define _W 0x014




int main(void) {
	//initialize
	int core_num;
	int *p0, *p1;
	//sprintf(outbuf,"\t");
	//core_num = e_group_config.core_row * 4 + e_group_config.core_col;

	//p1 = (int *);
	p0 = e_get_global_address(E_SELF, E_SELF,(void *) E_REG_MULTICAST);

//	*p1 = _Y;
//	*p0 = _Z;
//	sprintf(outbuf+strlen(outbuf),"p1 = 0x%08x, p0 = 0x%08x! *p1 = 0x%08x!\n\t",p1, p0, *p1);

	*p0 = _Y;
/*
	switch(core_num) {
		case 1: *p0 = _X; break;
		case 2: *p0 = _Y; break;
		case 3: *p0 = _Z; break;
		case 4: *p0 = _W; break;
		case 5: *p0 = _Y; break;
		case 6: *p0 = _Z; break;
		case 7: *p0 = _W; break;
		case 8: *p0 = _X; break;
		case 9: *p0 = _Z; break;
		case 10: *p0 = _W; break;
		case 11: *p0 = _X; break;
		case 12: *p0 = _Y; break;
		case 13: *p0 = _W; break;
		case 14: *p0 = _X; break;
		case 15: *p0 = _Y; break;
		default: break;
	}
*/

	
//	__asm__ __volatile__("idle");
	
	return EXIT_SUCCESS;
}




