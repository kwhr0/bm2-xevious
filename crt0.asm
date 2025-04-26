	.zp
	.export bp
	.export tmp1
	.export tmp2
	.export tmp3
	.export tmp4
	.export long
bp:	.word	0
tmp1:	.word	0
tmp2:	.word	0
tmp3:	.word	0
tmp4:	.word	0
long:	.word	0
	.word	0

	.code
	.export _exit
start:
	sei
	lds	#$ffef	; extended
	ldx	#interrupt
	stx	$fff8
	ldx	#__bss_size
	beq	nobss
	clra
	clrb
	subb	#<__bss_size
	sbca	#>__bss_size
	ldx	#__bss
clear_bss:
	clr	,x
	inx
	incb
	bne	clear_bss
	inca
	bne	clear_bss
nobss:
	jsr	_main
_exit:
	wai
	bra	_exit

	.export	_interruptCount
interrupt:
	inc	_interruptCount
	rti
	.bss
_interruptCount:	.byte	0


	.code
	.export	_clrs
_clrs:
gbase	.equ	$900
top	.equ	18
line	.equ	$20
xofs	.equ	10
height	.equ	136
	ldx	#gbase+line*top+xofs
	stx	@tmp1
	ldab	#height
l1:
	ldaa	#0
	staa	0,x
	staa	1,x
	staa	2,x
	staa	3,x
	staa	4,x
	staa	5,x
	staa	6,x
	staa	7,x
	staa	8,x
	staa	9,x
	staa	10,x
	staa	11,x
	ldaa	@tmp1+1
	adda	#$20
	staa	@tmp1+1
	ldaa	@tmp1
	adca	#0
	staa	@tmp1
	ldx	@tmp1
	decb
	bne	l1
	rts
