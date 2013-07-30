//------- start -------------
//int sfloop();

.file "sfloop.s";
.section .text;
.type   _sfloop, %function;
.global _sfloop;
.extern _A;   A global array A[] defined in one of the C modules
.extern _B;
.extern _result;

.balign 4;
_sfloop:


	mov 	r2, 0x0		; change the cal type
	movt	r2, 0x8
	gid
	movfs	r3, config
	orr	  r3, r3, r2
	movts	config, r3
	gie

	mov	  r45,0x400

	mov	  r0, _A		;A[N] defined in emain.c
	movt	r0, _A
	mov	  r1, _B		;B[N] defined in emain.c
	movt	r1, _B
	mov	  r44, 0x0		;sum = 0
	
loop_start:	
	ldr	  r2, [r0], +0x1 ;load the data
	ldr	  r3, [r1], +0x1
	fmul	r2, r2, r3
	fadd	r44, r44, r2
	
	sub	  r45,r45,+0x1
	bne	  loop_start

	mov 	r0, r44		;save the result into r0

 
	rts; return


.size _sfloop, .-_sfloop;
//------- end -------------

