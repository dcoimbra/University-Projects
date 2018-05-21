extern	readi
extern	printi
extern	prints
extern	println
extern	printd
extern	readd
segment	.text
align	4
global	argc:function
argc:
segment	.text
align	4
global	_main:function
_main:
	push	ebp
	mov	ebp, esp
	sub	esp, 0
	call	argc
	push	eax
	call	printi
	add	esp, 4
	call	println
	push	dword 0
	jmp	dword covfefeend
align	4
covfefeend:
	pop	eax
	leave
	ret
