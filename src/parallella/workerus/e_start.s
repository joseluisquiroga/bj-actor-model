;
;
;*************************************************************
;
;This file is part of messaging-cells.
;
;messaging-cells is free software: you can redistribute it and/or modify
;it under the terms of the version 3 of the GNU General Public 
;License as published by the Free Software Foundation.
;
;messaging-cells is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License
;along with messaging-cells.  If not, see <http://www.gnu.org/licenses/>.
;
;------------------------------------------------------------
;
;Copyright (C) 2017-2018. QUIROGA BELTRAN, Jose Luis.
;Id (cedula): 79523732 de Bogota - Colombia.
;See https://messaging-cells.github.io/
;
;messaging-cells is free software thanks to The Glory of Our Lord 
;	Yashua Melej Hamashiaj.
;Our Resurrected and Living, both in Body and Spirit, 
;	Prince of Peace.
;
;------------------------------------------------------------
;
	.file	"e_start.s"
	.section IVT_RESET, "ax",@progbits
	.balign 4
	.global	_start ;irq0_entry
_start:
	b normal_start
;
	.section ivt_entry_software_exception, "ax",@progbits
	.balign 4
	.global	irq1_entry
irq1_entry:
	b mck_software_exception_handler
;
	.section ivt_entry_page_miss, "ax",@progbits
	.balign 4
	.global	irq2_entry
irq2_entry:
	b mck_page_miss_handler
;
	.section ivt_entry_timer0, "ax",@progbits
	.balign 4
	.global	irq3_entry
irq3_entry:
	b mck_timer0_handler
;
	.section .text
	.balign 4
	.global	normal_start
normal_start:
; WARNING !!!!! Your .text code MUST fit in the space you give it in the link script (CODE_SIZE)
; WARNING !!!!! ALWAYS use modules for inworkeru funcs. 
; WARNING !!!!! ALWAYS have LOW STACK consuming functions (use pointers to dynamic allocated structs). 
; WARNING !!!!! Kernel already uses about 5k in .text so CODE_SIZE MUST be bigger.
; WARNING !!!!! sp initialized at addr LD_WORKERU_STACK_TOP defined in the link script
	mov sp, %low(LD_WORKERU_STACK_TOP)
	movt sp, %high(LD_WORKERU_STACK_TOP)
	mov fp,0x0
	mov r0, #0x3ff
	movts imask, r0
	mov r0, %low(main_caller)
	movt r0, %high(main_caller)
	movts iret, r0
	rti
	.balign 4
	.global	main_caller
main_caller:
	mov r0, %low(main)
	movt r0, %high(main)
	jalr r0
	nop
	mov r0, #0x3ff
	movts imask, r0
	gid
	trap 0x3
	nop
infn_loop:
	b infn_loop
	nop
	rts
	rti
