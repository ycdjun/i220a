	.text
	.globl get_parity
#edi contains n	
get_parity:

	testl %edi, %edi
	jpe even
	movl $0x0, %eax
	jmp ret
even:
	movl $0x1, %eax
ret:	
	ret
	
