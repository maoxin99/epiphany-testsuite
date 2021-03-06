//------- start -------------
//int hwloopone(count);

.file "hwloopone.s";
.section .text;
.type   _hwloopone, %function;
.global _hwloopone;
.extern _A;   A global array A[] defined in one of the C modules
.extern _B;
.extern _result;

.balign 4;
_hwloopone:

	mov 	r2, 0x0		; change the cal type
	movt	r2, 0x8
	gid
	movfs	r3, config
	orr	  r3, r3, r2
	movts	config, r3
	mov   r1, %low(lstart)	;setting the hardware loop register
	movt  r1, %high(lstart)	;
	movts ls, r1;
	mov   r1, %low(lend)	;
	movt  r1, %high(lend)	;
	movts le, r1;
	
	
	movts lc, r0;

	mov	  r0, _A		;A[N] defined in emain.c
	movt	r0, _A
	mov	  r1, _B		;B[N] defined in emain.c
	movt	r1, _B
	mov	  r44, 0x0	;sum = 0

.balignw 8,0x000001a2;
lstart:	
	ldr.l	  r2, [r0], +0x1 ;load the data

	
.balignw 8,0x000001a2;
	ldr.l	  r3, [r1], +0x1
lend:	  
	fmadd.l	r44, r3, r2

//idle	
	gie

	mov 	r0, r44		;save the result into r0
//	idle 
	rts; return


.size _hwloopone, .-_hwloopone;
//------- end -------------

