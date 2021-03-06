//------- start -------------
//int hwlooptwo(count);

.file "hwlooptwo.s";
.section .text;
.type   _hwlooptwo, %function;
.global _hwlooptwo;
.extern _A;   A global array A[] defined in one of the C modules
;.extern _B;
.extern _result;

.balign 4;
_hwlooptwo:

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
	mov   r44, 0x0	;sum = 0
	
	mov   r45, 0x0
	mov   r46, 0x1
	
.balignw 8,0x000001a2;
lstart:	
	ldr.l	  r2, [r0], +0x1 ;load the data
	
lend:	
	fadd.l	r44, r44, r2
//fadd.l   r45, r46, r45

	gie

	mov 	r0, r44		;save the result into r0
//mov r0, r45
	 
	rts; return


.size _hwlooptwo, .-_hwlooptwo;
//------- end -------------

