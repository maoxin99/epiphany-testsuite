#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <e_lib.h>

#define e_status 0x000f0404


char outbuf[4096] SECTION("shared_dram");



void test_add();
void test_and();
void test_asr();
void test_bitr();
void test_eor();
void test_fabs();
void test_fadd();
void test_fmadd();
void test_fmul();
void test_fmsub();
void test_fsub();
void test_gid();
void test_iadd();
void test_imadd();
void test_imsub();
void test_imul();
void test_isub();
void test_sub();
void test_jr();
void test_lsl();
void test_lsr();
void test_orr();
void test_fix();
void test_float();
void test_ldr();
void test_mov();
void test_movs();
void test_testset();

void test_sprintf() {
	double a, b, c;
	//a = 1234.6f;
	a = 10.0f;
	b = 0.0f;
	c = 0.0f;
	//sprintf(outbuf + strlen(outbuf),"a = %8.5f!\n", a);
	sprintf(outbuf + strlen(outbuf),"a = %8.5f, b = %8.5f!\n\t", a, b);	
	//sprintf(outbuf + strlen(outbuf),"a = %f, b = %f, c = %f!\n", a, b, c);	
	
//	sprintf(outbuf + strlen(outbuf),"test: ADD failed, result = %d!\t\t\tWarnning, expecting result = -20!\n", c);



	return;
}


int main(void) {
	sprintf(outbuf, "\t");
	//test_sprintf();

	test_add();
	test_and();
	test_asr();
	test_bitr(); 
	test_eor();
	test_fabs();
	test_fadd();
	test_fmadd();
	test_fmul();
	test_fmsub();
	test_fsub();
	test_gid();
	test_iadd();
	test_imadd();
	test_imsub();
	test_imul();
	test_isub();
	test_sub();
	test_jr();
	test_lsl();
	test_lsr();
	test_orr();
	test_fix();
	test_float();
	test_ldr();
	test_mov();
	test_movs();


	test_testset();


	return EXIT_SUCCESS;
}

void test_add() {
	int a, b, c;
	a = 40; b = -60; c = 0;
	unsigned d = 0;
	int * p0, p1;
	p0 = (int *) e_get_global_address(e_group_config.core_row, e_group_config.core_col, e_status);
	//add
	__asm__ __volatile__ ("add %0, %1, %2": "=r"(c): "r"(a), "r"(b));
	//read status reg
	d = *p0;
	if((c == -20) & (d == 0x21))
		sprintf(outbuf + strlen(outbuf),"test: ADD passed!\n\t", c);
	else if((c == -20) & (d != 0x21))
		sprintf(outbuf + strlen(outbuf),"test: ADD failed, status = 0x%08x!\t\t\tWarnning, expecting status = 0x00000051!\n\t", d);
	else if((c != -20) & (d == 0x21))
		sprintf(outbuf + strlen(outbuf),"test: ADD failed, result = %d!\t\t\tWarnning, expecting result = -20!\n\t", c);
	else
		sprintf(outbuf + strlen(outbuf),"test: ADD failed, result = %d, status = 0x%08x!\t\tWarnning, expecting result = -20, status = 0x00000051!\n\t",c,d);

	//add(displacement)
	c = 0; d = 0;
	__asm__ __volatile__ ("add %0, %1, #-60": "=r"(c): "r"(a));
	//read status reg
	d = *p0;
	if((c == -20) & (d == 0x21))
		sprintf(outbuf + strlen(outbuf),"test: ADD (DIS) passed!\n\t", c);
	else if((c == -20) & (d != 0x21))
		sprintf(outbuf + strlen(outbuf),"test: ADD (DIS) failed, status = 0x%08x!\t\t\tWarnning, expecting status = 0x00000051!\n\t", d);
	else if((c != -20) & (d == 0x21))
		sprintf(outbuf + strlen(outbuf),"test: ADD (DIS) failed, result = %d!\t\t\tWarnning, expecting result = -20!\n\t", c);
	else
		sprintf(outbuf + strlen(outbuf),"test: ADD (DIS) failed, result = %d, status = 0x%08x!\t\tWarnning, expecting result = -20, status = 0x00000051!\n\t",c,d);

	return;
}

void test_and() {
	unsigned a, b, c;
	a = 0xe3; b = 0x79; c = 0x0;
	__asm__ __volatile__ ("and %0, %1, %2": "=r"(c): "r"(a), "r"(b));
	if(c == 0x61)
		sprintf(outbuf + strlen(outbuf),"test: AND passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: AND failed!\t\t\tWarnning, expecting c = 0x00000061!\n\t");
	return;
}

void test_asr() {
	unsigned a, b, c;
	a = 0xf0; b = 4; c = 0;
	//reg,reg
	__asm__ __volatile__ ("asr %0, %1, %2": "=r"(c): "r"(a), "r"(b));
	if(c == 0xf)
		sprintf(outbuf + strlen(outbuf),"test: ASR passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: ASR failed!\t\t\tWarnning, expecting c = 0x0000000f!\n\t");
	//reg,imm
	__asm__ __volatile__ ("asr %0, %1,#4 ": "=r"(c): "r"(a));
	if(c == 0xf)
		sprintf(outbuf + strlen(outbuf),"test: ASR (DIS) passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: ASR (DIS) failed!\t\t\tWarnning, expecting c = 0x0000000f!\n\t");


	return;
}

void test_bitr() {
	unsigned a, b;
	a = 0x13; b = 0x0;
	__asm__ __volatile__ ("bitr %0, %1": "=r"(b): "r"(a));
	if(b == 0xc8000000)
		sprintf(outbuf + strlen(outbuf),"test: BITR passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: BITR failed!\t\t\tWarnning, expecting b = 0xc8000000!\n\t");
	return;
}

void test_eor() {
	unsigned a, b, c, d;
	a = 0x9999; b = 0x2222; c = 0x0, d = 0xbbbb;
	__asm__ __volatile__ ("eor %0, %1, %2": "=r"(c): "r"(a), "r"(b));
	if(c == d)
		sprintf(outbuf + strlen(outbuf),"test: EOR passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: EOR failed!\t\t\tWarnning, expecting c = 0x0000bbbb!\n\t");
	return;
}

void test_fadd() {
	float a, b, c;
	a = 4.6f; b = 5.4f; c = 0.0f;

	__asm__ __volatile__ ("fadd %0, %1, %2":"=r"(c):"r"(a),"r"(b):);
	if(c == 10.0f)
		sprintf(outbuf + strlen(outbuf),"test: FADD passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FADD failed!\t\t\tWarnning, expecting c = 10.000000!\n\t");
	
	return;
}

void test_fabs() {
	float a, b;
	a = -4.6f; 
	b = 0.0f;
	__asm__ __volatile__ ("fabs %0, %1":"=r"(b):"r"(a):);
	if(b == 4.6f)
		sprintf(outbuf + strlen(outbuf),"test: FABS passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FABS failed!\t\t\tWarnning, expecting b = 4.600000!\n\t");
	
	return;
}

void test_fmadd() {
	float a, b, c;
	a = 4.0f; b = 6.0f; c = 3.0f;

	__asm__ __volatile__ ("fmadd %0, %1, %2":"+r"(c):"r"(a),"r"(b):);
	if(c == 27.0f)
		sprintf(outbuf + strlen(outbuf),"test: FMADD passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FMADD failed!\t\t\tWarnning, expecting c = 27.000000!\n");
	
	return;
}

void test_fmul() {
	float a, b, c;
	a = 4.6f; b = 5.4f; c = 3.0f;

	__asm__ __volatile__ ("fmul %0, %1, %2":"=r"(c):"r"(a),"r"(b):);
	if(c == 24.84f)
		sprintf(outbuf + strlen(outbuf),"test: FMUL passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FMUL failed!\t\t\tWarnning, expecting c = 24.840000!\n\t");
	
	return;
}

void test_fmsub() {
	float a, b, c;
	a = 4.6f; b = 5.4f; c = 3.0f;

	__asm__ __volatile__ ("fmsub %0, %1, %2":"+r"(c):"r"(a),"r"(b):);
	if(c == -21.84f)
		sprintf(outbuf + strlen(outbuf),"test: FMSUB passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FMSUB failed!\t\t\tWarnning, expecting c = -21.840000!\n\t");
	
	return;
}

void test_fsub() {
	float a, b, c, r;
	a = 5.4f; b =4.6f; c = 3.0f; r = 0.8f; 

	__asm__ __volatile__ ("fsub %0, %1, %2":"=r"(c):"r"(a),"r"(b):);
	if((c <= r*1.05)&&(c >= r*0.95))
		sprintf(outbuf + strlen(outbuf),"test: FSUB passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FSUB failed!\t\t\tWarnning, expecting c = 0.800000!\n\t");
	
	return;
}

void test_gid() {
	unsigned c;
	unsigned set1, set2, set3;
	set1 = 0; set2 = 0; set3 = 0;
	c = 0x0;
	//disable for the 1st time
	__asm__ __volatile__ ("GID");
	c = e_reg_read(E_REG_STATUS);
	if((c&0x2) != 0x2) {
		set1 =1;
		sprintf(outbuf + strlen(outbuf),"test: GID failed!\t\t\tWarnning, current  state = 0x%08x!\n\t",c);
	}

	//enalbe
	__asm__ __volatile__ ("GIE");
	c = e_reg_read(E_REG_STATUS);
	if((c&0x2) != 0x0) {
		set2 =1;
		sprintf(outbuf + strlen(outbuf),"test: GIE failed!\t\t\tWarnning, current  state = 0x%08x!\n\t",c);
	}

	//disable for the 2nd time
	__asm__ __volatile__ ("GID");
	c = e_reg_read(E_REG_STATUS);
	if((c&0x2) != 0x2) {
		set3 =1;
		sprintf(outbuf + strlen(outbuf),"test: GID failed!\t\t\tWarnning, current  state = 0x%08x!\n\t",c);
	}

	//check if succeed
	if(set1==0 && set2==0 &&set3==0)
		sprintf(outbuf + strlen(outbuf),"test: GID & GIE passed!\n\t");

	return;

}


void test_iadd() {
	int a, b, c;
	a = -40; b = -60; c = 230;
	e_reg_write(E_REG_CONFIG, 0x80000);
	//reg,reg
	__asm__ __volatile__ ("iadd %0, %1, %2": "=r"(c): "r"(a), "r"(b));
	if(c == -100)
		sprintf(outbuf + strlen(outbuf),"test: IADD passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: IADD failed!\t\t\tWarnning, expecting c = -100!\n\t");
	return;
}

void test_imadd() {
	int a, b, c;
	e_reg_write(E_REG_CONFIG, 0x80000);
	a=4; b=6; c=3;

	__asm__ __volatile__ ("imadd %0, %1, %2"
		: "+r"(c): "r"(a), "r"(b));
	if(c == 27)
		sprintf(outbuf + strlen(outbuf),"test: IMADD passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: IMADD failed!\t\t\tWarnning, expecting c = 27!\n\t");

	e_reg_write(E_REG_CONFIG, 0x0);
	return;

}

void test_imsub() {
	int a, b, c;
	e_reg_write(E_REG_CONFIG, 0x80000);
	a=4; b=6; c=3;

	__asm__ __volatile__ ("imsub %0, %1, %2"
		: "+r"(c): "r"(a), "r"(b));
	if(c == -21)
		sprintf(outbuf + strlen(outbuf),"test: IMSUB passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: IMSUB failed!\t\t\tWarnning, expecting c = -21!\n\t");

	e_reg_write(E_REG_CONFIG, 0x0);
	return;

}

void test_imul() {
	int a, b, c;
	e_reg_write(E_REG_CONFIG, 0x80000);
	a=4; b=6; c=3;

	__asm__ __volatile__ ("imul %0, %1, %2"
		: "=r"(c): "r"(a), "r"(b));
	if(c == 24)
		sprintf(outbuf + strlen(outbuf),"test: IMUL passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: IMUL failed!\t\t\tWarnning, expecting c = 24!\n\t");

	e_reg_write(E_REG_CONFIG, 0x0);
	return;

}

void test_isub() {
	int a, b, c;
	e_reg_write(E_REG_CONFIG, 0x80000);
	a=4; b=6; c=3;

	__asm__ __volatile__ ("isub %0, %1, %2"
		: "=r"(c): "r"(a), "r"(b));
	if(c == -2)
		sprintf(outbuf + strlen(outbuf),"test: ISUB passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: ISUB failed!\t\t\tWarnning, expecting c = -2!\n\t");

	e_reg_write(E_REG_CONFIG, 0x0);
	return;

}


void test_sub() {
	int a, b, c;
	a = 40; b = 60; c = 0;

	//sub
	__asm__ __volatile__ ("sub %0, %1, %2  	"
		: "=r"(c): "r"(a), "r"(b));
	if(c == -20)
		sprintf(outbuf + strlen(outbuf),"test: SUB passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: SUB failed!\t\t\tWarnning, expecting c = -20!\n\t");

	//sub(displacement)
	__asm__ __volatile__ ("sub %0, %1, 60 "
		: "=r"(c): "r"(a));
	if(c == -20)
		sprintf(outbuf + strlen(outbuf),"test: SUB (DIS) passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: SUB (DIS) failed!\t\t\tWarnning, expecting c = -20!\n\t");


	return;
}

void test_jr() {
	unsigned *lab;
	unsigned i;
	unsigned jr_count, jr_sp_count;
	jr_count = 0; jr_sp_count = 0;
	lab = &&mylab;
	//jump
	__asm__ __volatile__("jr %0"::"r"(lab):);
	
	//if jump failed, jr_sp_count will accumulated to 2
	jr_sp_count++;

mylab:	for(i=0;i<50;i++){
	jr_count++;
	}

	jr_sp_count++;
	if(jr_sp_count == 1)
		sprintf(outbuf + strlen(outbuf),"test: JR passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: JR failed!\t\t\tWarnning, expecting jr_sp_count = 1!\n\t");
	return;
}



void test_lsl() {
	unsigned a, b, c;
	a = 0x12345678;
	b = 0x10000004;

	__asm__ __volatile__ ("lsl %0, %1, %2"
		: "=r"(c): "r"(a), "r"(b));

	if(c == 0x23456780)
		sprintf(outbuf + strlen(outbuf),"test: LSL passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: LSL failed!\t\t\tWarnning, expecting c = 0x23456780!\n\t");

	return;
}

void test_lsr() {
	unsigned a, b, c;
	a = 0x12345678;
	b = 0x10000004;

	__asm__ __volatile__ ("lsr %0, %1, %2"
		: "=r"(c): "r"(a), "r"(b));

	if(c == 0x01234567)
		sprintf(outbuf + strlen(outbuf),"test: LSR passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: LSR failed!\t\t\tWarnning, expecting c = 0x01234567!\n\t");

	return;
}


void test_orr() {
	unsigned a, b, c;
	a = 0x12345678;
	b = 0x30400304;

	__asm__ __volatile__ ("orr %0, %1, %2"
		: "=r"(c): "r"(a), "r"(b));

	if(c == 0x3274577c)
		sprintf(outbuf + strlen(outbuf),"test: ORR passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: ORR failed!\t\t\tWarnning, expecting c = 0x3274577c!\n\t");

	return;
}



void test_fix() {
	float a;
	int b;
	a = -134.567f;
	__asm__ __volatile__("fix %0, %1":"=r"(b):"r"(a):);
	if(b == -135)
		sprintf(outbuf + strlen(outbuf),"test: FIX passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FIX failed!\t\t\tWarnning, expecting b = -135!\n\t");

	return;
}

void test_float() {
	float b;
	int a;
	a = -135;
	
	
	__asm__ __volatile__("float %0, %1":"=r"(b):"r"(a):);

	if((b<=-134.0f)&(b>=-136.0f))
		sprintf(outbuf + strlen(outbuf),"test: FLOAT passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test: FLOAT failed!\t\t\tWarnning, expecting b = -135.0f!\n\t");

	return;
}

void test_ldr() {
	unsigned a,b,c,d,r;
	a = 0x1234;
	b = 0x4000;
	c = 0x200;
	d = 0x4200;
	unsigned *p0;
	p0 =(unsigned *) 0x4200;
	//test index
	*p0 = 0x20;
	__asm__ __volatile__("str %0,[%1,%2]"::"r"(a),"r"(b),"r"(c):);
	__asm__ __volatile__("ldr %0, [%1, %2]":"=r"(r):"r"(b),"r"(c):);
	
	if(r == a)
		sprintf(outbuf + strlen(outbuf),"test:LDR & STR (INDEX) passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:LDR & STR (INDEX) failed!\t\t\tWarning: failed!\n\t");
	//test displacement
	*p0 = 0x40;
	__asm__ __volatile__("str %0,[%1,#0x80]"::"r"(a),"r"(b):);
	__asm__ __volatile__("ldr %0, [%1, #0x80]":"=r"(r):"r"(b):);	
	if(r == a)
		sprintf(outbuf + strlen(outbuf),"test:LDR & STR (DISPLACEMENT) passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:LDR & STR (DISPLACEMENT) failed!\t\t\tWarning: failed!\n\t");
	//test ldr post
	p0 = (unsigned *) 0x4000;
	*p0 = 0x60;
	__asm__ __volatile__("ldr %0, [%1], %2":"=r"(r),"+r"(b):"r"(c):);	
	if((r == 0x60) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST) passed!\n\t");
	else if((r != a) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST) failed!\t\t\tWarning: RD isn't correct!\n\t");
	else if((r == a) & (b != 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST) failed!\t\t\tWarning: RN isn't correct!\n\t");
	else 
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST) failed!\t\t\tWarning: both RD and RN isn't correct!\n\t");
	//test ldr post+dis
	p0 = (unsigned *) 0x4000;
	b = 0x4000;
	*p0 = 0x80;
	__asm__ __volatile__("ldr %0, [%1], #0x80":"=r"(r),"+r"(b):"r"(c):);	
	if((r == 0x80) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST+DIS) passed!\n\t");
	else if((r != a) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST+DIS) failed!\t\t\tWarning: RD isn't correct!\n\t");
	else if((r == a) & (b != 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST+DIS) failed!\t\t\tWarning: RN isn't correct!\n\t");
	else 
		sprintf(outbuf + strlen(outbuf),"test:LDR (POST+DIS) failed!\t\t\tWarning: both RD and RN isn't correct!\n\t");

	//test str post
	p0 = (unsigned *) 0x4000;
	b = 0x4000;
	*p0 = 0x0;
	__asm__ __volatile__("str %1,[%0],%2":"+r"(b):"r"(a),"r"(c):);
	if((*p0 == a) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:STR (POST) passed!\n\t");
	else if((*p0 != a) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:STR (POST) failed!\t\t\tWarning: RD isn't correct!\n\t");
	else if((*p0 == a) & (b != 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:STR (POST) failed!\t\t\tWarning: RN isn't correct!\n\t");
	else 
		sprintf(outbuf + strlen(outbuf),"test:STR (POST) failed!\t\t\tWarning: both RD and RN isn't correct!\n\t");

	//test str post_dis
	p0 = (unsigned *) 0x4000;
	b = 0x4000;
	*p0 = 0x0;
	__asm__ __volatile__("str %1,[%0],#0x80":"+r"(b):"r"(a):);
	if((*p0 == a) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:STR (POST+DIS) passed!\n\t");
	else if((*p0 != a) & (b == 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:STR (POST+DIS) failed!\t\t\tWarning: RD isn't correct!\n\t");
	else if((*p0 == a) & (b != 0x4200))
		sprintf(outbuf + strlen(outbuf),"test:STR (POST+DIS) failed!\t\t\tWarning: RN isn't correct!\n\t");
	else 
		sprintf(outbuf + strlen(outbuf),"test:STR (POST+DIS) failed!\t\t\tWarning: both RD and RN isn't correct!\n\t");
	return;
}

void test_mov() {
	int a, b, c;
	a = 0x12345678;
	b = 0x123;
	c = 0x12345678;
	//test mov
	__asm__ __volatile__("mov %0, %1":"=r"(b):"r"(a):);
	if(b == a)
		sprintf(outbuf + strlen(outbuf),"test:MOV passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:MOV failed!\t\t\tWarning: expecting b = 0x12345678!\n\t");
	//test mov(immediate)
	__asm__ __volatile__("mov %0, #0x345":"=r"(c)::);
	if(c == 0x345)
		sprintf(outbuf + strlen(outbuf),"test:MOV (IMMEDIATE) passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:MOV (IMMEDIATE) failed!\t\t\tWarning: expecting c = 0x00000345!\n\t");
	//test movt
	__asm__ __volatile__("movt %0, #0x345":"=r"(c)::);
	if(c == 0x03450345)
		sprintf(outbuf + strlen(outbuf),"test:MOVT passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:MOVT failed!\t\t\tWarning: expecting c = 0x03450345!\n\t");
	return;

}

void test_movs() {
	int a, b, c, d;

	//test movts
	d = 0x00080000;
	__asm__ __volatile__("movts CONFIG, %0"::"r"(d):);
	a = e_reg_read(E_REG_CONFIG);
	if(a == d)
		sprintf(outbuf + strlen(outbuf),"test:MOVTS passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:MOVTS failed!\t\t\tWarning: expecting a = 0x%08x!\n\t", d);

	d = 0x0;
	__asm__ __volatile__("movts CONFIG, %0"::"r"(d):);


	//test movfs
	d = 0x0;
	a = e_reg_read(E_REG_CONFIG);
	__asm__ __volatile__("movfs %0, CONFIG":"=r"(c)::);
	if(d == a)
		sprintf(outbuf + strlen(outbuf),"test:MOVFS passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:MOVFS failed!\t\t\tWarning: expecting d = 0x%08x!\n\t",a);

	return;

}


void test_testset() {
	int a, b, c, d;
	a = 0x1234;
	//b = 0x80806000;
	c = 0x200;
	int * p0, p1;
	p1 = (int *) 0x6000;
	b = (int *)e_get_global_address(e_group_config.core_row, e_group_config.core_col, p1);
	p0 = 0x6200;

	//is zero
	*p0 = 0x0;
	__asm__ __volatile__("TESTSET %0,[%1,%2]":"+r"(a):"r"(b),"r"(c):);
	d = *p0;
	if((a == 0x0)&(d == 0x1234))
		sprintf(outbuf + strlen(outbuf),"test:TESTSET (is zero) passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:TESTSET (is zero) failed!\t\t\tWarnning, failed!\n\t");
	//is not zero
	a = 0x1234;
	*p0 = 0x100;
	__asm__ __volatile__("TESTSET %0,[%1,%2]":"=r"(a):"r"(b),"r"(c):);
	d = *p0;
	if((a == 0x100)&(d == 0x100))
		sprintf(outbuf + strlen(outbuf),"test:TESTSET (is not zero) passed!\n\t");
	else
		sprintf(outbuf + strlen(outbuf),"test:TESTSET (is not zero) failed!\t\t\tWarnning, failed!\n\t");
	return;
}







