//------- start -------------
//int hwloop(count);

.file "hwloop.s";
.section .text;
.type   _hwloop, %function;
.global _hwloop;
.extern _A;   A global array A[] defined in one of the C modules
.extern _B;
.extern _result;

.balign 4;
_hwloop:

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
//mov   r0, #0x400;
	movts lc, r0;

	mov	  r0,  _A		;A[N] defined in emain.c
	movt	r0,  _A
	mov	  r1,  _B		;B[N] defined in emain.c
	movt	r1,  _B
	mov	  r44, 0x0	;sum = 0
	mov   r45, 0x0	;document time
	mov   r46, 0x1
	mov   r47, 0x0
	mov   r48, 0x1
	
.balignw 8,0x000001a2;

lstart:	 
	ldr.l	r2, [r0], +0x1 ;load the data
	fadd.l r47, r47, r48
	
.balignw 8,0x000001a2;
	fadd.l r45, r45, r46
	fadd.l r49, r0, r1
	
.balignw 8,0x000001a2;
	ldr.l	 r3, [r1], +0x1
lend:
	fmadd.l	r44, r3, r2


	idle
	gie

	mov 	r0, r44		;save the result into r0
//mov   r0, r45
	 
	rts; return


.size _hwloop, .-_hwloop;
//------- end -------------

