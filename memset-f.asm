; original: chibicc-6800-v1/libc/__memset.s

;
;	void *memset(void *s,int c,size_t n)
;	{
;	  char *p = s;
;	  while (n--){
;	    *p++ = c;
;	  }
;	  return s;
;	}
;
;	stack:
;	+0 ret addr
;	+2 s
;	+4 c
;	+6 n
;
	.code
	.export	_memset
_memset:
	tsx
	ldx	6,x		; n==0 ?
	beq	_memset_ret
	pshb			; save s, access to the stack will add two.
	psha
	tsx
	ldab	9,x		; n
	ldaa	8,x
	cmpb	#1		; if b!=0 then a++
	sbca	#0
	inca
	staa	@tmp1
;
	ldaa	7,x		; c
;
	ldx	4,x		; s
;
_memset_loop:
	staa	0,x
	inx
	decb
	bne	_memset_loop
	dec	@tmp1
	bne	_memset_loop
;
	pula
	pulb
;
_memset_ret:
	jmp	__cleanup6
