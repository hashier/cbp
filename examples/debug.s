	.text
	.p2align 4,,15
.globl setNumber
	.type	setNumber, @function
setNumber:
        std
.L5:
	movq	%rsi, %rax
	andl	$15, %eax
	movzbl	chars(%rax), %eax
        stosb
	shrq	$4, %rsi
	jne	.L5
	ret
	.size	setNumber, .-setNumber
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
chars:
        .ascii  "0123456789ABCDEF"
buffer:
	.ascii	"        rax: 0x0000000000000000\n"
        .ascii  "        rbx: 0x0000000000000000\n"
        .ascii  "        rcx: 0x0000000000000000\n"
        .ascii  "        rdx: 0x0000000000000000\n"
        .ascii  "        rsi: 0x0000000000000000\n"
	.ascii	"        rdi: 0x0000000000000000\n"
        .ascii  "        rbp: 0x0000000000000000\n"
        .ascii  "        rsp: 0x0000000000000000\n"
        .ascii  "stack[0x00]: 0x0000000000000000\n"
        .ascii  "stack[0x08]: 0x0000000000000000\n"
        .ascii  "stack[0x10]: 0x0000000000000000\n"
        .ascii  "stack[0x18]: 0x0000000000000000\n"
        .ascii  "stack[0x20]: 0x0000000000000000\n"
	.text
	.p2align 4,,15
.globl dump
	.type	dump, @function
dump:
        pushq   %rsp
        pushq   %rbp
        pushq   %rdi
        pushq   %rsi
        pushq   %rdx
        pushq   %rcx
        pushq   %rbx
        pushq   %rax
	subq	$456, %rsp
	movl	$buffer, %esi
	movl	$52, %ecx
	movq	%rsp, %rdi
	movl	$68, %edx
	rep movsq
        movq    $13, %rcx
        movq    $30, %rdx
        movq    $456, %rbx
print:
        leaq    (%rsp,%rdx), %rdi
        movq    (%rsp,%rbx), %rsi
        call setNumber
        addq    $8, %rbx
        addq    $32, %rdx
        loop print
        movq    $1, %rdi
	movl	$416, %edx
        movq    %rsp, %rsi
	call	write
	xorl	%edi, %edi
	call	exit
	.size	dump, .-dump
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	subq	$8, %rsp
        movq    $0xCAFEFACEDEADBEEF, %rax
        movq    $0xAAAAAAAAAAAAAAAA, %rbx
	jmp	dump
	addq	$8, %rsp
	ret
	.size	main, .-main
