	.file	"emain.c"
	.section .text
	.balign 4
	.global	_wand_isr
_wand_isr:
	strd r0,[sp,#-1]
	movfs r0,status
	movfs r1,iret
	add sp,sp,#-16
	gie
; 52 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 53 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 54 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 55 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 56 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 57 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 58 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 59 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 60 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 61 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 62 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 63 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 64 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 65 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
; 66 "src/emain.c" 1
	nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

; 0 "" 2
	gid
	add sp,sp,#16
	movts status,r0
	movts iret,r1
	ldrd r0,[sp,#-1]
	rti
	.size	_wand_isr, .-_wand_isr
	.section	.text.startup,"ax",@progbits
	.balign 4
	.global	_main
_main:
	strd r4,[sp],#-6
	mov r1, %low(_e_irq_global_mask)
	strd lr,[sp,#2]
	str r32,[sp,#3]
	strd r6,[sp,#5]
	strd r8,[sp,#4]
	strd r10,[sp,#3]
	mov r0,#0
	movt r1, %high(_e_irq_global_mask)
	movfs r32,config
	jalr r1
	mov r1, %low(_wand_isr)
	mov r2, %low(_e_irq_attach)
	movt r1, %high(_wand_isr)
	mov r0,#8
	movt r2, %high(_e_irq_attach)
	jalr r2
	mov r2, %low(_e_irq_mask)
	mov r0,#8
	mov r1,#0
	movt r2, %high(_e_irq_mask)
	jalr r2
	mov r0, %low(#524288)
	movt r0, %high(#524288)
	gid
	movfs r1,config
	orr r1,r1,r0
	movts config,r1
	gie
	mov r0, %low(_e_group_config+28)
	mov r1, %low(_e_group_config+24)
	movt r0, %high(_e_group_config+28)
	movt r1, %high(_e_group_config+24)
	ldr r0,[r0]
	ldr r1,[r1]
	mov r9, %low(_e_wait)
	mov r8, %low(_e_reg_read)
	imul r1, r1, r0
	mov r0, %low(_e_group_config+32)
	movt r0, %high(_e_group_config+32)
	ldr r0,[r0]
	mov r15, %low(_state)
	mov r7, %low(_e_reg_write)
	add r0,r1,r0
	lsl fp,r0,#2
	mov r1, %low(#-1895825408)
	add r0,r0,#1
	movt r1, %high(#-1895825408)
	lsl r10,r0,#16
	mov r6, %low(#-9)
	mov r5, %low(#65536)
	mov r0, %low(#-917506)
	add fp,fp,r1
	mov r4,#0
	movt r9, %high(_e_wait)
	movt r8, %high(_e_reg_read)
	movt r15, %high(_state)
	movt r7, %high(_e_reg_write)
	movt r6, %high(#-9)
	movt r5, %high(#65536)
	movt r0, %high(#-917506)
	gid
	movfs r1,config
	eor r1,r1,r32
	and r1,r1,r0
	eor r1,r1,r32
	movts config,r1
	gie
.L3:
	str r4,[fp]
	mov r0,#0
	mov r1,r10
	jalr r9
; 37 "src/emain.c" 1
	wand
; 0 "" 2
; 38 "src/emain.c" 1
	idle
; 0 "" 2
	mov r0, %low(#984068)
	movt r0, %high(#984068)
	jalr r8
	and r2,r0,r6
	mov r0, %low(#984128)
	movt r0, %high(#984128)
	mov r1,r2
	str r2,[r15]
	jalr r7
	add r4,r4,#1
	sub r0,r4,r5
	bne .L3
	mov r0,#0
	ldrd r4,[sp,#6]
	ldrd r6,[sp,#5]
	ldrd r8,[sp,#4]
	ldrd r10,[sp,#3]
	ldrd lr,[sp,#2]
	ldr r32,[sp,#3]
	add sp,sp,#48
	rts
	.size	_main, .-_main
	.comm	_state,4,4
	.ident	"GCC: (Epiphany toolchain 20120120 (built 20130102)) 4.7.0 20120120 (experimental)"
