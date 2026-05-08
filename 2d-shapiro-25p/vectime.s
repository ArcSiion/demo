	.file	"vectime.c"
	.text
	.p2align 4
	.globl	vectime
	.type	vectime, @function
vectime:
.LFB6470:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	movl	%ecx, %r13d
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$1472, %rsp
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rdi, 816(%rsp)
	movl	%edx, %edi
	movl	%edx, 1384(%rsp)
	movl	%esi, 612(%rsp)
	movq	%fs:40, %rax
	movq	%rax, 1464(%rsp)
	xorl	%eax, %eax
	leal	8(%rdx), %eax
	movslq	%eax, %rdx
	movl	%eax, 592(%rsp)
	leaq	0(,%rdx,8), %rax
	movq	%rax, 1232(%rsp)
	leal	4(%rsi), %eax
	cltq
	imulq	%rdx, %rax
	movq	%rax, %r15
	cmpl	$3, %edi
	jle	.L142
	leal	-4(%rdi), %eax
	movl	%esi, %r9d
	sarl	$2, %eax
	leal	8(,%rax,4), %eax
	movl	%eax, 708(%rsp)
	decl	%eax
	movl	%eax, 1388(%rsp)
	cmpl	$11, %r9d
	jg	.L3
.L274:
	testl	%r13d, %r13d
	jle	.L1
	movq	1232(%rsp), %rbx
	shrq	$3, %rbx
	testl	%r9d, %r9d
	jle	.L1
	movl	1384(%rsp), %ecx
	testl	%ecx, %ecx
	jle	.L1
	leaq	(%rbx,%rbx), %rdi
	leal	-1(%rcx), %eax
	movl	%ecx, %edx
	movq	%rbx, %r11
	andl	$-4, %ecx
	movq	%rdi, 1056(%rsp)
	salq	$5, %r11
	movq	%rbx, %rsi
	movl	%eax, 1000(%rsp)
	addq	%rdi, %rax
	addq	%rbx, %rdi
	movq	816(%rsp), %r14
	leaq	24(,%rax,8), %r10
	movl	%ecx, 888(%rsp)
	leal	2(%r9), %eax
	addl	$4, %ecx
	leaq	0(,%rdi,8), %r8
	shrl	$2, %edx
	movl	%ecx, 880(%rsp)
	salq	$4, %rsi
	movl	%eax, 1168(%rsp)
	salq	$5, %rdx
	vmovsd	.LC1(%rip), %xmm13
	movq	%rbx, %r9
	movq	%r11, 848(%rsp)
	vmovsd	.LC3(%rip), %xmm12
	movq	%r15, %rbx
	movq	%rsi, %r12
	vmovsd	.LC5(%rip), %xmm11
	vmovsd	.LC7(%rip), %xmm10
	movl	%r13d, 840(%rsp)
	movq	%rdi, 832(%rsp)
	vmovsd	.LC9(%rip), %xmm9
	movq	%r8, 824(%rsp)
	movq	%r10, 816(%rsp)
	movq	%rdx, 1184(%rsp)
	xorl	%edx, %edx
.L5:
	movl	%edx, %eax
	incl	%edx
	movq	1056(%rsp), %r11
	movq	$0, 1336(%rsp)
	andl	$1, %eax
	movq	$0, 1160(%rsp)
	vmovsd	.LC11(%rip), %xmm8
	movq	%r9, %r13
	negq	%rax
	vmovsd	.LC1(%rip), %xmm7
	vmovsd	.LC3(%rip), %xmm6
	movl	$2, 1388(%rsp)
	andq	%rbx, %rax
	vmovsd	.LC5(%rip), %xmm5
	movq	%rax, %rdi
	movq	%rax, 1072(%rsp)
	leaq	0(,%rax,8), %rax
	leaq	(%r14,%rax), %r15
	movq	%rax, 1360(%rsp)
	movl	%edx, %eax
	andl	$1, %eax
	negq	%rax
	andq	%rbx, %rax
	leaq	(%r14,%rax,8), %rsi
	movq	%rax, 1048(%rsp)
	movq	%rax, %rcx
	movq	816(%rsp), %rax
	movq	%rsi, 1176(%rsp)
	addq	%r15, %rax
	movq	%rax, 1152(%rsp)
	leaq	(%rdi,%r11), %rax
	leaq	0(,%rax,8), %rdi
	subq	%r9, %rax
	movq	%rdi, 1192(%rsp)
	leaq	0(,%rax,8), %rdi
	movq	%rdi, 1200(%rsp)
	movq	832(%rsp), %rdi
	movq	%rax, 1024(%rsp)
	addq	%rdi, %rax
	movq	%rax, 1032(%rsp)
	leaq	0(,%rax,8), %rdi
	subq	%r9, %rax
	movq	%rax, 1040(%rsp)
	salq	$3, %rax
	movq	%rax, 1328(%rsp)
	leaq	(%rcx,%r11), %rax
	salq	$3, %rax
	movq	%rdi, 1368(%rsp)
	movq	%rax, 1144(%rsp)
	addq	%r14, %rax
	movq	%rax, 992(%rsp)
	leaq	(%r12,%r15), %rax
	movq	%rax, 896(%rsp)
	movq	1232(%rsp), %rax
	addq	%r15, %rax
	movq	%rax, 928(%rsp)
	movq	824(%rsp), %rax
	addq	%r15, %rax
	movq	%rax, 960(%rsp)
	movq	848(%rsp), %rax
	addq	%r15, %rax
	movq	%rax, 1008(%rsp)
	leaq	(%rsi,%r12), %rax
	movq	%rax, 1016(%rsp)
	vmovsd	.LC7(%rip), %xmm4
	movq	%rbx, 872(%rsp)
	movq	%r12, 864(%rsp)
	movl	%edx, 856(%rsp)
	.p2align 4,,10
	.p2align 3
.L17:
	movl	1388(%rsp), %eax
	movslq	%eax, %r9
	incl	%eax
	cmpl	$1, 1384(%rsp)
	movl	%eax, 1388(%rsp)
	leal	-1(%r9), %eax
	movl	%eax, 1376(%rsp)
	leal	-2(%r9), %eax
	movl	%eax, 1352(%rsp)
	leal	2(%r9), %eax
	movl	%eax, 1344(%rsp)
	je	.L14
	movq	1336(%rsp), %rdi
	movq	1144(%rsp), %rax
	leaq	32(%rax,%rdi), %rax
	movq	1192(%rsp), %rdi
	movq	%rax, %rdx
	leaq	24(%rdi), %rcx
	movq	1200(%rsp), %rdi
	subq	%rcx, %rdx
	movq	%rax, %rcx
	cmpq	$48, %rdx
	leaq	24(%rdi), %rsi
	movq	1360(%rsp), %rdi
	seta	%dl
	subq	%rsi, %rcx
	cmpq	$48, %rcx
	leaq	24(%rdi), %rsi
	movq	1368(%rsp), %rdi
	seta	%cl
	andl	%edx, %ecx
	movq	%rax, %rdx
	subq	%rsi, %rdx
	leaq	24(%rdi), %rsi
	cmpq	$48, %rdx
	seta	%dl
	andl	%ecx, %edx
	movq	%rax, %rcx
	subq	%rsi, %rcx
	cmpq	$48, %rcx
	seta	%cl
	testb	%cl, %dl
	je	.L14
	movq	1328(%rsp), %rdi
	leaq	24(%rdi), %rdx
	subq	%rdx, %rax
	cmpq	$48, %rax
	jbe	.L14
	cmpl	$2, 1000(%rsp)
	jbe	.L143
	leaq	32(%r14), %rcx
	movq	1192(%rsp), %rbx
	movq	%rdi, %r10
	leaq	24(%r14), %rdx
	leaq	(%rcx,%rdi), %rdi
	movq	1200(%rsp), %r11
	movq	1368(%rsp), %r12
	movq	%rdi, 1112(%rsp)
	movq	1360(%rsp), %rdi
	leaq	(%rdx,%rbx), %rsi
	leaq	(%rcx,%rbx), %rax
	vbroadcastsd	.LC1(%rip), %ymm17
	vbroadcastsd	.LC3(%rip), %ymm16
	movq	%rsi, 1120(%rsp)
	leaq	(%rcx,%rdi), %rsi
	vbroadcastsd	.LC5(%rip), %ymm15
	vbroadcastsd	.LC7(%rip), %ymm14
	movq	%rax, 1096(%rsp)
	movq	%rsi, 1136(%rsp)
	leaq	(%rcx,%r11), %rax
	leaq	16(%r14), %rsi
	addq	%r12, %rcx
	movq	%rax, 1104(%rsp)
	leaq	40(%r14), %rax
	movq	%rcx, 1216(%rsp)
	leaq	(%rsi,%rbx), %rcx
	leaq	(%rax,%rbx), %r8
	movq	%rcx, 1224(%rsp)
	leaq	48(%r14), %rcx
	addq	%rcx, %rbx
	movq	%r8, 1128(%rsp)
	movq	%r10, %r8
	leaq	(%rdx,%r10), %r10
	movq	%rbx, 1240(%rsp)
	leaq	(%rdx,%r11), %rbx
	movq	%r10, 1248(%rsp)
	movq	%r11, %r10
	leaq	(%rax,%r11), %r11
	movq	%r11, 1256(%rsp)
	leaq	(%rax,%r8), %r11
	movq	%r12, %r8
	leaq	(%rsi,%r12), %r12
	movq	%r11, 1264(%rsp)
	leaq	(%rsi,%rdi), %r11
	movq	%r12, 1272(%rsp)
	leaq	(%rdi,%rcx), %r12
	movq	1328(%rsp), %rdi
	movq	%r12, 1280(%rsp)
	leaq	(%r8,%rcx), %r12
	movq	%r10, %r8
	leaq	(%rsi,%r10), %r10
	movq	%r12, 1288(%rsp)
	leaq	(%rsi,%rdi), %r12
	movq	%r12, 1296(%rsp)
	leaq	(%r8,%rcx), %r12
	addq	%rdi, %rcx
	movq	1360(%rsp), %rdi
	movq	%r12, 1304(%rsp)
	leaq	(%rdx,%rdi), %r8
	leaq	(%rax,%rdi), %r12
	movq	1368(%rsp), %rdi
	movq	%r12, 1312(%rsp)
	leaq	(%rdi,%rax), %r12
	addq	%rdi, %rdx
	movq	992(%rsp), %rax
	movq	%r12, 1320(%rsp)
	movq	1336(%rsp), %r12
	vbroadcastsd	.LC9(%rip), %ymm3
	vbroadcastsd	.LC11(%rip), %ymm2
	movq	%rdx, 1208(%rsp)
	leaq	32(%r12,%rax), %rsi
	movq	%r15, 1088(%rsp)
	movq	1136(%rsp), %r12
	xorl	%eax, %eax
	movq	1104(%rsp), %rdi
	movq	1096(%rsp), %r15
	movq	%r13, 1080(%rsp)
	movl	%r9d, 1136(%rsp)
	movq	1120(%rsp), %r13
	movq	1128(%rsp), %r9
	movq	%r14, 1128(%rsp)
	movq	1112(%rsp), %r14
	.p2align 4,,10
	.p2align 3
.L7:
	vmovupd	(%r14,%rax), %ymm1
	movq	1216(%rsp), %rdx
	vaddpd	(%rdi,%rax), %ymm1, %ymm0
	vmovupd	(%rdx,%rax), %ymm1
	movq	1224(%rsp), %rdx
	vaddpd	(%r12,%rax), %ymm1, %ymm1
	vaddpd	0(%r13,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1240(%rsp), %rdx
	vaddpd	(%r9,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1248(%rsp), %rdx
	vmulpd	%ymm17, %ymm0, %ymm0
	vfmadd231pd	(%r15,%rax), %ymm16, %ymm0
	vfmadd132pd	%ymm15, %ymm0, %ymm1
	vmovupd	(%rdx,%rax), %ymm0
	movq	1256(%rsp), %rdx
	vaddpd	(%rbx,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1264(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1272(%rsp), %rdx
	vfmadd132pd	%ymm14, %ymm1, %ymm0
	vmovupd	(%rdx,%rax), %ymm1
	movq	1280(%rsp), %rdx
	vaddpd	(%r11,%rax), %ymm1, %ymm1
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1288(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1296(%rsp), %rdx
	vfmadd132pd	%ymm3, %ymm0, %ymm1
	vmovupd	(%rdx,%rax), %ymm0
	movq	1304(%rsp), %rdx
	vaddpd	(%r10,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	vaddpd	(%rcx,%rax), %ymm0, %ymm0
	vaddpd	(%r8,%rax), %ymm0, %ymm0
	movq	1312(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1208(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1320(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1184(%rsp), %rdx
	vfmadd132pd	%ymm2, %ymm1, %ymm0
	vmovupd	%ymm0, (%rsi,%rax)
	addq	$32, %rax
	cmpq	%rdx, %rax
	jne	.L7
	movl	1384(%rsp), %r11d
	movl	888(%rsp), %edx
	movq	1088(%rsp), %r15
	movq	1080(%rsp), %r13
	movslq	1136(%rsp), %r9
	movq	1128(%rsp), %r14
	cmpl	%edx, %r11d
	je	.L12
	subl	%edx, %r11d
	movl	880(%rsp), %eax
	cmpl	$1, %r11d
	je	.L9
	movl	%eax, 1320(%rsp)
.L6:
	movq	1160(%rsp), %r10
	movq	1056(%rsp), %rax
	movq	1024(%rsp), %rdi
	movq	1040(%rsp), %rcx
	leaq	(%r10,%rax), %r8
	movq	1072(%rsp), %rbx
	leaq	(%r10,%rdi), %rax
	addq	%rdx, %r8
	leaq	4(%rdx,%rax), %r12
	leaq	(%r10,%rcx), %rax
	vmovupd	(%r14,%r12,8), %xmm0
	leaq	4(%rdx,%rax), %rcx
	leaq	4(%rbx,%r8), %rsi
	movq	%rbx, %rax
	movq	1032(%rsp), %rbx
	addq	%r10, %rax
	vmovupd	(%r14,%rsi,8), %xmm3
	leaq	0(,%r12,8), %rdi
	vaddpd	(%r14,%rcx,8), %xmm0, %xmm0
	leaq	4(%rdx,%rax), %rax
	addq	%rbx, %r10
	salq	$3, %rax
	movq	1048(%rsp), %rbx
	leaq	4(%rdx,%r10), %rdx
	vaddpd	-8(%r14,%rsi,8), %xmm0, %xmm0
	salq	$3, %rdx
	leaq	4(%rbx,%r8), %r8
	vaddpd	8(%r14,%rsi,8), %xmm0, %xmm0
	vmulpd	.LC1(%rip){1to2}, %xmm0, %xmm0
	vfmadd231pd	.LC3(%rip){1to2}, %xmm3, %xmm0
	vmovupd	(%r14,%rdx), %xmm3
	vaddpd	(%r14,%rax), %xmm3, %xmm1
	vmovupd	-8(%r14,%rcx,8), %xmm3
	vaddpd	-16(%r14,%rsi,8), %xmm1, %xmm1
	vaddpd	16(%r14,%rsi,8), %xmm1, %xmm1
	vfmadd132pd	.LC5(%rip){1to2}, %xmm0, %xmm1
	vaddpd	-8(%r14,%rdi), %xmm3, %xmm0
	vmovupd	-16(%r14,%rdx), %xmm3
	vaddpd	8(%r14,%rdi), %xmm0, %xmm0
	vaddpd	8(%r14,%rcx,8), %xmm0, %xmm0
	vfmadd132pd	.LC7(%rip){1to2}, %xmm1, %xmm0
	vaddpd	-16(%r14,%rax), %xmm3, %xmm1
	vmovupd	-16(%r14,%rcx,8), %xmm3
	vaddpd	16(%r14,%rax), %xmm1, %xmm1
	vaddpd	16(%r14,%rdx), %xmm1, %xmm1
	vfmadd132pd	.LC9(%rip){1to2}, %xmm0, %xmm1
	vaddpd	-16(%r14,%rdi), %xmm3, %xmm0
	vaddpd	16(%r14,%rdi), %xmm0, %xmm0
	vaddpd	16(%r14,%rcx,8), %xmm0, %xmm0
	vaddpd	-8(%r14,%rax), %xmm0, %xmm0
	vaddpd	8(%r14,%rax), %xmm0, %xmm0
	vaddpd	-8(%r14,%rdx), %xmm0, %xmm0
	vaddpd	8(%r14,%rdx), %xmm0, %xmm0
	vfmadd132pd	.LC11(%rip){1to2}, %xmm1, %xmm0
	vmovupd	%xmm0, (%r14,%r8,8)
	testb	$1, %r11b
	je	.L12
	movl	1320(%rsp), %eax
	andl	$-2, %r11d
	addl	%r11d, %eax
.L9:
	movslq	1388(%rsp), %r11
	movslq	1376(%rsp), %rdi
	movslq	%eax, %rbx
	imulq	%r13, %r9
	leal	-1(%rax), %ecx
	leal	1(%rax), %edx
	movslq	1344(%rsp), %r10
	imulq	%r13, %r11
	movslq	%ecx, %rcx
	movslq	%edx, %rdx
	imulq	%r13, %rdi
	leaq	(%rbx,%r9), %r12
	imulq	%r13, %r10
	leaq	(%rbx,%r11), %r8
	vmovsd	(%r15,%r8,8), %xmm0
	leaq	(%rbx,%rdi), %rsi
	leal	-2(%rax), %r8d
	addl	$2, %eax
	movl	%eax, 1376(%rsp)
	leaq	(%rbx,%r10), %rax
	movslq	%r8d, %r8
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	leaq	(%rcx,%r9), %rsi
	vmovsd	(%r15,%rax,8), %xmm1
	movslq	1376(%rsp), %rax
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	leaq	(%rdx,%r9), %rsi
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	movslq	1352(%rsp), %rsi
	imulq	%r13, %rsi
	vmulsd	%xmm13, %xmm0, %xmm0
	addq	%rsi, %rbx
	vaddsd	(%r15,%rbx,8), %xmm1, %xmm1
	leaq	(%r8,%r9), %rbx
	addq	%rax, %r9
	vfmadd231sd	(%r15,%r12,8), %xmm12, %xmm0
	vaddsd	(%r15,%rbx,8), %xmm1, %xmm1
	leaq	(%rcx,%r11), %rbx
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%rcx,%rdi), %r9
	vfmadd132sd	%xmm11, %xmm0, %xmm1
	vmovsd	(%r15,%rbx,8), %xmm0
	leaq	(%r8,%r10), %rbx
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%rdi), %r9
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%r11), %r9
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%r8,%rsi), %r9
	vfmadd132sd	%xmm10, %xmm1, %xmm0
	vmovsd	(%r15,%rbx,8), %xmm1
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%rax,%rsi), %r9
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%rax,%r10), %r9
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%r8,%r11), %r9
	addq	%rdi, %r8
	addq	%rax, %rdi
	addq	%r11, %rax
	vfmadd132sd	%xmm9, %xmm0, %xmm1
	vmovsd	(%r15,%r9,8), %xmm0
	vaddsd	(%r15,%r8,8), %xmm0, %xmm0
	vaddsd	(%r15,%rdi,8), %xmm0, %xmm0
	vaddsd	(%r15,%rax,8), %xmm0, %xmm0
	leaq	(%rcx,%rsi), %rax
	addq	%rdx, %rsi
	addq	%r10, %rcx
	addq	%r10, %rdx
	vaddsd	(%r15,%rax,8), %xmm0, %xmm0
	movq	1176(%rsp), %rax
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	vaddsd	(%r15,%rcx,8), %xmm0, %xmm0
	vaddsd	(%r15,%rdx,8), %xmm0, %xmm0
	vfmadd132sd	%xmm8, %xmm1, %xmm0
	vmovsd	%xmm0, (%rax,%r12,8)
.L12:
	movq	1232(%rsp), %rax
	movl	1388(%rsp), %edi
	addq	%r13, 1160(%rsp)
	addq	%rax, 1152(%rsp)
	addq	%rax, 1192(%rsp)
	addq	%rax, 1200(%rsp)
	addq	%rax, 1360(%rsp)
	addq	%rax, 1368(%rsp)
	addq	%rax, 1328(%rsp)
	addq	%rax, 1336(%rsp)
	cmpl	%edi, 1168(%rsp)
	jne	.L17
	movl	856(%rsp), %edx
	movq	872(%rsp), %rbx
	movq	%r13, %r9
	movq	864(%rsp), %r12
	cmpl	%edx, 840(%rsp)
	jne	.L5
	vzeroupper
.L1:
	movq	1464(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L273
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L142:
	.cfi_restore_state
	movl	%esi, %r9d
	movl	$4, 708(%rsp)
	movl	$3, 1388(%rsp)
	cmpl	$11, %r9d
	jle	.L274
.L3:
	movslq	1384(%rsp), %rax
	movl	$1, %ebx
	leaq	1400(%rsp), %rdi
	movl	$64, %esi
	testl	%eax, %eax
	cmovg	%rax, %rbx
	leaq	(%rbx,%rbx,2), %rdx
	salq	$6, %rdx
	call	posix_memalign@PLT
	movq	$0, 376(%rsp)
	testl	%eax, %eax
	jne	.L19
	movq	1400(%rsp), %rax
	movq	%rax, 376(%rsp)
.L19:
	movq	376(%rsp), %rsi
	salq	$5, %rbx
	leaq	(%rsi,%rbx), %rax
	leaq	(%rax,%rbx), %rdi
	movq	%rax, 128(%rsp)
	leaq	(%rdi,%rbx), %rcx
	vmovq	%rdi, %xmm6
	movq	%rdi, 120(%rsp)
	vpinsrq	$1, %rcx, %xmm6, %xmm0
	vmovq	%rsi, %xmm6
	movq	%rcx, 112(%rsp)
	vpinsrq	$1, %rax, %xmm6, %xmm1
	leaq	(%rcx,%rbx), %rax
	leaq	(%rax,%rbx), %rdi
	movq	%rax, 392(%rsp)
	vinserti128	$0x1, %xmm0, %ymm1, %ymm8
	movq	%rax, 1440(%rsp)
	leal	-3(%r13), %eax
	movq	%rdi, 384(%rsp)
	movq	%rdi, 1448(%rsp)
	movl	%eax, 244(%rsp)
	vmovdqa	%ymm8, 1408(%rsp)
	cmpl	$3, %r13d
	jle	.L146
	movl	1388(%rsp), %edi
	movl	1384(%rsp), %edx
	vmovdqa	%ymm8, %ymm1
	movq	1232(%rsp), %r9
	movq	816(%rsp), %rbx
	leal	-3(%rdi), %ecx
	leaq	0(,%r15,8), %rdi
	movl	%ecx, 1224(%rsp)
	leal	3(%rdx), %ecx
	movq	%r9, %r14
	movq	%r9, %r11
	movl	%ecx, 1024(%rsp)
	movl	612(%rsp), %ecx
	shrq	$3, %r14
	shrq	%r11
	movq	%r14, %r8
	movq	%r14, 1272(%rsp)
	movq	%r9, %r14
	leaq	(%rbx,%rdi), %r12
	leal	1(%rcx), %esi
	shrq	$2, %r14
	movq	%r11, 24(%rsp)
	movl	%esi, 608(%rsp)
	leal	-1(%rdx), %esi
	movq	%r14, %rcx
	leaq	(%r14,%rsi), %rax
	movq	%r14, 776(%rsp)
	movq	%r11, %r14
	movq	%rcx, %r11
	salq	$3, %rax
	leaq	(%rcx,%r8), %rcx
	movq	%r12, 616(%rsp)
	movq	%rax, 416(%rsp)
	movl	%edx, %eax
	leaq	0(,%rcx,8), %r10
	shrl	$2, %eax
	movq	%r10, 432(%rsp)
	salq	$5, %rax
	movl	%esi, 808(%rsp)
	movq	%rax, 1264(%rsp)
	movl	%edx, %eax
	andl	$-4, %eax
	movl	%eax, 788(%rsp)
	addl	$4, %eax
	movl	%eax, 704(%rsp)
	movq	%r8, %rax
	salq	$4, %r8
	leaq	0(,%rax,8), %rdx
	movq	%rax, %r10
	movq	%r8, 408(%rsp)
	movq	%rax, %r8
	leaq	(%rdx,%rax), %rax
	salq	$5, %r8
	subq	%r10, %rdx
	movq	%rax, 192(%rsp)
	leaq	(%rbx,%rax,8), %rax
	movq	%rcx, %rbx
	movq	%rax, 344(%rsp)
	movq	%rcx, %rax
	salq	$4, %rax
	movq	%rbx, 688(%rsp)
	leaq	(%rbx,%rbx), %rbx
	movq	%rax, %rcx
	leaq	(%r12,%rax), %rax
	movq	%r8, 400(%rsp)
	movq	%rax, 200(%rsp)
	movq	344(%rsp), %rax
	subq	%rcx, %rax
	movq	%rax, 336(%rsp)
	movl	1384(%rsp), %eax
	subl	$4, %eax
	shrl	$2, %eax
	incl	%eax
	salq	$5, %rax
	movq	%rax, 1376(%rsp)
	leaq	(%r12,%r9), %rax
	movq	%rax, 224(%rsp)
	leaq	(%r14,%r10), %rax
	movq	%rax, 680(%rsp)
	addq	%rax, %rax
	movq	%rax, 184(%rsp)
	addq	%r10, %rax
	movq	%rax, 176(%rsp)
	movq	%rbx, 168(%rsp)
	movq	224(%rsp), %rbx
	movq	%rdx, 160(%rsp)
	movq	%r10, %rdx
	negq	%r10
	movq	%r10, %rax
	movq	336(%rsp), %r10
	leaq	(%r10,%rax,8), %r10
	movq	%r10, 216(%rsp)
	movq	%rdx, %r10
	salq	$6, %rdx
	leaq	(%rbx,%rdx), %rax
	movq	%rax, 64(%rsp)
	movq	216(%rsp), %rbx
	addq	%rbx, %r8
	movq	%r8, 56(%rsp)
	movq	%r8, %rbx
	movq	%rax, %r8
	addq	%r9, %rax
	subq	%rcx, %r8
	movq	%rax, 40(%rsp)
	addq	%r9, %rbx
	movq	%r8, 48(%rsp)
	addq	%r9, %r8
	movq	%r8, 32(%rsp)
	movl	1388(%rsp), %r8d
	movq	%rbx, 208(%rsp)
	movq	816(%rsp), %rbx
	leal	-13(%r8), %eax
	shrl	$2, %eax
	leal	0(,%rax,4), %r8d
	movq	%r8, 600(%rsp)
	leal	12(,%rax,4), %r8d
	movq	192(%rsp), %rax
	movl	%r8d, 632(%rsp)
	movslq	708(%rsp), %r8
	addq	%r8, %rax
	movq	%r8, 536(%rsp)
	leaq	(%rbx,%rax,8), %rbx
	movq	%rbx, 528(%rsp)
	movq	168(%rsp), %rbx
	leaq	(%rbx,%r8), %rax
	leaq	(%r12,%rax,8), %rbx
	movq	688(%rsp), %rax
	movq	%rbx, 520(%rsp)
	movq	816(%rsp), %rbx
	addq	%r8, %rax
	leaq	(%rbx,%rax,8), %rbx
	movq	184(%rsp), %rax
	movq	%rbx, 512(%rsp)
	leaq	(%r12,%r8,8), %rbx
	movq	%rbx, 504(%rsp)
	movq	816(%rsp), %rbx
	addq	%r8, %rax
	leaq	(%rbx,%rax,8), %rax
	movq	%rax, 496(%rsp)
	movq	160(%rsp), %rax
	addq	%r8, %rax
	leaq	(%r12,%rax,8), %rax
	movq	%rax, 488(%rsp)
	leaq	(%r8,%r14), %rax
	movq	%rbx, %r14
	leaq	(%rbx,%rax,8), %rbx
	leaq	(%r10,%r8), %rax
	movq	%rbx, 480(%rsp)
	leaq	(%r12,%rax,8), %rbx
	movq	%rbx, 472(%rsp)
	movq	176(%rsp), %rbx
	leaq	(%rbx,%r8), %rax
	leaq	(%r14,%rax,8), %rbx
	leaq	(%r9,%r8), %rax
	movq	%rbx, 464(%rsp)
	leaq	(%r12,%rax,8), %rbx
	movq	680(%rsp), %rax
	movq	%rbx, 456(%rsp)
	movl	612(%rsp), %ebx
	addq	%r8, %rax
	leaq	(%r14,%rax,8), %rax
	movq	%rax, 448(%rsp)
	leaq	(%r8,%r11), %rax
	salq	$5, %r8
	leaq	(%r12,%rax,8), %r11
	movq	%r8, 576(%rsp)
	movq	%r14, %r8
	imulq	$88, %r10, %rax
	movq	%r11, 440(%rsp)
	leaq	(%rax,%r12), %r11
	imulq	$-40, %r10, %rax
	movq	%r11, 552(%rsp)
	leaq	(%rdx,%r14), %r11
	movq	%r12, %rdx
	movq	%r11, 544(%rsp)
	subq	%rax, %rdx
	movq	688(%rsp), %rax
	movq	%rdx, 584(%rsp)
	salq	$5, %rax
	leaq	(%rax,%r14), %r14
	imulq	$104, %r10, %rax
	movq	%r14, 568(%rsp)
	leaq	(%rax,%r8), %r14
	leal	-11(%rbx), %eax
	leaq	9(%rax), %rdx
	movq	%r14, 560(%rsp)
	imulq	%r10, %rdx
	leaq	(%r8,%rdx,8), %r14
	leaq	6(%rax), %rdx
	imulq	%r10, %rdx
	movq	%r14, 104(%rsp)
	leaq	(%r12,%rdx,8), %r11
	leaq	3(%rax), %rdx
	imulq	%r10, %rdx
	movq	%r11, 96(%rsp)
	imulq	%r10, %rax
	leaq	(%r8,%rdx,8), %r11
	leal	-10(%rbx), %edx
	leaq	(%r12,%rax,8), %r14
	leal	-6(%rbx), %eax
	movq	%r11, 88(%rsp)
	cmpl	%eax, %edx
	movq	%r14, 80(%rsp)
	leaq	1408(%rsp), %r14
	leal	-9(%rbx), %edx
	setle	%al
	movq	%r14, 368(%rsp)
	subq	$8, %rdi
	salq	$3, %rsi
	movzbl	%al, %eax
	movq	%rdi, 232(%rsp)
	movl	592(%rsp), %edi
	xorl	%r12d, %r12d
	salq	$5, %rax
	movl	%edx, 636(%rsp)
	leaq	1416(%rsp,%rax), %r14
	movl	%ebx, %eax
	movq	%rsi, 424(%rsp)
	imulq	%r10, %rax
	movq	%r14, 72(%rsp)
	movq	%rax, 256(%rsp)
	leaq	0(,%rax,8), %r14
	leaq	10(%rdx), %rax
	imulq	%r10, %rax
	movq	%r14, 264(%rsp)
	movq	%rax, 272(%rsp)
	leaq	0(,%rax,8), %r14
	leaq	7(%rdx), %rax
	movl	1384(%rsp), %edx
	imulq	%r10, %rax
	movq	%r14, 280(%rsp)
	leal	7(%rdx), %esi
	movl	%esi, 240(%rsp)
	movq	%rax, 248(%rsp)
	leaq	0(,%rax,8), %r14
	movl	%edi, %eax
	andl	$-4, %edi
	shrl	$2, %eax
	movq	%r14, 320(%rsp)
	salq	$5, %rax
	movq	%rax, 136(%rsp)
	leaq	(%r15,%r10), %rax
	leaq	0(,%rax,8), %rdx
	movl	%edi, 364(%rsp)
	leaq	1(%r10), %rdi
	leaq	8(%r9), %rax
	movq	%rdi, 624(%rsp)
	movq	%rdx, %rdi
	subq	%rax, %rdi
	movq	208(%rsp), %rax
	movq	%r15, 672(%rsp)
	movq	%rdi, 352(%rsp)
	subq	%rcx, %rax
	movl	%r13d, 4(%rsp)
	movq	%rax, 8(%rsp)
	leaq	(%r8,%rdx), %rax
	movq	%rax, 16(%rsp)
	movl	%esi, %eax
	movq	%rax, 152(%rsp)
	leaq	1(%r10,%rax), %rax
	salq	$3, %rax
	movq	%rax, 144(%rsp)
	leal	-12(%rbx), %eax
	movq	1376(%rsp), %rbx
	movl	%eax, 596(%rsp)
.L21:
	leal	3(%r12), %esi
	movl	$11, 1128(%rsp)
	movq	816(%rsp), %r15
	movl	%r12d, %edx
	movl	%r12d, 864(%rsp)
	movq	%rbx, 856(%rsp)
	movl	%esi, 872(%rsp)
.L124:
	movl	872(%rsp), %eax
	subl	%edx, %eax
	testl	%eax, %eax
	jle	.L275
	movl	%edx, %eax
	movq	672(%rsp), %rdi
	incl	%edx
	movl	1384(%rsp), %r8d
	andl	$1, %eax
	negq	%rax
	andq	%rdi, %rax
	movq	%rax, 1040(%rsp)
	movq	%rax, %rcx
	leaq	0(,%rax,8), %rax
	movq	%rax, 1360(%rsp)
	leaq	(%r15,%rax), %r14
	movl	%edx, %eax
	andl	$1, %eax
	negq	%rax
	andq	%rdi, %rax
	leaq	(%r15,%rax,8), %rsi
	movq	%rax, 1048(%rsp)
	movq	%rax, %rdi
	movq	%rsi, 1160(%rsp)
	testl	%r8d, %r8d
	jle	.L29
	movq	416(%rsp), %rax
	movq	776(%rsp), %rbx
	movq	$0, 1328(%rsp)
	movq	1272(%rsp), %r13
	vmovsd	.LC11(%rip), %xmm10
	movq	$0, 1144(%rsp)
	leaq	24(%r14,%rax), %rax
	vmovsd	.LC1(%rip), %xmm9
	vmovsd	.LC3(%rip), %xmm8
	movl	$2, 1376(%rsp)
	vmovsd	.LC5(%rip), %xmm7
	vmovsd	.LC7(%rip), %xmm6
	movq	%rax, 1136(%rsp)
	leaq	(%rcx,%rbx), %rax
	leaq	0(,%rax,8), %rcx
	subq	%r13, %rax
	movl	%edx, 880(%rsp)
	movq	%rcx, 1176(%rsp)
	leaq	0(,%rax,8), %rcx
	movq	%rcx, 1168(%rsp)
	movq	688(%rsp), %rcx
	movq	%rax, 1032(%rsp)
	addq	%rcx, %rax
	leaq	0(,%rax,8), %rcx
	movq	%rax, 1016(%rsp)
	subq	%r13, %rax
	movq	%rax, 1008(%rsp)
	salq	$3, %rax
	movq	%rax, 1152(%rsp)
	leaq	(%rdi,%rbx), %rax
	salq	$3, %rax
	movq	%rcx, 1352(%rsp)
	movq	%rax, 1120(%rsp)
	addq	%r15, %rax
	movq	%rax, 888(%rsp)
	movq	408(%rsp), %rax
	leaq	(%r14,%rax), %rdi
	addq	%rsi, %rax
	movq	%rdi, 960(%rsp)
	movq	1232(%rsp), %rdi
	movq	%rax, 992(%rsp)
	addq	%r14, %rdi
	movq	%rdi, 928(%rsp)
	movq	432(%rsp), %rdi
	addq	%r14, %rdi
	movq	%rdi, 896(%rsp)
	movq	400(%rsp), %rdi
	addq	%r14, %rdi
	movq	%rdi, 1000(%rsp)
	.p2align 4,,10
	.p2align 3
.L34:
	movl	1376(%rsp), %eax
	movl	%eax, %r11d
	incl	%eax
	cmpl	$1, 1384(%rsp)
	movl	%eax, 1376(%rsp)
	leal	-1(%r11), %eax
	movl	%eax, 1368(%rsp)
	leal	-2(%r11), %eax
	movl	%eax, 1344(%rsp)
	leal	2(%r11), %eax
	movl	%eax, 1336(%rsp)
	je	.L30
	movq	1120(%rsp), %rdi
	movq	1328(%rsp), %rax
	movq	1168(%rsp), %rsi
	leaq	32(%rax,%rdi), %rax
	movq	1176(%rsp), %rdi
	movq	%rax, %rdx
	leaq	24(%rdi), %rcx
	movq	1360(%rsp), %rdi
	subq	%rcx, %rdx
	movq	%rax, %rcx
	cmpq	$48, %rdx
	seta	%dl
	addq	$24, %rsi
	subq	%rsi, %rcx
	leaq	24(%rdi), %rsi
	cmpq	$48, %rcx
	seta	%cl
	andl	%edx, %ecx
	movq	%rax, %rdx
	subq	%rsi, %rdx
	movq	1352(%rsp), %rsi
	cmpq	$48, %rdx
	seta	%dl
	addq	$24, %rsi
	andl	%ecx, %edx
	movq	%rax, %rcx
	subq	%rsi, %rcx
	cmpq	$48, %rcx
	seta	%cl
	testb	%cl, %dl
	je	.L30
	movq	1152(%rsp), %rdi
	leaq	24(%rdi), %rdx
	subq	%rdx, %rax
	cmpq	$48, %rax
	jbe	.L30
	cmpl	$2, 808(%rsp)
	jbe	.L147
	movq	1176(%rsp), %rbx
	movq	1168(%rsp), %r9
	movq	%rdi, %r10
	leaq	32(%r15), %rdi
	leaq	24(%r15), %rcx
	leaq	40(%r15), %rdx
	vbroadcastsd	.LC1(%rip), %ymm13
	vbroadcastsd	.LC3(%rip), %ymm12
	leaq	(%rdi,%rbx), %rsi
	leaq	16(%r15), %r8
	vbroadcastsd	.LC5(%rip), %ymm11
	movq	%rsi, 1320(%rsp)
	leaq	(%rdi,%r9), %rsi
	leaq	(%rdi,%r10), %rax
	movq	%rsi, 1312(%rsp)
	leaq	(%rcx,%rbx), %rsi
	movq	%rsi, 1304(%rsp)
	leaq	(%rdx,%rbx), %rsi
	movq	%rsi, 1296(%rsp)
	movq	1360(%rsp), %rsi
	movq	%rax, 1184(%rsp)
	leaq	(%r8,%rbx), %rax
	leaq	(%rdi,%rsi), %r12
	movq	%rax, 1280(%rsp)
	leaq	48(%r15), %rax
	movq	1352(%rsp), %rsi
	movq	%r12, 1288(%rsp)
	leaq	(%rax,%rbx), %r12
	leaq	(%rdx,%r9), %rbx
	movq	%rbx, 1240(%rsp)
	leaq	(%rdx,%r10), %rbx
	addq	%rsi, %rdi
	movq	%rbx, 1216(%rsp)
	movq	1360(%rsp), %rbx
	movq	%r12, 1256(%rsp)
	leaq	(%rcx,%r9), %r12
	addq	%r8, %rbx
	movq	%r12, 1248(%rsp)
	leaq	(%rcx,%r10), %r12
	movq	%rbx, 1208(%rsp)
	leaq	(%r8,%rsi), %rbx
	movq	1360(%rsp), %rsi
	addq	%rax, %rsi
	movq	%rsi, 1200(%rsp)
	movq	1352(%rsp), %rsi
	addq	%rax, %rsi
	movq	%rsi, 1192(%rsp)
	leaq	(%r8,%r9), %rsi
	addq	%rax, %r9
	movq	%rsi, 1112(%rsp)
	leaq	(%r8,%r10), %rsi
	addq	%rax, %r10
	movq	1360(%rsp), %rax
	movq	%r9, 1104(%rsp)
	movq	888(%rsp), %r8
	movq	%r10, 1096(%rsp)
	leaq	(%rdx,%rax), %r9
	leaq	(%rcx,%rax), %r10
	movq	1352(%rsp), %rax
	addq	%rax, %rdx
	addq	%rax, %rcx
	movq	1328(%rsp), %rax
	leaq	32(%rax,%r8), %rax
	movq	%rax, 1088(%rsp)
	vbroadcastsd	.LC7(%rip), %ymm5
	xorl	%eax, %eax
	movq	1184(%rsp), %r8
	vbroadcastsd	.LC9(%rip), %ymm4
	movq	%rdx, 1184(%rsp)
	movq	%r14, 1080(%rsp)
	vbroadcastsd	.LC11(%rip), %ymm3
	movq	%r13, 1056(%rsp)
	movq	1096(%rsp), %r14
	movl	%r11d, 1072(%rsp)
	movq	1104(%rsp), %r13
	movq	1112(%rsp), %r11
	movq	%r15, 1112(%rsp)
	movq	1088(%rsp), %r15
	.p2align 4,,10
	.p2align 3
.L23:
	movq	1312(%rsp), %rdx
	vmovupd	(%rdx,%rax), %ymm2
	movq	1304(%rsp), %rdx
	vaddpd	(%r8,%rax), %ymm2, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1296(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1320(%rsp), %rdx
	vmulpd	%ymm13, %ymm0, %ymm0
	vfmadd231pd	(%rdx,%rax), %ymm12, %ymm0
	movq	1288(%rsp), %rdx
	vmovupd	(%rdx,%rax), %ymm2
	movq	1280(%rsp), %rdx
	vaddpd	(%rdi,%rax), %ymm2, %ymm2
	vaddpd	(%rdx,%rax), %ymm2, %ymm2
	movq	1256(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm2, %ymm2
	movq	1248(%rsp), %rdx
	vfmadd132pd	%ymm11, %ymm0, %ymm2
	vmovupd	(%rdx,%rax), %ymm0
	movq	1240(%rsp), %rdx
	vaddpd	(%r12,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1216(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1208(%rsp), %rdx
	vfmadd132pd	%ymm5, %ymm2, %ymm0
	vmovupd	(%rdx,%rax), %ymm2
	movq	1200(%rsp), %rdx
	vaddpd	(%rbx,%rax), %ymm2, %ymm2
	vaddpd	(%rdx,%rax), %ymm2, %ymm2
	movq	1192(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm2, %ymm2
	vfmadd132pd	%ymm4, %ymm0, %ymm2
	vmovupd	(%r11,%rax), %ymm0
	vaddpd	(%rsi,%rax), %ymm0, %ymm0
	vaddpd	0(%r13,%rax), %ymm0, %ymm0
	movq	1184(%rsp), %rdx
	vaddpd	(%r14,%rax), %ymm0, %ymm0
	vaddpd	(%r10,%rax), %ymm0, %ymm0
	vaddpd	(%r9,%rax), %ymm0, %ymm0
	vaddpd	(%rcx,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1264(%rsp), %rdx
	vfmadd132pd	%ymm3, %ymm2, %ymm0
	vmovupd	%ymm0, (%r15,%rax)
	addq	$32, %rax
	cmpq	%rdx, %rax
	jne	.L23
	movl	788(%rsp), %ecx
	movl	1384(%rsp), %eax
	movq	1080(%rsp), %r14
	movq	1056(%rsp), %r13
	movl	1072(%rsp), %r11d
	movq	1112(%rsp), %r15
	cmpl	%eax, %ecx
	je	.L28
	subl	%ecx, %eax
	cmpl	$1, %eax
	je	.L148
	movl	704(%rsp), %edi
	movl	%edi, 1320(%rsp)
.L22:
	movq	1144(%rsp), %r12
	movq	1032(%rsp), %rsi
	movq	1008(%rsp), %r10
	movq	776(%rsp), %rdi
	leaq	(%rsi,%r12), %rdx
	movq	1040(%rsp), %rbx
	leaq	4(%rcx,%rdx), %rsi
	leaq	(%r10,%r12), %rdx
	leaq	4(%rcx,%rdx), %r10
	leaq	(%rdi,%r12), %r9
	movq	%rbx, %rdx
	vmovupd	(%r15,%r10,8), %xmm0
	addq	%rcx, %r9
	addq	%r12, %rdx
	leaq	0(,%r10,8), %r8
	leaq	4(%rbx,%r9), %rdi
	movq	1016(%rsp), %rbx
	leaq	4(%rcx,%rdx), %rdx
	vaddpd	(%r15,%rsi,8), %xmm0, %xmm0
	vmovupd	(%r15,%rdi,8), %xmm5
	salq	$3, %rdx
	addq	%rbx, %r12
	movq	1048(%rsp), %rbx
	leaq	4(%rcx,%r12), %rcx
	vaddpd	-8(%r15,%rdi,8), %xmm0, %xmm0
	salq	$3, %rcx
	leaq	4(%rbx,%r9), %r9
	vaddpd	8(%r15,%rdi,8), %xmm0, %xmm0
	vmulpd	.LC1(%rip){1to2}, %xmm0, %xmm0
	vfmadd231pd	.LC3(%rip){1to2}, %xmm5, %xmm0
	vmovupd	(%r15,%rdx), %xmm5
	vaddpd	(%r15,%rcx), %xmm5, %xmm2
	vmovupd	-8(%r15,%rsi,8), %xmm5
	vaddpd	-16(%r15,%rdi,8), %xmm2, %xmm2
	vaddpd	16(%r15,%rdi,8), %xmm2, %xmm2
	vfmadd132pd	.LC5(%rip){1to2}, %xmm0, %xmm2
	vaddpd	-8(%r15,%r8), %xmm5, %xmm0
	vmovupd	-16(%r15,%rdx), %xmm5
	vaddpd	8(%r15,%rsi,8), %xmm0, %xmm0
	vaddpd	8(%r15,%r8), %xmm0, %xmm0
	vfmadd132pd	.LC7(%rip){1to2}, %xmm2, %xmm0
	vaddpd	-16(%r15,%rcx), %xmm5, %xmm2
	vmovupd	-16(%r15,%rsi,8), %xmm5
	vaddpd	16(%r15,%rdx), %xmm2, %xmm2
	vaddpd	16(%r15,%rcx), %xmm2, %xmm2
	vfmadd132pd	.LC9(%rip){1to2}, %xmm0, %xmm2
	vaddpd	-16(%r15,%r8), %xmm5, %xmm0
	vaddpd	16(%r15,%rsi,8), %xmm0, %xmm0
	vaddpd	16(%r15,%r8), %xmm0, %xmm0
	vaddpd	-8(%r15,%rdx), %xmm0, %xmm0
	vaddpd	8(%r15,%rdx), %xmm0, %xmm0
	vaddpd	-8(%r15,%rcx), %xmm0, %xmm0
	vaddpd	8(%r15,%rcx), %xmm0, %xmm0
	vfmadd132pd	.LC11(%rip){1to2}, %xmm2, %xmm0
	vmovupd	%xmm0, (%r15,%r9,8)
	testb	$1, %al
	je	.L28
	movl	1320(%rsp), %edi
	andl	$-2, %eax
	addl	%edi, %eax
.L25:
	movslq	1368(%rsp), %rdi
	movslq	%r11d, %r9
	movslq	1376(%rsp), %r11
	movslq	%eax, %rbx
	imulq	%r13, %r9
	leal	-1(%rax), %ecx
	movslq	1336(%rsp), %r10
	vmovsd	.LC3(%rip), %xmm5
	imulq	%r13, %rdi
	movslq	%ecx, %rcx
	leal	1(%rax), %edx
	imulq	%r13, %r11
	movslq	%edx, %rdx
	imulq	%r13, %r10
	leaq	(%rbx,%r9), %r12
	leaq	(%rbx,%rdi), %r8
	vmovsd	(%r14,%r8,8), %xmm0
	leaq	(%rbx,%r11), %rsi
	leal	-2(%rax), %r8d
	addl	$2, %eax
	movl	%eax, 1368(%rsp)
	movslq	%r8d, %r8
	vaddsd	(%r14,%rsi,8), %xmm0, %xmm0
	leaq	(%rcx,%r9), %rsi
	vaddsd	(%r14,%rsi,8), %xmm0, %xmm0
	leaq	(%rdx,%r9), %rsi
	vaddsd	(%r14,%rsi,8), %xmm0, %xmm0
	movslq	1344(%rsp), %rsi
	imulq	%r13, %rsi
	vmulsd	.LC1(%rip), %xmm0, %xmm0
	leaq	(%rbx,%rsi), %rax
	addq	%r10, %rbx
	vmovsd	(%r14,%rax,8), %xmm2
	movslq	1368(%rsp), %rax
	vaddsd	(%r14,%rbx,8), %xmm2, %xmm2
	leaq	(%r8,%r9), %rbx
	vfmadd231sd	(%r14,%r12,8), %xmm5, %xmm0
	addq	%rax, %r9
	vaddsd	(%r14,%rbx,8), %xmm2, %xmm2
	leaq	(%rcx,%rdi), %rbx
	vaddsd	(%r14,%r9,8), %xmm2, %xmm2
	leaq	(%rcx,%r11), %r9
	vfmadd132sd	.LC5(%rip), %xmm0, %xmm2
	vmovsd	(%r14,%rbx,8), %xmm0
	leaq	(%r8,%rsi), %rbx
	vaddsd	(%r14,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%rdi), %r9
	vaddsd	(%r14,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%r11), %r9
	vaddsd	(%r14,%r9,8), %xmm0, %xmm0
	leaq	(%r8,%r10), %r9
	vfmadd132sd	.LC7(%rip), %xmm2, %xmm0
	vmovsd	(%r14,%rbx,8), %xmm2
	vaddsd	(%r14,%r9,8), %xmm2, %xmm2
	leaq	(%rax,%rsi), %r9
	vaddsd	(%r14,%r9,8), %xmm2, %xmm2
	leaq	(%rax,%r10), %r9
	vaddsd	(%r14,%r9,8), %xmm2, %xmm2
	leaq	(%r8,%rdi), %r9
	addq	%r11, %r8
	addq	%rax, %rdi
	addq	%r11, %rax
	vfmadd132sd	.LC9(%rip), %xmm0, %xmm2
	vmovsd	(%r14,%r9,8), %xmm0
	vaddsd	(%r14,%r8,8), %xmm0, %xmm0
	vaddsd	(%r14,%rdi,8), %xmm0, %xmm0
	vaddsd	(%r14,%rax,8), %xmm0, %xmm0
	leaq	(%rcx,%rsi), %rax
	addq	%rdx, %rsi
	addq	%r10, %rcx
	addq	%r10, %rdx
	vaddsd	(%r14,%rax,8), %xmm0, %xmm0
	movq	1160(%rsp), %rax
	vaddsd	(%r14,%rsi,8), %xmm0, %xmm0
	vaddsd	(%r14,%rcx,8), %xmm0, %xmm0
	vaddsd	(%r14,%rdx,8), %xmm0, %xmm0
	vfmadd132sd	%xmm10, %xmm2, %xmm0
	vmovsd	%xmm0, (%rax,%r12,8)
.L28:
	movq	1232(%rsp), %rax
	movl	1128(%rsp), %edi
	addq	%r13, 1144(%rsp)
	addq	%rax, 1136(%rsp)
	addq	%rax, 1176(%rsp)
	addq	%rax, 1168(%rsp)
	addq	%rax, 1360(%rsp)
	addq	%rax, 1352(%rsp)
	addq	%rax, 1152(%rsp)
	addq	%rax, 1328(%rsp)
	cmpl	%edi, 1376(%rsp)
	jne	.L34
	movl	880(%rsp), %edx
.L29:
	subl	$3, 1128(%rsp)
	movl	1128(%rsp), %eax
	cmpl	$2, %eax
	jne	.L124
	movq	392(%rsp), %rax
	cmpl	$3, 1384(%rsp)
	vmovdqa	%ymm1, 1408(%rsp)
	movl	864(%rsp), %r12d
	movq	856(%rsp), %rbx
	movq	%rax, 1440(%rsp)
	movq	384(%rsp), %rax
	movq	%rax, 1448(%rsp)
	jle	.L35
	movq	368(%rsp), %r9
	movq	616(%rsp), %r8
	movq	336(%rsp), %rdi
	movq	200(%rsp), %rsi
	movq	344(%rsp), %rcx
	movq	1232(%rsp), %r11
	leaq	40(%r9), %r10
	.p2align 4,,10
	.p2align 3
.L36:
	movq	(%r9), %rdx
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L37:
	vmovupd	32(%rdi,%rax), %ymm4
	vmovupd	32(%r8,%rax), %ymm5
	vmovupd	32(%rcx,%rax), %ymm0
	vmovupd	32(%rsi,%rax), %ymm2
	vshuff64x2	$0, %ymm4, %ymm0, %ymm3
	vshuff64x2	$3, %ymm4, %ymm0, %ymm0
	vshuff64x2	$0, %ymm5, %ymm2, %ymm4
	vshuff64x2	$3, %ymm5, %ymm2, %ymm2
	vunpckhpd	%ymm2, %ymm0, %ymm5
	vunpcklpd	%ymm2, %ymm0, %ymm0
	vunpckhpd	%ymm4, %ymm3, %ymm2
	vunpcklpd	%ymm4, %ymm3, %ymm3
	vmovupd	%ymm5, 96(%rdx,%rax,4)
	vmovupd	%ymm0, 64(%rdx,%rax,4)
	vmovupd	%ymm2, 32(%rdx,%rax,4)
	vmovupd	%ymm3, (%rdx,%rax,4)
	addq	$32, %rax
	cmpq	%rbx, %rax
	jne	.L37
	addq	$8, %r9
	addq	%r11, %rcx
	addq	%r11, %rsi
	addq	%r11, %rdi
	addq	%r11, %r8
	cmpq	%r10, %r9
	jne	.L36
.L35:
	movq	32(%rsp), %rax
	movq	%rax, 1176(%rsp)
	movq	208(%rsp), %rax
	movq	%rax, 1184(%rsp)
	movq	40(%rsp), %rax
	movq	%rax, 1192(%rsp)
	movq	48(%rsp), %rax
	movq	%rax, 1200(%rsp)
	movq	56(%rsp), %rax
	movq	%rax, 1208(%rsp)
	movq	64(%rsp), %rax
	movq	%rax, 1216(%rsp)
	movq	216(%rsp), %rax
	movq	%rax, 1256(%rsp)
	movq	24(%rsp), %rax
	movq	%rax, 720(%rsp)
	movq	160(%rsp), %rax
	movq	%rax, 728(%rsp)
	movq	184(%rsp), %rax
	movq	%rax, 736(%rsp)
	movq	168(%rsp), %rax
	movq	%rax, 744(%rsp)
	movq	192(%rsp), %rax
	movq	%rax, 752(%rsp)
	movq	776(%rsp), %rax
	movq	%rax, 800(%rsp)
	movq	1232(%rsp), %rax
	movq	%rax, 760(%rsp)
	movq	176(%rsp), %rax
	movq	%rax, 768(%rsp)
	movq	224(%rsp), %rax
	movq	%rax, 1240(%rsp)
	movq	616(%rsp), %r14
	movq	384(%rsp), %rax
	movl	$2, 812(%rsp)
	movq	%r14, 1248(%rsp)
	vbroadcastsd	.LC3(%rip), %ymm17
	movq	%rax, 856(%rsp)
	movq	392(%rsp), %rax
	movq	$0, 696(%rsp)
	vbroadcastsd	.LC5(%rip), %ymm19
	movq	%rax, 1344(%rsp)
	movq	112(%rsp), %rax
	movq	$0, 712(%rsp)
	vbroadcastsd	.LC7(%rip), %ymm18
	movq	%rax, 1376(%rsp)
	movq	120(%rsp), %rax
	movl	$13, 784(%rsp)
	movq	%rax, 1352(%rsp)
	movq	128(%rsp), %rax
	movl	$12, 1008(%rsp)
	movq	%rax, 1360(%rsp)
	movq	376(%rsp), %rax
	movl	%r12d, 360(%rsp)
	movq	%rbx, 328(%rsp)
	movq	%rax, 1368(%rsp)
	movl	812(%rsp), %eax
	vmovdqa	%ymm1, 288(%rsp)
	.p2align 4,,10
	.p2align 3
.L63:
	movslq	%eax, %rdi
	incl	%eax
	cmpl	$3, 1224(%rsp)
	movl	%eax, 812(%rsp)
	jle	.L276
	movq	1272(%rsp), %rax
	leal	12(%rdi), %edx
	leal	4(%rdi), %esi
	movslq	812(%rsp), %r9
	movslq	%edx, %rdx
	movl	%esi, 848(%rsp)
	imulq	%rax, %rdx
	movq	%r9, 888(%rsp)
	imulq	%rax, %rsi
	movq	%rdx, 1072(%rsp)
	addq	$4, %rdx
	leaq	0(,%rdx,8), %rcx
	salq	$3, %rsi
	movq	%rcx, 1304(%rsp)
	movq	816(%rsp), %rcx
	vmovupd	(%rcx,%rdx,8), %ymm7
	leal	7(%rdi), %edx
	movl	%edx, 1048(%rsp)
	imulq	%rax, %rdx
	leaq	0(,%rdx,8), %r11
	leaq	(%r14,%rsi), %rdx
	movq	%rdx, 1104(%rsp)
	movq	%rax, %rdx
	leaq	(%rcx,%r11), %rbx
	imulq	%r9, %rdx
	movq	%r11, 1280(%rsp)
	salq	$3, %rdx
	movq	%rdx, 1312(%rsp)
	addq	%rcx, %rdx
	movq	%rdx, 1288(%rsp)
	leal	-2(%rdi), %edx
	movq	%rdx, %r9
	leal	8(%rdi), %edx
	imulq	%rax, %r9
	movl	%edx, 1040(%rsp)
	imulq	%rax, %rdx
	leaq	(%r14,%r9,8), %r10
	movq	%r9, 1168(%rsp)
	movq	%rax, %r9
	movq	%r10, 1080(%rsp)
	leaq	0(,%rdx,8), %r10
	movq	%rcx, %rdx
	leaq	(%rcx,%r10), %r13
	leal	5(%rdi), %ecx
	movq	%r10, 1320(%rsp)
	movq	%rdx, %r10
	movl	%ecx, 840(%rsp)
	imulq	%rax, %rcx
	addq	%rsi, %r10
	leaq	0(,%rcx,8), %r15
	leal	2(%rdi), %ecx
	leaq	(%r14,%r15), %r8
	movl	%ecx, 1032(%rsp)
	movq	%r8, 1120(%rsp)
	imulq	%rax, %rcx
	leal	-1(%rdi), %r8d
	imulq	%r9, %r8
	movq	%r15, 1336(%rsp)
	leaq	0(,%rcx,8), %rax
	movq	%r8, 1160(%rsp)
	leaq	(%r14,%r8,8), %rcx
	leal	9(%rdi), %r8d
	movl	%r8d, 832(%rsp)
	imulq	%r9, %r8
	leaq	(%rdx,%rax), %r12
	movq	%rax, 1328(%rsp)
	movq	%r9, %rax
	movq	%rcx, 1144(%rsp)
	leaq	(%rdx,%r8,8), %r15
	leal	6(%rdi), %r8d
	imulq	%r8, %rax
	leal	3(%rdi), %r8d
	movl	%r8d, 824(%rsp)
	imulq	%r9, %r8
	imulq	%r9, %rdi
	leaq	(%r14,%rax,8), %rcx
	movq	%rcx, 1152(%rsp)
	leaq	(%rdx,%r8,8), %rcx
	movq	%r9, %r8
	movq	%rcx, 1296(%rsp)
	leaq	(%r14,%rdi,8), %rcx
	movq	%rcx, 1136(%rsp)
	movl	1008(%rsp), %ecx
	movq	%rdi, 1056(%rsp)
	movq	%r9, %rdi
	imulq	%rcx, %rdi
	movq	%rcx, 792(%rsp)
	leaq	(%r14,%r11), %rcx
	movq	%rcx, 1128(%rsp)
	movq	1328(%rsp), %r11
	vmovsd	24(%rbx), %xmm3
	vmovsd	24(%r15), %xmm4
	leaq	(%rdx,%rdi,8), %r9
	movq	1312(%rsp), %rdi
	vmovhpd	24(%r14,%rsi), %xmm3, %xmm3
	vmovhpd	24(%r14,%rax,8), %xmm4, %xmm4
	leaq	(%r14,%rdi), %rcx
	movq	%r8, %rdi
	movq	%rdx, %r8
	movq	%rcx, 1112(%rsp)
	movslq	784(%rsp), %rcx
	imulq	%rcx, %rdi
	movq	%rcx, 1016(%rsp)
	leaq	(%rdx,%rdi,8), %rdi
	movq	1320(%rsp), %rdx
	vmovsd	24(%rdi), %xmm13
	movq	%rdx, %rcx
	addq	%r14, %rcx
	vmovhpd	24(%r14,%rdx), %xmm13, %xmm13
	movq	1352(%rsp), %rdx
	movq	%rcx, 1096(%rsp)
	movq	1336(%rsp), %rcx
	vmovapd	(%rdx), %ymm27
	addq	%rcx, %r8
	leaq	(%r14,%r11), %rcx
	movq	%rcx, 1088(%rsp)
	movq	1288(%rsp), %rcx
	vmovsd	24(%rcx), %xmm0
	movq	1168(%rsp), %rcx
	vmovhpd	24(%r14,%rcx,8), %xmm0, %xmm0
	movq	1296(%rsp), %rcx
	vinsertf128	$0x1, %xmm0, %ymm3, %ymm3
	vmovsd	24(%rcx), %xmm0
	movq	1056(%rsp), %rcx
	vmovhpd	24(%r14,%rcx,8), %xmm0, %xmm0
	movq	1344(%rsp), %rcx
	vinsertf128	$0x1, %xmm0, %ymm4, %ymm4
	vmovsd	24(%r8), %xmm0
	vmovapd	(%rcx), %ymm30
	vmovhpd	24(%r14,%r11), %xmm0, %xmm0
	movq	1368(%rsp), %r11
	vinsertf128	$0x1, %xmm0, %ymm13, %ymm13
	vmovapd	(%r11), %ymm31
	vmovapd	32(%r11), %ymm26
	movq	1360(%rsp), %r11
	vmovapd	32(%rdx), %ymm22
	movq	1376(%rsp), %rdx
	vmovsd	24(%r12), %xmm0
	vaddpd	%ymm30, %ymm31, %ymm10
	vmovsd	24(%r13), %xmm1
	vmovapd	32(%r11), %ymm11
	vmovapd	32(%rdx), %ymm15
	vmovapd	64(%rdx), %ymm28
	movq	1160(%rsp), %rdx
	vmovapd	64(%r11), %ymm12
	vmovsd	24(%r10), %xmm2
	movq	1344(%rsp), %r11
	vmovhpd	24(%r14,%rdx,8), %xmm0, %xmm0
	movq	1336(%rsp), %rdx
	movq	1344(%rsp), %rcx
	vmovapd	64(%r11), %ymm20
	movq	1360(%rsp), %r11
	vmovhpd	24(%r14,%rdx), %xmm1, %xmm1
	movq	1312(%rsp), %rdx
	vmovapd	32(%rcx), %ymm25
	vinsertf128	$0x1, %xmm0, %ymm1, %ymm1
	vmovsd	24(%r9), %xmm0
	movq	1368(%rsp), %rcx
	vmovhpd	24(%r14,%rdx), %xmm2, %xmm2
	movq	1280(%rsp), %rdx
	vmovapd	(%r11), %ymm6
	vaddpd	%ymm25, %ymm26, %ymm29
	vmovapd	64(%rcx), %ymm21
	movq	1352(%rsp), %rcx
	vmovhpd	24(%r14,%rdx), %xmm0, %xmm0
	movq	1376(%rsp), %rdx
	vinsertf128	$0x1, %xmm2, %ymm0, %ymm0
	vmovapd	64(%rcx), %ymm16
	vaddpd	(%rdx), %ymm6, %ymm2
	movq	1296(%rsp), %rdx
	vaddpd	%ymm0, %ymm1, %ymm1
	vmovsd	16(%rdx), %xmm0
	movq	1056(%rsp), %rdx
	vmovapd	%ymm17, 928(%rsp)
	vmovsd	16(%r15), %xmm8
	vmovsd	16(%rbx), %xmm5
	vaddpd	%ymm1, %ymm11, %ymm6
	vaddpd	%ymm15, %ymm11, %ymm11
	vmovhpd	16(%r14,%rdx,8), %xmm0, %xmm0
	movq	1288(%rsp), %rdx
	vmovsd	16(%r8), %xmm9
	vmovhpd	16(%r14,%rax,8), %xmm8, %xmm8
	movq	1168(%rsp), %rax
	vmovhpd	16(%r14,%rsi), %xmm5, %xmm5
	movq	1160(%rsp), %rsi
	vinsertf128	$0x1, %xmm0, %ymm8, %ymm8
	vmovsd	16(%rdx), %xmm0
	vmovsd	16(%r10), %xmm14
	vaddpd	%ymm15, %ymm6, %ymm6
	vaddpd	%ymm10, %ymm8, %ymm8
	vaddpd	%ymm2, %ymm12, %ymm15
	vmovhpd	16(%r14,%rax,8), %xmm0, %xmm0
	movq	1328(%rsp), %rax
	vinsertf128	$0x1, %xmm0, %ymm5, %ymm5
	vmovsd	16(%rdi), %xmm0
	vmovhpd	16(%r14,%rax), %xmm9, %xmm9
	movq	1320(%rsp), %rax
	vaddpd	%ymm16, %ymm8, %ymm8
	vaddpd	%ymm28, %ymm15, %ymm15
	vmovhpd	16(%r14,%rax), %xmm0, %xmm0
	movq	1336(%rsp), %rax
	vinsertf128	$0x1, %xmm9, %ymm0, %ymm0
	vmovsd	16(%r12), %xmm9
	vaddpd	%ymm0, %ymm5, %ymm5
	vmovsd	16(%r13), %xmm0
	vmovhpd	16(%r14,%rsi,8), %xmm9, %xmm9
	movq	1312(%rsp), %rsi
	vmovhpd	16(%r14,%rax), %xmm0, %xmm0
	movq	1280(%rsp), %rax
	vinsertf128	$0x1, %xmm9, %ymm0, %ymm0
	vmovsd	16(%r9), %xmm9
	vmovhpd	16(%r14,%rsi), %xmm14, %xmm14
	vaddpd	%ymm21, %ymm5, %ymm5
	vmovhpd	16(%r14,%rax), %xmm9, %xmm9
	vinsertf128	$0x1, %xmm14, %ymm9, %ymm9
	vmovapd	.LC0(%rip), %ymm14
	vaddpd	%ymm20, %ymm5, %ymm5
	vaddpd	%ymm9, %ymm0, %ymm0
	vaddpd	%ymm2, %ymm4, %ymm9
	vmovapd	%ymm14, 896(%rsp)
	vaddpd	%ymm29, %ymm4, %ymm4
	vmovapd	%ymm19, 960(%rsp)
	movq	856(%rsp), %rax
	movq	1376(%rsp), %rdx
	vaddpd	%ymm22, %ymm9, %ymm9
	vaddpd	%ymm12, %ymm0, %ymm0
	movq	1344(%rsp), %rsi
	cmpl	$7, 1388(%rsp)
	vmovapd	%ymm18, 640(%rsp)
	vmulpd	%ymm14, %ymm9, %ymm9
	vaddpd	%ymm28, %ymm0, %ymm0
	vaddpd	%ymm3, %ymm0, %ymm0
	vaddpd	%ymm13, %ymm3, %ymm3
	vfmadd231pd	%ymm17, %ymm27, %ymm9
	vaddpd	%ymm26, %ymm0, %ymm0
	vaddpd	%ymm13, %ymm0, %ymm0
	vaddpd	%ymm11, %ymm27, %ymm13
	vfmadd132pd	%ymm19, %ymm9, %ymm8
	vbroadcastsd	.LC9(%rip), %ymm9
	vaddpd	%ymm25, %ymm0, %ymm0
	vaddpd	%ymm16, %ymm13, %ymm13
	vmulpd	.LC0(%rip), %ymm13, %ymm13
	vfmadd132pd	%ymm18, %ymm8, %ymm6
	vbroadcastsd	.LC11(%rip), %ymm8
	vfmadd132pd	%ymm9, %ymm6, %ymm5
	vfmadd231pd	%ymm17, %ymm22, %ymm13
	vfmadd132pd	%ymm8, %ymm5, %ymm0
	vpermpd	$147, %ymm0, %ymm0
	vblendpd	$1, %ymm7, %ymm0, %ymm5
	vpermilpd	$5, %ymm7, %ymm7
	vmovapd	%ymm5, (%rax)
	movq	1368(%rsp), %rax
	vmovapd	96(%r11), %ymm6
	vmovapd	96(%rcx), %ymm14
	vmovapd	96(%rdx), %ymm23
	vmovapd	96(%rax), %ymm5
	vaddpd	%ymm14, %ymm4, %ymm4
	vaddpd	%ymm1, %ymm6, %ymm1
	vmovapd	96(%rsi), %ymm24
	movq	856(%rsp), %rdx
	vaddpd	%ymm5, %ymm3, %ymm3
	vfmadd132pd	%ymm19, %ymm13, %ymm4
	vaddpd	%ymm23, %ymm1, %ymm1
	vaddpd	%ymm24, %ymm3, %ymm3
	vaddpd	%ymm31, %ymm1, %ymm1
	vfmadd132pd	%ymm18, %ymm4, %ymm15
	vaddpd	%ymm21, %ymm1, %ymm1
	vaddpd	%ymm30, %ymm1, %ymm1
	vfmadd132pd	%ymm9, %ymm15, %ymm3
	vaddpd	%ymm20, %ymm1, %ymm1
	vfmadd132pd	%ymm8, %ymm3, %ymm1
	vpermpd	$147, %ymm1, %ymm1
	vunpcklpd	%ymm1, %ymm0, %ymm0
	vblendpd	$1, %ymm7, %ymm1, %ymm1
	vmovapd	%ymm1, 32(%rdx)
	je	.L42
	movq	1376(%rsp), %rdx
	vmovapd	128(%rax), %ymm15
	vmovapd	128(%r11), %ymm1
	vmovapd	128(%rcx), %ymm4
	vmovapd	128(%rdx), %ymm30
	vmovapd	128(%rsi), %ymm13
.L43:
	vaddpd	%ymm2, %ymm1, %ymm2
	vaddpd	%ymm28, %ymm12, %ymm12
	movq	856(%rsp), %rax
	cmpl	$8, 1388(%rsp)
	vaddpd	%ymm20, %ymm21, %ymm3
	vaddpd	%ymm6, %ymm11, %ymm28
	vaddpd	%ymm10, %ymm15, %ymm10
	vaddpd	%ymm30, %ymm2, %ymm2
	vaddpd	%ymm27, %ymm3, %ymm3
	vaddpd	%ymm23, %ymm28, %ymm28
	vaddpd	%ymm13, %ymm10, %ymm10
	vaddpd	%ymm26, %ymm2, %ymm2
	vpermilpd	$5, %ymm7, %ymm26
	vaddpd	%ymm4, %ymm3, %ymm3
	vaddpd	%ymm5, %ymm2, %ymm2
	vaddpd	%ymm25, %ymm2, %ymm2
	vaddpd	%ymm12, %ymm22, %ymm25
	vaddpd	%ymm14, %ymm25, %ymm25
	vaddpd	%ymm24, %ymm2, %ymm2
	vmulpd	.LC0(%rip), %ymm25, %ymm25
	vfmadd231pd	%ymm17, %ymm16, %ymm25
	vfmadd132pd	%ymm19, %ymm25, %ymm3
	vfmadd132pd	%ymm18, %ymm3, %ymm28
	vfmadd132pd	%ymm9, %ymm28, %ymm10
	vfmadd132pd	%ymm8, %ymm10, %ymm2
	vblendpd	$8, %ymm2, %ymm0, %ymm0
	vblendpd	$8, %ymm7, %ymm2, %ymm2
	vpermilpd	$6, %ymm0, %ymm3
	vpermpd	$147, %ymm2, %ymm0
	vmovapd	%ymm0, 64(%rax)
	jle	.L44
	movq	1368(%rsp), %rax
	vmovapd	160(%rax), %ymm2
	movq	1360(%rsp), %rax
	vmovapd	160(%rax), %ymm0
	movq	1352(%rsp), %rax
	vmovapd	160(%rax), %ymm25
	movq	1376(%rsp), %rax
	vmovapd	160(%rax), %ymm10
	movq	1344(%rsp), %rax
	vmovapd	160(%rax), %ymm7
.L45:
	vaddpd	%ymm23, %ymm6, %ymm6
	vaddpd	%ymm24, %ymm5, %ymm5
	movq	856(%rsp), %rax
	movq	1256(%rsp), %rdi
	vaddpd	%ymm11, %ymm0, %ymm0
	vaddpd	%ymm12, %ymm1, %ymm1
	cmpl	$12, 1388(%rsp)
	vaddpd	%ymm29, %ymm2, %ymm2
	vaddpd	%ymm16, %ymm6, %ymm6
	vaddpd	%ymm22, %ymm5, %ymm5
	vaddpd	%ymm10, %ymm0, %ymm0
	vaddpd	%ymm30, %ymm1, %ymm1
	vaddpd	%ymm7, %ymm2, %ymm2
	vaddpd	%ymm4, %ymm6, %ymm4
	vaddpd	%ymm25, %ymm5, %ymm5
	vmovapd	%ymm26, %ymm6
	vaddpd	%ymm21, %ymm0, %ymm0
	vmulpd	.LC0(%rip), %ymm4, %ymm4
	vaddpd	%ymm15, %ymm0, %ymm0
	vaddpd	%ymm20, %ymm0, %ymm0
	vfmadd132pd	%ymm17, %ymm4, %ymm14
	vaddpd	%ymm13, %ymm0, %ymm0
	vfmadd132pd	%ymm19, %ymm14, %ymm5
	vfmadd132pd	%ymm18, %ymm5, %ymm1
	vfmadd132pd	%ymm9, %ymm1, %ymm2
	vfmadd132pd	%ymm8, %ymm2, %ymm0
	vblendpd	$8, %ymm0, %ymm3, %ymm3
	vblendpd	$8, %ymm6, %ymm0, %ymm0
	vpermpd	$147, %ymm0, %ymm0
	vmovupd	%ymm3, 32(%rdi)
	vmovapd	%ymm0, 96(%rax)
	jle	.L149
	movq	816(%rsp), %rbx
	movq	1304(%rsp), %rdi
	leaq	128(%rax), %r9
	movq	600(%rsp), %r11
	movq	1072(%rsp), %rax
	movq	1352(%rsp), %rcx
	leaq	32(%rbx,%rdi), %r10
	movq	1368(%rsp), %rdi
	addq	%rax, %r11
	movq	1344(%rsp), %rdx
	leaq	64(%rdi), %r8
	leaq	64(%rcx), %rsi
	movq	1360(%rsp), %rdi
	movq	1376(%rsp), %rcx
	leaq	96(%rbx,%r11,8), %r11
	movq	800(%rsp), %rbx
	addq	$64, %rdx
	addq	$64, %rdi
	addq	$64, %rcx
	subq	%rax, %rbx
	.p2align 4,,10
	.p2align 3
.L47:
	vmovapd	32(%rdi), %ymm5
	vmovapd	32(%rsi), %ymm23
	subq	$-128, %r8
	subq	$-128, %rdi
	vmovapd	96(%rsi), %ymm20
	vmovapd	64(%rdx), %ymm25
	subq	$-128, %rsi
	subq	$-128, %rcx
	vaddpd	-96(%rcx), %ymm5, %ymm3
	vmovapd	-64(%rdi), %ymm5
	subq	$-128, %rdx
	subq	$-128, %r9
	vmovapd	-64(%r8), %ymm27
	vmovapd	-128(%rdi), %ymm0
	vaddpd	-64(%rcx), %ymm5, %ymm2
	vmovapd	-64(%rsi), %ymm30
	vaddpd	%ymm25, %ymm27, %ymm10
	vaddpd	-128(%rcx), %ymm0, %ymm0
	vmovapd	(%rdi), %ymm13
	vmovapd	(%rsi), %ymm14
	vmovapd	-32(%rdi), %ymm1
	vaddpd	%ymm2, %ymm23, %ymm21
	vmovapd	(%rcx), %ymm11
	vmovapd	-128(%r8), %ymm4
	vaddpd	%ymm2, %ymm13, %ymm31
	vaddpd	-128(%rsi), %ymm10, %ymm12
	vaddpd	%ymm13, %ymm0, %ymm0
	vmovapd	-32(%rcx), %ymm7
	vaddpd	%ymm3, %ymm1, %ymm5
	vmovapd	-96(%r8), %ymm22
	vaddpd	-128(%rdx), %ymm4, %ymm4
	vaddpd	%ymm20, %ymm21, %ymm21
	vmovapd	(%r8), %ymm16
	vmovapd	-32(%r8), %ymm29
	vaddpd	%ymm7, %ymm1, %ymm1
	vaddpd	%ymm14, %ymm12, %ymm12
	vaddpd	%ymm11, %ymm0, %ymm0
	vmovapd	(%rdx), %ymm15
	vmovapd	-96(%rdx), %ymm28
	vaddpd	%ymm7, %ymm5, %ymm5
	vaddpd	%ymm16, %ymm4, %ymm4
	vmovapd	-32(%rdx), %ymm26
	vmovupd	(%r10), %ymm6
	vmulpd	.LC0(%rip), %ymm21, %ymm21
	vaddpd	%ymm11, %ymm31, %ymm31
	vaddpd	%ymm22, %ymm0, %ymm0
	vaddpd	%ymm26, %ymm29, %ymm7
	vaddpd	%ymm15, %ymm4, %ymm4
	vaddpd	%ymm28, %ymm22, %ymm22
	vaddpd	%ymm29, %ymm0, %ymm0
	vaddpd	%ymm7, %ymm23, %ymm23
	vfmadd231pd	%ymm17, %ymm30, %ymm21
	vaddpd	%ymm28, %ymm0, %ymm0
	vfmadd132pd	%ymm19, %ymm21, %ymm12
	vaddpd	%ymm26, %ymm0, %ymm0
	vfmadd132pd	%ymm18, %ymm12, %ymm5
	vfmadd132pd	%ymm9, %ymm5, %ymm4
	vfmadd132pd	%ymm8, %ymm4, %ymm0
	vpermpd	$147, %ymm0, %ymm0
	vblendpd	$1, %ymm6, %ymm0, %ymm4
	vpermilpd	$5, %ymm6, %ymm6
	vmovapd	%ymm4, -128(%r9)
	vmovapd	32(%rdi), %ymm5
	vmovapd	32(%rcx), %ymm21
	vmovapd	32(%r8), %ymm4
	vmovapd	32(%rsi), %ymm12
	vaddpd	%ymm5, %ymm3, %ymm3
	vmovapd	32(%rdx), %ymm24
	vaddpd	%ymm12, %ymm23, %ymm23
	vaddpd	%ymm4, %ymm22, %ymm22
	vaddpd	%ymm21, %ymm3, %ymm3
	vaddpd	%ymm24, %ymm22, %ymm22
	vaddpd	%ymm27, %ymm3, %ymm3
	vaddpd	%ymm16, %ymm3, %ymm3
	vaddpd	%ymm25, %ymm3, %ymm3
	vaddpd	%ymm1, %ymm30, %ymm25
	vaddpd	%ymm14, %ymm25, %ymm25
	vaddpd	%ymm15, %ymm3, %ymm3
	vmulpd	.LC0(%rip), %ymm25, %ymm25
	vfmadd231pd	%ymm17, %ymm20, %ymm25
	vfmadd132pd	%ymm19, %ymm25, %ymm23
	vfmadd132pd	%ymm18, %ymm23, %ymm31
	vfmadd132pd	%ymm9, %ymm31, %ymm22
	vfmadd132pd	%ymm8, %ymm22, %ymm3
	vpermpd	$147, %ymm3, %ymm3
	vunpcklpd	%ymm3, %ymm0, %ymm0
	vblendpd	$1, %ymm6, %ymm3, %ymm3
	vmovapd	%ymm3, -96(%r9)
	vaddpd	%ymm11, %ymm13, %ymm3
	vaddpd	%ymm15, %ymm16, %ymm11
	vmovapd	64(%rdi), %ymm28
	vmovapd	64(%rcx), %ymm27
	vaddpd	%ymm1, %ymm5, %ymm13
	vmovapd	64(%rsi), %ymm22
	vmovapd	64(%r8), %ymm25
	vaddpd	%ymm28, %ymm2, %ymm2
	vaddpd	%ymm30, %ymm11, %ymm11
	vmovapd	64(%rdx), %ymm23
	vaddpd	%ymm21, %ymm13, %ymm13
	vaddpd	%ymm25, %ymm10, %ymm10
	vaddpd	%ymm27, %ymm2, %ymm2
	vaddpd	%ymm22, %ymm11, %ymm11
	vaddpd	%ymm23, %ymm10, %ymm10
	vaddpd	%ymm29, %ymm2, %ymm2
	vaddpd	%ymm4, %ymm2, %ymm2
	vaddpd	%ymm24, %ymm4, %ymm4
	vaddpd	%ymm26, %ymm2, %ymm2
	vaddpd	%ymm20, %ymm3, %ymm26
	vaddpd	%ymm20, %ymm4, %ymm4
	vaddpd	%ymm28, %ymm3, %ymm3
	vaddpd	%ymm12, %ymm26, %ymm26
	vaddpd	%ymm24, %ymm2, %ymm2
	vaddpd	%ymm27, %ymm3, %ymm3
	vmulpd	.LC0(%rip), %ymm26, %ymm26
	vfmadd231pd	%ymm17, %ymm14, %ymm26
	vfmadd132pd	%ymm19, %ymm26, %ymm11
	vfmadd132pd	%ymm18, %ymm11, %ymm13
	vfmadd132pd	%ymm9, %ymm13, %ymm10
	vfmadd132pd	%ymm8, %ymm10, %ymm2
	vblendpd	$8, %ymm2, %ymm0, %ymm0
	vblendpd	$8, %ymm6, %ymm2, %ymm2
	vpermilpd	$5, %ymm6, %ymm6
	vpermpd	$147, %ymm2, %ymm2
	vpermilpd	$6, %ymm0, %ymm0
	vmovapd	%ymm2, -64(%r9)
	vaddpd	%ymm21, %ymm5, %ymm2
	vaddpd	96(%rsi), %ymm4, %ymm4
	vaddpd	96(%rdi), %ymm1, %ymm1
	vaddpd	96(%r8), %ymm7, %ymm7
	vaddpd	%ymm14, %ymm2, %ymm2
	vaddpd	96(%rcx), %ymm1, %ymm1
	vaddpd	96(%rdx), %ymm7, %ymm7
	vaddpd	%ymm22, %ymm2, %ymm2
	vaddpd	%ymm16, %ymm1, %ymm1
	vmulpd	.LC0(%rip), %ymm2, %ymm2
	vaddpd	%ymm25, %ymm1, %ymm1
	vaddpd	%ymm15, %ymm1, %ymm1
	vfmadd132pd	%ymm17, %ymm2, %ymm12
	vaddpd	%ymm23, %ymm1, %ymm1
	vfmadd132pd	%ymm19, %ymm12, %ymm4
	vfmadd132pd	%ymm18, %ymm4, %ymm3
	vfmadd132pd	%ymm9, %ymm3, %ymm7
	vfmadd132pd	%ymm8, %ymm7, %ymm1
	vblendpd	$8, %ymm1, %ymm0, %ymm0
	vblendpd	$8, %ymm6, %ymm1, %ymm1
	vpermpd	$147, %ymm1, %ymm1
	vmovupd	%ymm0, (%r10,%rbx,8)
	addq	$32, %r10
	vmovapd	%ymm1, -32(%r9)
	cmpq	%r11, %r10
	jne	.L47
	movslq	632(%rsp), %rcx
.L46:
	movl	1224(%rsp), %eax
	cmpl	%eax, %ecx
	jg	.L48
	movq	712(%rsp), %rdi
	movq	856(%rsp), %rax
	movq	%rcx, %rdx
	salq	$5, %rdx
	movq	728(%rsp), %r11
	leaq	1(%rcx,%rdi), %rsi
	addq	%rdx, %rax
	leaq	(%r14,%rsi,8), %rsi
	movq	%rax, 1056(%rsp)
	movq	816(%rsp), %rax
	leaq	(%rcx,%r11), %r10
	movq	%rsi, 1336(%rsp)
	movq	624(%rsp), %rsi
	leaq	0(,%r10,8), %r13
	movq	720(%rsp), %r11
	addq	%rdi, %rsi
	addq	%rcx, %rsi
	leaq	(%rcx,%r11), %r10
	movq	1304(%rsp), %r11
	leaq	(%r14,%rsi,8), %rsi
	leaq	0(,%r10,8), %r12
	movq	%rsi, 1328(%rsp)
	movq	768(%rsp), %rsi
	leaq	-32(%rax,%r11), %r11
	movq	%r11, 1168(%rsp)
	movq	1232(%rsp), %r11
	leaq	1(%rcx,%rsi), %rsi
	leaq	(%rax,%rsi,8), %r15
	movq	760(%rsp), %rsi
	movq	%r11, %r10
	leaq	1(%rcx,%rsi), %rsi
	leaq	(%r14,%rsi,8), %rsi
	movq	%rsi, 1320(%rsp)
	movq	680(%rsp), %rsi
	leaq	1(%rdi,%rsi), %rsi
	addq	%rcx, %rsi
	leaq	(%rax,%rsi,8), %rbx
	movq	800(%rsp), %rsi
	leaq	1(%rcx,%rsi), %rsi
	leaq	(%r14,%rsi,8), %rsi
	movq	%rsi, 1312(%rsp)
	movq	752(%rsp), %rsi
	leaq	(%rcx,%rsi), %r9
	movq	744(%rsp), %rsi
	salq	$3, %r9
	leaq	(%rcx,%rsi), %r8
	movq	688(%rsp), %rsi
	salq	$3, %r8
	addq	%rdi, %rsi
	movq	736(%rsp), %rdi
	addq	%rcx, %rsi
	salq	$3, %rsi
	addq	%rcx, %rdi
	movq	%rsi, 1072(%rsp)
	movq	792(%rsp), %rsi
	salq	$3, %rdi
	imulq	%rsi, %r10
	leaq	(%rax,%r10), %rsi
	movslq	1048(%rsp), %r10
	movq	%rsi, 1288(%rsp)
	movq	1016(%rsp), %rsi
	imulq	%rsi, %r11
	leaq	(%rax,%r11), %rsi
	movq	1272(%rsp), %r11
	movq	%rsi, 1280(%rsp)
	imulq	%r11, %r10
	leaq	2(%r10), %rsi
	movq	%r10, 1000(%rsp)
	movq	888(%rsp), %r10
	movq	%rsi, 992(%rsp)
	imulq	%r11, %r10
	movq	%r10, 1160(%rsp)
	movslq	1040(%rsp), %r10
	movq	%r15, 1304(%rsp)
	movq	%r8, %r15
	imulq	%r11, %r10
	movq	%rbx, 1296(%rsp)
	leaq	2(%r10), %rsi
	movq	%r10, 888(%rsp)
	movslq	1032(%rsp), %r10
	movq	%rsi, 880(%rsp)
	imulq	%r11, %r10
	movq	%r9, %r11
	movq	1056(%rsp), %r9
	leaq	2(%r10), %rsi
	movq	%r10, 872(%rsp)
	movq	%rsi, 864(%rsp)
	movq	1072(%rsp), %rsi
	jmp	.L61
	.p2align 4,,10
	.p2align 3
.L277:
	movq	1360(%rsp), %r8
	movq	1368(%rsp), %r10
	movq	1344(%rsp), %rbx
	vmovupd	-192(%r8,%rdx), %ymm0
	movq	1352(%rsp), %r8
	vmovupd	-192(%r10,%rdx), %ymm3
	vmovupd	-192(%rbx,%rdx), %ymm24
	vmovupd	-192(%r8,%rdx), %ymm4
	movq	1376(%rsp), %r8
	vmovupd	-128(%rbx,%rdx), %ymm25
	vmovupd	-192(%r8,%rdx), %ymm21
	leal	-1(%rcx), %r8d
	movslq	%r8d, %r8
	salq	$5, %r8
	vmovapd	-128(%r10,%r8), %ymm5
	movq	1360(%rsp), %r10
	vmovapd	-128(%rbx,%r8), %ymm23
	vmovapd	-128(%r10,%r8), %ymm11
	movq	1352(%rsp), %r10
	vmovapd	-128(%r10,%r8), %ymm22
	movq	1376(%rsp), %r10
	vmovapd	-128(%r10,%r8), %ymm1
	movq	1368(%rsp), %r10
	leal	1(%rcx), %r8d
	vmovupd	-128(%r10,%rdx), %ymm26
	movq	1360(%rsp), %r10
	vmovupd	-128(%r10,%rdx), %ymm12
	movq	1352(%rsp), %r10
	vmovupd	-128(%r10,%rdx), %ymm31
	movq	1376(%rsp), %r10
	vmovupd	-128(%r10,%rdx), %ymm2
	cmpl	%r8d, 1388(%rsp)
	jge	.L50
	movq	1336(%rsp), %rbx
	vmovsd	8(%rax,%rsi), %xmm7
	leal	2(%rcx), %r8d
	vmovsd	8(%rax,%r11), %xmm10
	vmovhpd	(%rbx), %xmm7, %xmm7
	movq	1328(%rsp), %rbx
	vmovhpd	8(%r14,%r15), %xmm10, %xmm10
	vinsertf64x2	$0x1, %xmm7, %ymm10, %ymm30
	vmovsd	8(%rax,%r12), %xmm10
	vmovsd	8(%rax,%rdi), %xmm7
	vmovhpd	(%rbx), %xmm10, %xmm10
	movq	1296(%rsp), %rbx
	vmovhpd	8(%r14,%r13), %xmm7, %xmm7
	vinsertf128	$0x1, %xmm10, %ymm7, %ymm7
	vmovsd	(%rbx), %xmm10
	movq	1312(%rsp), %rbx
	vmovhpd	(%rbx), %xmm10, %xmm10
	movq	1304(%rsp), %rbx
	vmovsd	(%rbx), %xmm13
	movq	1320(%rsp), %rbx
	vmovhpd	(%rbx), %xmm13, %xmm13
	movq	1288(%rsp), %rbx
	vinsertf64x2	$0x1, %xmm10, %ymm13, %ymm20
	vmovsd	8(%rax,%r15), %xmm10
	vmovsd	8(%rbx,%rcx,8), %xmm13
	movq	1280(%rsp), %rbx
	vmovhpd	8(%r14,%rsi), %xmm10, %xmm10
	vmovhpd	8(%r14,%r11), %xmm13, %xmm13
	vinsertf64x2	$0x1, %xmm10, %ymm13, %ymm27
	vmovsd	8(%rax,%r13), %xmm10
	vmovsd	8(%rbx,%rcx,8), %xmm13
	vmovhpd	8(%r14,%r12), %xmm10, %xmm10
	vmovhpd	8(%r14,%rdi), %xmm13, %xmm13
	vinsertf64x2	$0x1, %xmm10, %ymm13, %ymm29
	cmpl	%r8d, 1388(%rsp)
	jl	.L53
.L278:
	movq	1368(%rsp), %rbx
	vmovupd	-64(%rbx,%rdx), %ymm16
	movq	1360(%rsp), %rbx
	vmovupd	-64(%rbx,%rdx), %ymm10
	movq	1352(%rsp), %rbx
	vmovupd	-64(%rbx,%rdx), %ymm14
	movq	1376(%rsp), %rbx
	vmovupd	-64(%rbx,%rdx), %ymm13
	movq	1344(%rsp), %rbx
	vmovupd	-64(%rbx,%rdx), %ymm15
.L54:
	vaddpd	%ymm12, %ymm2, %ymm2
	vaddpd	%ymm21, %ymm0, %ymm0
	vaddpd	%ymm11, %ymm1, %ymm1
	vaddpd	%ymm25, %ymm26, %ymm11
	vaddpd	%ymm24, %ymm3, %ymm3
	leal	3(%rcx), %r8d
	vaddpd	%ymm22, %ymm2, %ymm21
	vaddpd	%ymm10, %ymm0, %ymm0
	vaddpd	%ymm4, %ymm11, %ymm4
	vaddpd	%ymm1, %ymm7, %ymm12
	vaddpd	%ymm16, %ymm3, %ymm3
	vaddpd	%ymm20, %ymm21, %ymm21
	vaddpd	%ymm13, %ymm0, %ymm0
	vaddpd	%ymm14, %ymm4, %ymm4
	vaddpd	%ymm27, %ymm12, %ymm12
	vaddpd	%ymm15, %ymm3, %ymm3
	vmulpd	.LC0(%rip), %ymm21, %ymm21
	vaddpd	%ymm5, %ymm0, %ymm0
	vaddpd	%ymm30, %ymm0, %ymm0
	vfmadd231pd	%ymm17, %ymm31, %ymm21
	vaddpd	%ymm23, %ymm0, %ymm0
	vaddpd	%ymm29, %ymm0, %ymm0
	vfmadd132pd	%ymm19, %ymm21, %ymm4
	vfmadd132pd	%ymm18, %ymm4, %ymm12
	vfmadd132pd	%ymm9, %ymm12, %ymm3
	vfmadd132pd	%ymm8, %ymm3, %ymm0
	vpermpd	$147, %ymm0, %ymm0
	vblendpd	$1, %ymm6, %ymm0, %ymm3
	vpermilpd	$5, %ymm6, %ymm6
	vmovapd	%ymm3, -128(%r9)
	cmpl	%r8d, 1388(%rsp)
	jl	.L55
	movq	1368(%rsp), %rbx
	vmovupd	-32(%rbx,%rdx), %ymm3
	movq	1360(%rsp), %rbx
	vmovupd	-32(%rbx,%rdx), %ymm4
	movq	1352(%rsp), %rbx
	vmovupd	-32(%rbx,%rdx), %ymm12
	movq	1376(%rsp), %rbx
	vmovupd	-32(%rbx,%rdx), %ymm21
	movq	1344(%rsp), %rbx
	vmovupd	-32(%rbx,%rdx), %ymm24
.L56:
	vaddpd	%ymm27, %ymm7, %ymm7
	vaddpd	%ymm23, %ymm5, %ymm5
	vaddpd	%ymm29, %ymm30, %ymm27
	vaddpd	%ymm1, %ymm4, %ymm1
	vaddpd	%ymm2, %ymm10, %ymm28
	leal	4(%rcx), %r8d
	vaddpd	%ymm31, %ymm7, %ymm23
	vaddpd	%ymm3, %ymm5, %ymm5
	vaddpd	%ymm22, %ymm27, %ymm22
	vaddpd	%ymm21, %ymm1, %ymm1
	vaddpd	%ymm13, %ymm28, %ymm28
	vaddpd	%ymm14, %ymm23, %ymm23
	vaddpd	%ymm24, %ymm5, %ymm5
	vaddpd	%ymm12, %ymm22, %ymm22
	vaddpd	%ymm26, %ymm1, %ymm1
	vmulpd	.LC0(%rip), %ymm23, %ymm23
	vaddpd	%ymm16, %ymm1, %ymm1
	vaddpd	%ymm25, %ymm1, %ymm1
	vfmadd231pd	%ymm17, %ymm20, %ymm23
	vaddpd	%ymm15, %ymm1, %ymm1
	vfmadd132pd	%ymm19, %ymm23, %ymm22
	vfmadd132pd	%ymm18, %ymm22, %ymm28
	vfmadd132pd	%ymm9, %ymm28, %ymm5
	vfmadd132pd	%ymm8, %ymm5, %ymm1
	vpermpd	$147, %ymm1, %ymm1
	vunpcklpd	%ymm1, %ymm0, %ymm0
	vblendpd	$1, %ymm6, %ymm1, %ymm1
	vmovapd	%ymm1, -96(%r9)
	cmpl	%r8d, 1388(%rsp)
	jl	.L57
	movq	1368(%rsp), %rbx
	vmovupd	(%rbx,%rdx), %ymm25
	movq	1360(%rsp), %rbx
	vmovupd	(%rbx,%rdx), %ymm5
	movq	1352(%rsp), %rbx
	vmovupd	(%rbx,%rdx), %ymm22
	movq	1376(%rsp), %rbx
	vmovupd	(%rbx,%rdx), %ymm28
	movq	1344(%rsp), %rbx
	vmovupd	(%rbx,%rdx), %ymm23
.L58:
	vaddpd	%ymm13, %ymm10, %ymm10
	vaddpd	%ymm2, %ymm5, %ymm1
	movl	%ecx, %r8d
	vaddpd	%ymm15, %ymm16, %ymm13
	vaddpd	%ymm7, %ymm4, %ymm26
	addl	$5, %r8d
	vaddpd	%ymm11, %ymm25, %ymm11
	vaddpd	%ymm10, %ymm20, %ymm2
	vaddpd	%ymm28, %ymm1, %ymm1
	vaddpd	%ymm31, %ymm13, %ymm13
	vaddpd	%ymm21, %ymm26, %ymm26
	vaddpd	%ymm23, %ymm11, %ymm11
	vaddpd	%ymm12, %ymm2, %ymm2
	vaddpd	%ymm30, %ymm1, %ymm1
	vaddpd	%ymm22, %ymm13, %ymm13
	vmulpd	.LC0(%rip), %ymm2, %ymm2
	vaddpd	%ymm3, %ymm1, %ymm1
	vaddpd	%ymm29, %ymm1, %ymm1
	vfmadd231pd	%ymm17, %ymm14, %ymm2
	vaddpd	%ymm24, %ymm1, %ymm1
	vfmadd132pd	%ymm19, %ymm2, %ymm13
	vfmadd132pd	%ymm18, %ymm13, %ymm26
	vfmadd132pd	%ymm9, %ymm26, %ymm11
	vfmadd132pd	%ymm8, %ymm11, %ymm1
	vblendpd	$8, %ymm1, %ymm0, %ymm0
	vblendpd	$8, %ymm6, %ymm1, %ymm1
	vpermilpd	$5, %ymm6, %ymm6
	vpermpd	$147, %ymm1, %ymm1
	vpermilpd	$6, %ymm0, %ymm0
	vmovapd	%ymm1, -64(%r9)
	cmpl	%r8d, 1388(%rsp)
	jl	.L59
	movq	1368(%rsp), %rbx
	vmovupd	32(%rbx,%rdx), %ymm11
	movq	1360(%rsp), %rbx
	vmovupd	32(%rbx,%rdx), %ymm1
	movq	1352(%rsp), %rbx
	vmovupd	32(%rbx,%rdx), %ymm26
	movq	1376(%rsp), %rbx
	vmovupd	32(%rbx,%rdx), %ymm2
	movq	1344(%rsp), %rbx
	vmovupd	32(%rbx,%rdx), %ymm13
.L60:
	vaddpd	%ymm7, %ymm1, %ymm1
	vaddpd	%ymm24, %ymm3, %ymm3
	movq	1256(%rsp), %rbx
	addq	$32, 1336(%rsp)
	vaddpd	%ymm10, %ymm5, %ymm5
	vaddpd	%ymm27, %ymm11, %ymm10
	addq	$32, 1328(%rsp)
	subq	$-128, %r9
	addq	$32, 1304(%rsp)
	subq	$-128, %rdx
	addq	$32, %r11
	addq	$32, %r15
	vaddpd	%ymm2, %ymm1, %ymm1
	vaddpd	%ymm21, %ymm4, %ymm2
	addq	$32, 1320(%rsp)
	addq	$32, %rsi
	vaddpd	%ymm20, %ymm3, %ymm3
	vaddpd	%ymm28, %ymm5, %ymm5
	addq	$32, 1296(%rsp)
	addq	$32, %rdi
	vaddpd	%ymm13, %ymm10, %ymm10
	addq	$32, 1312(%rsp)
	addq	$32, %r13
	addq	$32, %r12
	vaddpd	%ymm14, %ymm2, %ymm2
	vaddpd	%ymm16, %ymm1, %ymm1
	vaddpd	%ymm26, %ymm3, %ymm3
	vaddpd	%ymm22, %ymm2, %ymm2
	vaddpd	%ymm25, %ymm1, %ymm1
	vmulpd	.LC0(%rip), %ymm2, %ymm2
	vaddpd	%ymm15, %ymm1, %ymm1
	vaddpd	%ymm23, %ymm1, %ymm1
	vfmadd132pd	%ymm17, %ymm2, %ymm12
	vfmadd132pd	%ymm19, %ymm12, %ymm3
	vfmadd132pd	%ymm18, %ymm3, %ymm5
	vfmadd132pd	%ymm9, %ymm5, %ymm10
	vfmadd132pd	%ymm8, %ymm10, %ymm1
	vblendpd	$8, %ymm1, %ymm0, %ymm0
	vblendpd	$8, %ymm6, %ymm1, %ymm1
	vpermpd	$147, %ymm1, %ymm1
	vmovupd	%ymm0, (%rbx,%rcx,8)
	addq	$4, %rcx
	vmovapd	%ymm1, -160(%r9)
	cmpl	%ecx, 1224(%rsp)
	jl	.L48
.L61:
	movq	1168(%rsp), %r10
	leal	-2(%rcx), %r8d
	movl	%ecx, 1056(%rsp)
	vmovupd	(%r10,%rcx,8), %ymm6
	cmpl	$3, %r8d
	jg	.L277
	movq	1160(%rsp), %rbx
	movq	1080(%rsp), %r8
	vmovsd	16(%rax,%rbx,8), %xmm0
	movq	992(%rsp), %rbx
	vmovhpd	16(%r8), %xmm0, %xmm0
	movq	864(%rsp), %r8
	vmovsd	(%rax,%rbx,8), %xmm3
	movq	1104(%rsp), %rbx
	vmovsd	(%rax,%r8,8), %xmm1
	movq	1144(%rsp), %r8
	vmovhpd	16(%rbx), %xmm3, %xmm3
	movq	880(%rsp), %rbx
	vmovhpd	16(%r8), %xmm1, %xmm1
	movslq	832(%rsp), %r8
	vinsertf128	$0x1, %xmm0, %ymm3, %ymm3
	vmovsd	(%rax,%rbx,8), %xmm0
	movq	1120(%rsp), %rbx
	movq	%r8, %r10
	movq	1272(%rsp), %r8
	vmovhpd	16(%rbx), %xmm0, %xmm0
	imulq	%r8, %r10
	movslq	824(%rsp), %r8
	vinsertf128	$0x1, %xmm1, %ymm0, %ymm0
	movq	%r8, %rbx
	movq	1272(%rsp), %r8
	vmovsd	16(%rax,%r10,8), %xmm4
	movq	%r10, 1056(%rsp)
	imulq	%r8, %rbx
	movq	1136(%rsp), %r8
	vmovsd	16(%rax,%rbx,8), %xmm1
	movq	%rbx, 1072(%rsp)
	vmovhpd	16(%r8), %xmm1, %xmm1
	movq	1152(%rsp), %r8
	vmovhpd	16(%r8), %xmm4, %xmm4
	movslq	1008(%rsp), %r8
	vinsertf128	$0x1, %xmm1, %ymm4, %ymm4
	movq	%r8, %r10
	movq	1272(%rsp), %r8
	imulq	%r8, %r10
	movslq	848(%rsp), %r8
	movq	%r8, %rbx
	movq	1272(%rsp), %r8
	imulq	%r8, %rbx
	movq	%rbx, 1040(%rsp)
	vmovsd	16(%rax,%rbx,8), %xmm1
	movq	1112(%rsp), %rbx
	vmovsd	16(%rax,%r10,8), %xmm2
	movq	%r10, 1048(%rsp)
	movq	1128(%rsp), %r10
	vmovhpd	16(%rbx), %xmm1, %xmm1
	movq	1016(%rsp), %rbx
	vmovhpd	16(%r10), %xmm2, %xmm2
	movq	%r8, %r10
	imulq	%r8, %rbx
	movslq	840(%rsp), %r8
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm21
	imulq	%r10, %r8
	movq	1088(%rsp), %r10
	vmovsd	16(%rax,%rbx,8), %xmm2
	movq	%rbx, 1032(%rsp)
	movq	1096(%rsp), %rbx
	vmovsd	16(%rax,%r8,8), %xmm1
	vmovhpd	16(%rbx), %xmm2, %xmm2
	vmovhpd	16(%r10), %xmm1, %xmm1
	movq	1160(%rsp), %r10
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm24
	vmovsd	24(%rax,%r10,8), %xmm1
	movq	1080(%rsp), %r10
	vmovhpd	24(%r10), %xmm1, %xmm1
	movq	1000(%rsp), %r10
	vmovsd	24(%rax,%r10,8), %xmm5
	movq	1104(%rsp), %r10
	vmovhpd	24(%r10), %xmm5, %xmm5
	movq	872(%rsp), %r10
	vinsertf128	$0x1, %xmm1, %ymm5, %ymm5
	vmovsd	24(%rax,%r10,8), %xmm1
	movq	1144(%rsp), %r10
	vmovhpd	24(%r10), %xmm1, %xmm1
	movq	888(%rsp), %r10
	vmovsd	24(%rax,%r10,8), %xmm11
	movq	1120(%rsp), %r10
	vmovhpd	24(%r10), %xmm11, %xmm11
	movq	1072(%rsp), %rbx
	movq	1056(%rsp), %r10
	vinsertf128	$0x1, %xmm1, %ymm11, %ymm11
	vmovsd	24(%rax,%rbx,8), %xmm1
	movq	1136(%rsp), %rbx
	vmovsd	24(%rax,%r10,8), %xmm2
	movq	1152(%rsp), %r10
	vmovhpd	24(%rbx), %xmm1, %xmm1
	movq	1040(%rsp), %rbx
	vmovhpd	24(%r10), %xmm2, %xmm2
	movq	1048(%rsp), %r10
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm22
	vmovsd	24(%rax,%rbx,8), %xmm2
	movq	1112(%rsp), %rbx
	vmovsd	24(%rax,%r10,8), %xmm1
	movq	1128(%rsp), %r10
	vmovhpd	24(%rbx), %xmm2, %xmm2
	movq	1032(%rsp), %rbx
	vmovhpd	24(%r10), %xmm1, %xmm1
	vmovsd	24(%rax,%rbx,8), %xmm7
	movq	1096(%rsp), %rbx
	vinsertf128	$0x1, %xmm2, %ymm1, %ymm1
	vmovsd	24(%rax,%r8,8), %xmm2
	movq	1088(%rsp), %r8
	vmovhpd	24(%rbx), %xmm7, %xmm7
	movq	1368(%rsp), %rbx
	vmovhpd	24(%r8), %xmm2, %xmm2
	vmovupd	-128(%rbx,%rdx), %ymm26
	movq	1360(%rsp), %rbx
	vinsertf64x2	$0x1, %xmm2, %ymm7, %ymm23
	vmovupd	-128(%rbx,%rdx), %ymm12
	movq	1352(%rsp), %rbx
	vmovupd	-128(%rbx,%rdx), %ymm31
	movq	1376(%rsp), %rbx
	vmovupd	-128(%rbx,%rdx), %ymm2
	movq	1344(%rsp), %rbx
	vmovupd	-128(%rbx,%rdx), %ymm25
.L50:
	movq	1368(%rsp), %rbx
	leal	2(%rcx), %r8d
	vmovupd	-96(%rbx,%rdx), %ymm30
	movq	1360(%rsp), %rbx
	vmovupd	-96(%rbx,%rdx), %ymm7
	movq	1352(%rsp), %rbx
	vmovupd	-96(%rbx,%rdx), %ymm20
	movq	1376(%rsp), %rbx
	vmovupd	-96(%rbx,%rdx), %ymm27
	movq	1344(%rsp), %rbx
	vmovupd	-96(%rbx,%rdx), %ymm29
	cmpl	%r8d, 1388(%rsp)
	jge	.L278
.L53:
	movq	1336(%rsp), %rbx
	vmovsd	16(%rax,%rsi), %xmm10
	vmovsd	16(%rax,%r11), %xmm13
	vmovsd	16(%rax,%r15), %xmm15
	vmovhpd	8(%rbx), %xmm10, %xmm10
	movq	1328(%rsp), %rbx
	vmovhpd	16(%r14,%r15), %xmm13, %xmm13
	vmovhpd	16(%r14,%rsi), %xmm15, %xmm15
	vinsertf64x2	$0x1, %xmm10, %ymm13, %ymm16
	vmovsd	16(%rax,%r12), %xmm13
	vmovsd	16(%rax,%rdi), %xmm10
	vmovhpd	8(%rbx), %xmm13, %xmm13
	movq	1296(%rsp), %rbx
	vmovhpd	16(%r14,%r13), %xmm10, %xmm10
	vinsertf128	$0x1, %xmm13, %ymm10, %ymm10
	vmovsd	8(%rbx), %xmm13
	movq	1312(%rsp), %rbx
	vmovhpd	8(%rbx), %xmm13, %xmm13
	movq	1304(%rsp), %rbx
	vmovsd	8(%rbx), %xmm14
	movq	1320(%rsp), %rbx
	vmovhpd	8(%rbx), %xmm14, %xmm14
	movq	1288(%rsp), %rbx
	vinsertf128	$0x1, %xmm13, %ymm14, %ymm14
	vmovsd	16(%rbx,%rcx,8), %xmm13
	movq	1280(%rsp), %rbx
	vmovhpd	16(%r14,%r11), %xmm13, %xmm13
	vmovsd	16(%rbx,%rcx,8), %xmm28
	vinsertf128	$0x1, %xmm15, %ymm13, %ymm13
	vmovsd	16(%rax,%r13), %xmm15
	vmovhpd	16(%r14,%r12), %xmm15, %xmm15
	vmovapd	%xmm15, 1056(%rsp)
	vmovsd	16(%r14,%rdi), %xmm15
	vunpcklpd	%xmm15, %xmm28, %xmm28
	vinsertf64x2	$0x1, 1056(%rsp), %ymm28, %ymm15
	jmp	.L54
	.p2align 4,,10
	.p2align 3
.L14:
	movq	1336(%rsp), %rbx
	movq	960(%rsp), %rcx
	movq	896(%rsp), %rax
	movq	928(%rsp), %rdi
	movq	1008(%rsp), %rdx
	movq	1016(%rsp), %r11
	leaq	16(%rcx,%rbx), %rsi
	leaq	16(%r15,%rbx), %rcx
	vmovsd	.LC9(%rip), %xmm3
	vmovsd	.LC11(%rip), %xmm2
	leaq	16(%rax,%rbx), %rax
	leaq	16(%rdi,%rbx), %rdi
	movq	1152(%rsp), %r9
	leaq	16(%rdx,%rbx), %rdx
	leaq	32(%r11,%rbx), %r8
	.p2align 4,,10
	.p2align 3
.L11:
	vmovsd	16(%rsi), %xmm0
	vmovsd	16(%rdx), %xmm1
	addq	$8, %rax
	addq	$8, %rdi
	addq	$8, %rsi
	addq	$8, %rcx
	addq	$8, %rdx
	addq	$8, %r8
	vaddsd	8(%rdi), %xmm0, %xmm0
	vaddsd	8(%rcx), %xmm1, %xmm1
	vaddsd	(%rax), %xmm0, %xmm0
	vaddsd	-8(%rax), %xmm1, %xmm1
	vaddsd	16(%rax), %xmm0, %xmm0
	vaddsd	24(%rax), %xmm1, %xmm1
	vmulsd	%xmm7, %xmm0, %xmm0
	vfmadd231sd	8(%rax), %xmm6, %xmm0
	vfmadd132sd	%xmm5, %xmm0, %xmm1
	vmovsd	(%rsi), %xmm0
	vaddsd	(%rdi), %xmm0, %xmm14
	vmovsd	-8(%rdx), %xmm0
	vaddsd	16(%rdi), %xmm14, %xmm14
	vaddsd	16(%rsi), %xmm14, %xmm14
	vfmadd132sd	%xmm4, %xmm1, %xmm14
	vaddsd	-8(%rcx), %xmm0, %xmm1
	vmovsd	-8(%rsi), %xmm0
	vaddsd	-8(%rdi), %xmm0, %xmm0
	vaddsd	24(%rcx), %xmm1, %xmm1
	vaddsd	24(%rdi), %xmm0, %xmm0
	vaddsd	24(%rdx), %xmm1, %xmm1
	vaddsd	24(%rsi), %xmm0, %xmm0
	vfmadd132sd	%xmm3, %xmm14, %xmm1
	vaddsd	(%rcx), %xmm0, %xmm0
	vaddsd	16(%rcx), %xmm0, %xmm0
	vaddsd	(%rdx), %xmm0, %xmm0
	vaddsd	16(%rdx), %xmm0, %xmm0
	vfmadd132sd	%xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, -8(%r8)
	cmpq	%rax, %r9
	jne	.L11
	jmp	.L12
.L143:
	movl	$4, 1320(%rsp)
	movl	1384(%rsp), %r11d
	xorl	%edx, %edx
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L59:
	movq	1336(%rsp), %rbx
	vmovsd	40(%rax,%rsi), %xmm1
	vmovsd	40(%rax,%r11), %xmm11
	vmovsd	40(%rax,%r12), %xmm2
	vmovhpd	32(%rbx), %xmm1, %xmm1
	movq	1328(%rsp), %rbx
	vmovsd	40(%r14,%rdi), %xmm30
	vmovhpd	40(%r14,%r15), %xmm11, %xmm11
	vinsertf128	$0x1, %xmm1, %ymm11, %ymm11
	vmovsd	40(%rax,%rdi), %xmm1
	vmovhpd	32(%rbx), %xmm2, %xmm2
	movq	1296(%rsp), %rbx
	vmovhpd	40(%r14,%r13), %xmm1, %xmm1
	vinsertf128	$0x1, %xmm2, %ymm1, %ymm1
	vmovsd	32(%rbx), %xmm2
	movq	1312(%rsp), %rbx
	vmovhpd	32(%rbx), %xmm2, %xmm2
	movq	1304(%rsp), %rbx
	vmovsd	32(%rbx), %xmm13
	movq	1320(%rsp), %rbx
	vmovhpd	32(%rbx), %xmm13, %xmm13
	movq	1288(%rsp), %rbx
	vinsertf64x2	$0x1, %xmm2, %ymm13, %ymm26
	vmovsd	40(%rax,%r15), %xmm13
	vmovsd	40(%rbx,%rcx,8), %xmm2
	movq	1280(%rsp), %rbx
	vmovhpd	40(%r14,%rsi), %xmm13, %xmm13
	vmovhpd	40(%r14,%r11), %xmm2, %xmm2
	vmovsd	40(%rbx,%rcx,8), %xmm29
	vinsertf128	$0x1, %xmm13, %ymm2, %ymm2
	vmovsd	40(%rax,%r13), %xmm13
	vunpcklpd	%xmm30, %xmm29, %xmm29
	vmovhpd	40(%r14,%r12), %xmm13, %xmm13
	vinsertf64x2	$0x1, %xmm13, %ymm29, %ymm13
	jmp	.L60
	.p2align 4,,10
	.p2align 3
.L57:
	movq	1336(%rsp), %rbx
	vmovsd	32(%rax,%rsi), %xmm1
	vmovsd	32(%rax,%r11), %xmm5
	vmovhpd	24(%rbx), %xmm1, %xmm1
	movq	1328(%rsp), %rbx
	vmovhpd	32(%r14,%r15), %xmm5, %xmm5
	vinsertf64x2	$0x1, %xmm1, %ymm5, %ymm25
	vmovsd	32(%rax,%r12), %xmm1
	vmovsd	32(%rax,%rdi), %xmm5
	vmovhpd	24(%rbx), %xmm1, %xmm1
	movq	1296(%rsp), %rbx
	vmovhpd	32(%r14,%r13), %xmm5, %xmm5
	vinsertf128	$0x1, %xmm1, %ymm5, %ymm5
	vmovsd	24(%rbx), %xmm1
	movq	1312(%rsp), %rbx
	vmovhpd	24(%rbx), %xmm1, %xmm1
	movq	1304(%rsp), %rbx
	vmovsd	24(%rbx), %xmm22
	movq	1320(%rsp), %rbx
	vmovsd	24(%rbx), %xmm26
	movq	1288(%rsp), %rbx
	vunpcklpd	%xmm26, %xmm22, %xmm22
	vmovsd	32(%rbx,%rcx,8), %xmm28
	vmovsd	32(%r14,%r11), %xmm26
	vinsertf64x2	$0x1, %xmm1, %ymm22, %ymm22
	vmovsd	32(%rax,%r15), %xmm1
	movq	1280(%rsp), %rbx
	vunpcklpd	%xmm26, %xmm28, %xmm28
	vmovsd	32(%r14,%rdi), %xmm26
	vmovhpd	32(%r14,%rsi), %xmm1, %xmm1
	vmovsd	32(%rbx,%rcx,8), %xmm23
	vinsertf64x2	$0x1, %xmm1, %ymm28, %ymm28
	vmovsd	32(%rax,%r13), %xmm1
	vunpcklpd	%xmm26, %xmm23, %xmm23
	vmovhpd	32(%r14,%r12), %xmm1, %xmm1
	vinsertf64x2	$0x1, %xmm1, %ymm23, %ymm23
	jmp	.L58
	.p2align 4,,10
	.p2align 3
.L55:
	movq	1336(%rsp), %rbx
	vmovsd	24(%rax,%rsi), %xmm4
	vmovsd	24(%rax,%r11), %xmm3
	vmovsd	24(%rax,%r12), %xmm12
	vmovhpd	16(%rbx), %xmm4, %xmm4
	movq	1328(%rsp), %rbx
	vmovsd	24(%r14,%rsi), %xmm28
	vmovhpd	24(%r14,%r15), %xmm3, %xmm3
	vinsertf128	$0x1, %xmm4, %ymm3, %ymm3
	vmovsd	24(%rax,%rdi), %xmm4
	vmovhpd	16(%rbx), %xmm12, %xmm12
	movq	1296(%rsp), %rbx
	vmovhpd	24(%r14,%r13), %xmm4, %xmm4
	vinsertf128	$0x1, %xmm12, %ymm4, %ymm4
	vmovsd	16(%rbx), %xmm12
	movq	1312(%rsp), %rbx
	vmovhpd	16(%rbx), %xmm12, %xmm12
	movq	1304(%rsp), %rbx
	vmovsd	16(%rbx), %xmm21
	movq	1320(%rsp), %rbx
	vmovsd	16(%rbx), %xmm24
	movq	1288(%rsp), %rbx
	vunpcklpd	%xmm24, %xmm21, %xmm21
	vmovsd	24(%rax,%r15), %xmm24
	vinsertf64x2	$0x1, %xmm12, %ymm21, %ymm12
	vmovsd	24(%rbx,%rcx,8), %xmm21
	movq	1280(%rsp), %rbx
	vunpcklpd	%xmm28, %xmm24, %xmm24
	vmovsd	24(%r14,%r11), %xmm28
	vunpcklpd	%xmm28, %xmm21, %xmm21
	vmovsd	24(%rax,%r13), %xmm28
	vinsertf64x2	$0x1, %xmm24, %ymm21, %ymm21
	vmovsd	24(%r14,%r12), %xmm24
	vunpcklpd	%xmm24, %xmm28, %xmm28
	vmovsd	24(%rbx,%rcx,8), %xmm24
	vmovapd	%xmm28, 1056(%rsp)
	vmovsd	24(%r14,%rdi), %xmm28
	vunpcklpd	%xmm28, %xmm24, %xmm24
	vinsertf64x2	$0x1, 1056(%rsp), %ymm24, %ymm24
	jmp	.L56
	.p2align 4,,10
	.p2align 3
.L30:
	movq	1328(%rsp), %rbx
	movq	960(%rsp), %rax
	movq	928(%rsp), %rdi
	movq	896(%rsp), %rsi
	movq	1000(%rsp), %rdx
	movq	992(%rsp), %r11
	leaq	16(%rbx,%rax), %rax
	leaq	16(%r14,%rbx), %rcx
	vmovsd	.LC9(%rip), %xmm5
	vmovsd	.LC11(%rip), %xmm4
	leaq	16(%rbx,%rdi), %rdi
	leaq	16(%rbx,%rsi), %rsi
	movq	1136(%rsp), %r9
	leaq	16(%rbx,%rdx), %rdx
	leaq	32(%rbx,%r11), %r8
	.p2align 4,,10
	.p2align 3
.L27:
	vmovsd	16(%rdi), %xmm0
	vmovsd	16(%rcx), %xmm2
	addq	$8, %rax
	addq	$8, %rdi
	addq	$8, %rsi
	addq	$8, %rcx
	addq	$8, %rdx
	addq	$8, %r8
	vaddsd	8(%rsi), %xmm0, %xmm0
	vaddsd	8(%rdx), %xmm2, %xmm2
	vaddsd	(%rax), %xmm0, %xmm0
	vaddsd	-8(%rax), %xmm2, %xmm2
	vaddsd	16(%rax), %xmm0, %xmm0
	vaddsd	24(%rax), %xmm2, %xmm2
	vmulsd	%xmm9, %xmm0, %xmm0
	vfmadd231sd	8(%rax), %xmm8, %xmm0
	vfmadd132sd	%xmm7, %xmm0, %xmm2
	vmovsd	(%rdi), %xmm0
	vaddsd	(%rsi), %xmm0, %xmm3
	vmovsd	-8(%rcx), %xmm0
	vaddsd	16(%rdi), %xmm3, %xmm3
	vaddsd	16(%rsi), %xmm3, %xmm3
	vfmadd132sd	%xmm6, %xmm2, %xmm3
	vaddsd	-8(%rdx), %xmm0, %xmm2
	vmovsd	-8(%rdi), %xmm0
	vaddsd	-8(%rsi), %xmm0, %xmm0
	vaddsd	24(%rcx), %xmm2, %xmm2
	vaddsd	24(%rdi), %xmm0, %xmm0
	vaddsd	24(%rdx), %xmm2, %xmm2
	vaddsd	24(%rsi), %xmm0, %xmm0
	vfmadd132sd	%xmm5, %xmm3, %xmm2
	vaddsd	(%rcx), %xmm0, %xmm0
	vaddsd	16(%rcx), %xmm0, %xmm0
	vaddsd	(%rdx), %xmm0, %xmm0
	vaddsd	16(%rdx), %xmm0, %xmm0
	vfmadd132sd	%xmm4, %xmm2, %xmm0
	vmovsd	%xmm0, -8(%r8)
	cmpq	%r9, %rax
	jne	.L27
	jmp	.L28
	.p2align 4,,10
	.p2align 3
.L48:
	movl	708(%rsp), %edi
	cmpl	%edi, 1024(%rsp)
	jl	.L41
	movl	1388(%rsp), %eax
	cmpl	%edi, %eax
	jl	.L65
	movl	1024(%rsp), %ecx
	leal	1(%rdi), %edx
	movq	528(%rsp), %rdi
	movq	816(%rsp), %rsi
	cmpl	%eax, %ecx
	cmovle	%ecx, %eax
	movq	792(%rsp), %rcx
	movl	%eax, 1168(%rsp)
	movslq	%edx, %rax
	movq	1232(%rsp), %rdx
	movq	%rax, 1304(%rsp)
	movq	696(%rsp), %rax
	leaq	8(%rax,%rdi), %r13
	movq	520(%rsp), %rdi
	leaq	8(%rax,%rdi), %r12
	movq	512(%rsp), %rdi
	leaq	8(%rax,%rdi), %rbx
	movq	504(%rsp), %rdi
	leaq	8(%rax,%rdi), %r15
	movq	496(%rsp), %rdi
	leaq	8(%rax,%rdi), %r11
	movq	488(%rsp), %rdi
	leaq	8(%rax,%rdi), %rdi
	movq	%rdi, 1320(%rsp)
	movq	480(%rsp), %rdi
	leaq	8(%rax,%rdi), %r10
	movq	472(%rsp), %rdi
	leaq	8(%rax,%rdi), %rdi
	movq	%rdi, 1328(%rsp)
	movq	464(%rsp), %rdi
	leaq	8(%rax,%rdi), %r9
	movq	456(%rsp), %rdi
	leaq	8(%rax,%rdi), %rdi
	movq	%rdi, 1336(%rsp)
	movq	448(%rsp), %rdi
	leaq	8(%rax,%rdi), %r8
	movq	440(%rsp), %rdi
	leaq	8(%rax,%rdi), %rdi
	movq	%rdi, 1152(%rsp)
	movq	1272(%rsp), %rdi
	imulq	%rdi, %rcx
	movq	536(%rsp), %rdi
	leaq	1(%rdi,%rcx), %rcx
	leaq	(%rsi,%rcx,8), %rdi
	movq	1016(%rsp), %rcx
	imulq	%rdx, %rcx
	addq	%rsi, %rcx
	movq	%rcx, 1312(%rsp)
	movq	552(%rsp), %rcx
	addq	%rax, %rcx
	movq	%rcx, 1280(%rsp)
	movq	544(%rsp), %rcx
	addq	%rax, %rcx
	movq	%rcx, 1288(%rsp)
	movq	584(%rsp), %rcx
	addq	%rcx, %rax
	movq	%rax, 1296(%rsp)
	movq	576(%rsp), %rcx
	movq	1304(%rsp), %rdx
	movq	1152(%rsp), %rsi
	movq	%r14, 1160(%rsp)
	.p2align 4,,10
	.p2align 3
.L74:
	leal	-3(%rdx), %r14d
	movl	%edx, %eax
	cmpl	$3, %r14d
	jle	.L66
	cmpl	%r14d, 1388(%rsp)
	jl	.L66
	movq	1368(%rsp), %r14
	vmovupd	-192(%r14,%rcx), %ymm3
	movq	1360(%rsp), %r14
	vmovupd	-192(%r14,%rcx), %ymm0
	movq	1352(%rsp), %r14
	vmovupd	-192(%r14,%rcx), %ymm25
	movq	1376(%rsp), %r14
	vmovupd	-192(%r14,%rcx), %ymm20
	movq	1344(%rsp), %r14
	vmovupd	-192(%r14,%rcx), %ymm23
.L67:
	leal	-2(%rax), %r14d
	cmpl	$3, %r14d
	jle	.L68
	cmpl	%r14d, 1388(%rsp)
	jl	.L68
	movq	1368(%rsp), %r14
	vmovupd	-160(%r14,%rcx), %ymm14
	movq	1360(%rsp), %r14
	vmovupd	-160(%r14,%rcx), %ymm4
	movq	1352(%rsp), %r14
	vmovupd	-160(%r14,%rcx), %ymm7
	movq	1376(%rsp), %r14
	vmovupd	-160(%r14,%rcx), %ymm29
	movq	1344(%rsp), %r14
	vmovupd	-160(%r14,%rcx), %ymm11
.L69:
	movq	1368(%rsp), %r14
	movl	%edx, 1304(%rsp)
	vmovupd	-128(%r14,%rcx), %ymm2
	movq	1360(%rsp), %r14
	vmovupd	-128(%r14,%rcx), %ymm1
	movq	1352(%rsp), %r14
	vmovupd	-128(%r14,%rcx), %ymm5
	movq	1376(%rsp), %r14
	vmovupd	-128(%r14,%rcx), %ymm12
	movq	1344(%rsp), %r14
	vmovupd	-128(%r14,%rcx), %ymm28
	cmpl	%edx, 1388(%rsp)
	jl	.L70
	movq	1368(%rsp), %r14
	vmovupd	-96(%r14,%rcx), %ymm13
	movq	1360(%rsp), %r14
	vmovupd	-96(%r14,%rcx), %ymm27
	movq	1352(%rsp), %r14
	vmovupd	-96(%r14,%rcx), %ymm6
	movq	1376(%rsp), %r14
	vmovupd	-96(%r14,%rcx), %ymm26
	movq	1344(%rsp), %r14
	vmovupd	-96(%r14,%rcx), %ymm10
.L71:
	incl	%eax
	cmpl	%eax, 1388(%rsp)
	jl	.L72
	movq	1368(%rsp), %rax
	vmovupd	-64(%rax,%rcx), %ymm22
	movq	1360(%rsp), %rax
	vmovupd	-64(%rax,%rcx), %ymm16
	movq	1352(%rsp), %rax
	vmovupd	-64(%rax,%rcx), %ymm24
	movq	1376(%rsp), %rax
	vmovupd	-64(%rax,%rcx), %ymm15
	movq	1344(%rsp), %rax
	vmovupd	-64(%rax,%rcx), %ymm21
.L73:
	vaddpd	%ymm12, %ymm1, %ymm1
	vaddpd	%ymm28, %ymm2, %ymm2
	movq	1280(%rsp), %rax
	movl	1168(%rsp), %r14d
	vaddpd	%ymm20, %ymm0, %ymm0
	vaddpd	%ymm29, %ymm4, %ymm4
	addq	$8, 1320(%rsp)
	addq	$8, %r13
	vaddpd	%ymm23, %ymm3, %ymm3
	addq	$8, 1328(%rsp)
	addq	$8, %r12
	addq	$8, %rbx
	vaddpd	%ymm7, %ymm1, %ymm1
	vaddpd	%ymm25, %ymm2, %ymm2
	addq	$8, 1336(%rsp)
	addq	$8, %r15
	vaddpd	%ymm16, %ymm0, %ymm0
	vaddpd	%ymm27, %ymm4, %ymm4
	addq	$8, %r11
	addq	$8, %r10
	vaddpd	%ymm22, %ymm3, %ymm3
	addq	$8, %r9
	addq	$8, %r8
	addq	$8, %rsi
	vaddpd	%ymm6, %ymm1, %ymm1
	vaddpd	%ymm24, %ymm2, %ymm2
	addq	$8, %rdi
	addq	$32, %rcx
	vaddpd	%ymm15, %ymm0, %ymm0
	vaddpd	%ymm26, %ymm4, %ymm4
	vaddpd	%ymm21, %ymm3, %ymm3
	vmulpd	.LC0(%rip), %ymm1, %ymm1
	vaddpd	%ymm14, %ymm0, %ymm0
	vaddpd	%ymm13, %ymm0, %ymm0
	vfmadd231pd	%ymm17, %ymm5, %ymm1
	vaddpd	%ymm11, %ymm0, %ymm0
	vfmadd132pd	%ymm19, %ymm1, %ymm2
	vaddpd	%ymm10, %ymm0, %ymm0
	vfmadd132pd	%ymm18, %ymm2, %ymm4
	vfmadd132pd	%ymm9, %ymm4, %ymm3
	vfmadd132pd	%ymm8, %ymm3, %ymm0
	vextractf64x2	$1, %ymm0, %xmm1
	vmovlpd	%xmm0, -8(%rax,%rdx,8)
	movq	1288(%rsp), %rax
	vmovhpd	%xmm0, -8(%rax,%rdx,8)
	movq	1296(%rsp), %rax
	valignq	$3, %ymm0, %ymm0, %ymm0
	vmovsd	%xmm1, -8(%rax,%rdx,8)
	movq	1256(%rsp), %rax
	vmovsd	%xmm0, -8(%rax,%rdx,8)
	incq	%rdx
	leal	-1(%rdx), %eax
	cmpl	%r14d, %eax
	jle	.L74
	movl	1304(%rsp), %eax
	movq	1160(%rsp), %r14
	cmpl	%eax, 1024(%rsp)
	jl	.L41
.L62:
	movslq	%eax, %rdx
	movq	768(%rsp), %rax
	movq	680(%rsp), %rdi
	vmovapd	%ymm9, %ymm30
	movq	792(%rsp), %r13
	movq	%rdx, %rcx
	vmovapd	%ymm8, %ymm31
	leaq	(%rdx,%rax), %r12
	movq	760(%rsp), %rax
	salq	$5, %rcx
	movq	%rcx, 1328(%rsp)
	salq	$3, %r12
	leaq	(%rdx,%rax), %rsi
	movq	712(%rsp), %rax
	leaq	0(,%rsi,8), %r15
	leaq	(%rax,%rdi), %rsi
	movq	800(%rsp), %rax
	leaq	(%rdx,%rsi), %rbx
	leaq	(%rdx,%rax), %rdi
	salq	$3, %rbx
	leaq	0(,%rdi,8), %rax
	movq	%rax, 1336(%rsp)
	movq	752(%rsp), %rax
	leaq	(%rdx,%rax), %rdi
	movq	744(%rsp), %rax
	salq	$3, %rdi
	leaq	(%rdx,%rax), %r8
	movq	776(%rsp), %rax
	movq	%rdi, 1288(%rsp)
	salq	$3, %r8
	subq	%rax, %rsi
	movq	736(%rsp), %rax
	movq	%r8, 1320(%rsp)
	addq	%rdx, %rsi
	leaq	(%rdx,%rax), %r9
	movq	728(%rsp), %rax
	salq	$3, %rsi
	salq	$3, %r9
	leaq	(%rdx,%rax), %r10
	movq	720(%rsp), %rax
	movq	%r9, 1312(%rsp)
	salq	$3, %r10
	leaq	(%rdx,%rax), %r11
	movq	1232(%rsp), %rax
	movq	%r10, 1304(%rsp)
	salq	$3, %r11
	imulq	%rax, %r13
	movq	816(%rsp), %rax
	movq	%r11, 1296(%rsp)
	leaq	(%rax,%r13), %rcx
	movq	1232(%rsp), %r13
	movq	%rcx, 1168(%rsp)
	movq	1016(%rsp), %rcx
	imulq	%r13, %rcx
	movq	568(%rsp), %r13
	leaq	(%rax,%rcx), %rcx
	movq	%rcx, 1280(%rsp)
	movq	696(%rsp), %rcx
	addq	%rcx, %r13
	movq	%r13, 1152(%rsp)
	movq	560(%rsp), %r13
	addq	%r13, %rcx
	movq	%rcx, 1160(%rsp)
	movq	1328(%rsp), %rcx
	movq	%r15, 1328(%rsp)
	vmovapd	640(%rsp), %ymm29
	jmp	.L84
	.p2align 4,,10
	.p2align 3
.L279:
	movq	1368(%rsp), %r15
	vmovupd	-96(%r15,%rcx), %ymm14
	movq	1360(%rsp), %r15
	vmovupd	-96(%r15,%rcx), %ymm27
	movq	1352(%rsp), %r15
	vmovupd	-96(%r15,%rcx), %ymm10
	movq	1376(%rsp), %r15
	vmovupd	-96(%r15,%rcx), %ymm26
	movq	1344(%rsp), %r15
	vmovupd	-96(%r15,%rcx), %ymm12
.L81:
	movl	%edx, %r13d
	addl	$2, %r13d
	cmpl	%r13d, 1388(%rsp)
	jl	.L82
	movq	1368(%rsp), %r15
	vmovupd	-64(%r15,%rcx), %ymm22
	movq	1360(%rsp), %r15
	vmovupd	-64(%r15,%rcx), %ymm20
	movq	1352(%rsp), %r15
	vmovupd	-64(%r15,%rcx), %ymm24
	movq	1376(%rsp), %r15
	vmovupd	-64(%r15,%rcx), %ymm16
	movq	1344(%rsp), %r15
	vmovupd	-64(%r15,%rcx), %ymm8
.L83:
	vaddpd	%ymm6, %ymm2, %ymm2
	vaddpd	%ymm7, %ymm1, %ymm1
	movq	1328(%rsp), %r15
	movq	1336(%rsp), %r13
	vaddpd	%ymm21, %ymm0, %ymm0
	vaddpd	%ymm28, %ymm4, %ymm4
	incq	%rdx
	addq	$8, 1288(%rsp)
	vaddpd	%ymm23, %ymm3, %ymm3
	addq	$8, 1320(%rsp)
	addq	$32, %rcx
	addq	$8, %rdi
	vaddpd	%ymm11, %ymm2, %ymm2
	vaddpd	%ymm25, %ymm1, %ymm1
	addq	$8, 1312(%rsp)
	addq	$8, %r8
	vaddpd	%ymm20, %ymm0, %ymm0
	vaddpd	%ymm27, %ymm4, %ymm4
	addq	$8, 1304(%rsp)
	addq	$8, %rsi
	vaddpd	%ymm22, %ymm3, %ymm3
	addq	$8, 1296(%rsp)
	addq	$8, %r9
	addq	$8, %r10
	vaddpd	%ymm10, %ymm2, %ymm2
	vaddpd	%ymm24, %ymm1, %ymm1
	addq	$8, %r11
	vaddpd	%ymm16, %ymm0, %ymm0
	vaddpd	%ymm26, %ymm4, %ymm4
	vaddpd	%ymm8, %ymm3, %ymm3
	vmulpd	896(%rsp), %ymm2, %ymm2
	vaddpd	%ymm15, %ymm0, %ymm0
	vaddpd	%ymm14, %ymm0, %ymm0
	vfmadd132pd	928(%rsp), %ymm2, %ymm5
	vaddpd	%ymm13, %ymm0, %ymm0
	vfmadd132pd	960(%rsp), %ymm5, %ymm1
	vaddpd	%ymm12, %ymm0, %ymm0
	vfmadd132pd	%ymm29, %ymm1, %ymm4
	vfmadd132pd	%ymm30, %ymm4, %ymm3
	vfmadd132pd	%ymm31, %ymm3, %ymm0
	vextractf64x2	$1, %ymm0, %xmm1
	vmovlpd	%xmm0, (%r14,%r12)
	addq	$8, %r12
	vmovhpd	%xmm0, (%rax,%r15)
	addq	$8, %r15
	valignq	$3, %ymm0, %ymm0, %ymm0
	movq	%r15, 1328(%rsp)
	vmovsd	%xmm1, (%r14,%rbx)
	addq	$8, %rbx
	vmovsd	%xmm0, (%rax,%r13)
	addq	$8, %r13
	movq	%r13, 1336(%rsp)
	cmpl	%edx, 1024(%rsp)
	jl	.L41
.L84:
	leal	-2(%rdx), %r15d
	movl	%edx, 1144(%rsp)
	cmpl	%r15d, 1388(%rsp)
	jl	.L76
	cmpl	$3, %r15d
	jle	.L76
	movq	1368(%rsp), %r15
	vmovupd	-192(%r15,%rcx), %ymm23
	movq	1360(%rsp), %r15
	vmovupd	-192(%r15,%rcx), %ymm21
	movq	1352(%rsp), %r15
	vmovupd	-192(%r15,%rcx), %ymm25
	movq	1376(%rsp), %r15
	vmovupd	-192(%r15,%rcx), %ymm0
	movq	1344(%rsp), %r15
	vmovupd	-192(%r15,%rcx), %ymm3
.L77:
	movl	%edx, %r15d
	decl	%r15d
	cmpl	%r15d, 1388(%rsp)
	jl	.L78
	cmpl	$3, %r15d
	jle	.L78
	movq	1368(%rsp), %r15
	vmovupd	-160(%r15,%rcx), %ymm15
	movq	1360(%rsp), %r15
	vmovupd	-160(%r15,%rcx), %ymm28
	movq	1352(%rsp), %r15
	vmovupd	-160(%r15,%rcx), %ymm11
	movq	1376(%rsp), %r15
	vmovupd	-160(%r15,%rcx), %ymm4
	movq	1344(%rsp), %r15
	vmovupd	-160(%r15,%rcx), %ymm13
.L79:
	movq	1248(%rsp), %r15
	vmovsd	(%rax,%rsi), %xmm1
	vmovsd	(%rax,%rdi), %xmm7
	vmovsd	(%rax,%r9), %xmm6
	vmovhpd	(%r15,%rdx,8), %xmm1, %xmm1
	movq	1240(%rsp), %r15
	vmovsd	(%rax,%r12), %xmm5
	vmovhpd	(%r14,%r8), %xmm7, %xmm7
	vmovhpd	(%r14,%r10), %xmm6, %xmm6
	vmovsd	(%rax,%r10), %xmm8
	vinsertf128	$0x1, %xmm1, %ymm7, %ymm7
	vmovsd	(%rax,%r11), %xmm1
	vmovhpd	(%r14,%r11), %xmm8, %xmm8
	vmovhpd	(%r15,%rdx,8), %xmm1, %xmm1
	movq	1336(%rsp), %r15
	vinsertf128	$0x1, %xmm1, %ymm6, %ymm6
	vmovsd	(%rax,%rbx), %xmm1
	vmovhpd	(%r14,%r15), %xmm1, %xmm1
	movq	1328(%rsp), %r15
	vmovhpd	(%r14,%r15), %xmm5, %xmm5
	movq	1152(%rsp), %r15
	vinsertf128	$0x1, %xmm1, %ymm5, %ymm5
	vmovsd	(%rax,%r8), %xmm1
	vmovsd	(%r15,%rdx,8), %xmm2
	movq	1160(%rsp), %r15
	vmovhpd	(%r14,%rsi), %xmm1, %xmm1
	vmovhpd	(%r14,%rdi), %xmm2, %xmm2
	vinsertf128	$0x1, %xmm1, %ymm2, %ymm2
	vmovsd	(%r15,%rdx,8), %xmm1
	movl	%edx, %r15d
	incl	%r15d
	vmovhpd	(%r14,%r9), %xmm1, %xmm1
	vinsertf128	$0x1, %xmm8, %ymm1, %ymm1
	cmpl	%r15d, 1388(%rsp)
	jge	.L279
	movq	1248(%rsp), %r15
	vmovsd	8(%rax,%rsi), %xmm8
	vmovsd	8(%rax,%rdi), %xmm14
	vmovsd	8(%rax,%r9), %xmm9
	vmovhpd	8(%r15,%rdx,8), %xmm8, %xmm8
	movq	1240(%rsp), %r15
	vmovsd	8(%rax,%r12), %xmm10
	vmovhpd	8(%r14,%r8), %xmm14, %xmm14
	vmovhpd	8(%r14,%r10), %xmm9, %xmm9
	vinsertf128	$0x1, %xmm8, %ymm14, %ymm14
	vmovsd	8(%rax,%r11), %xmm8
	vmovhpd	8(%r15,%rdx,8), %xmm8, %xmm8
	movq	1336(%rsp), %r15
	vinsertf64x2	$0x1, %xmm8, %ymm9, %ymm27
	vmovsd	8(%rax,%rbx), %xmm8
	vmovhpd	8(%r14,%r15), %xmm8, %xmm8
	movq	1328(%rsp), %r15
	vmovhpd	8(%r14,%r15), %xmm10, %xmm10
	movq	1168(%rsp), %r15
	vinsertf128	$0x1, %xmm8, %ymm10, %ymm10
	vmovsd	8(%rax,%r8), %xmm8
	vmovsd	8(%r15,%rdx,8), %xmm9
	movq	1280(%rsp), %r15
	vmovhpd	8(%r14,%rsi), %xmm8, %xmm8
	vmovhpd	8(%r14,%rdi), %xmm9, %xmm9
	vmovsd	8(%r15,%rdx,8), %xmm12
	vinsertf64x2	$0x1, %xmm8, %ymm9, %ymm26
	vmovsd	8(%rax,%r10), %xmm8
	vmovhpd	8(%r14,%r9), %xmm12, %xmm12
	vmovhpd	8(%r14,%r11), %xmm8, %xmm8
	vinsertf128	$0x1, %xmm8, %ymm12, %ymm12
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L78:
	movq	1248(%rsp), %r15
	vmovsd	-8(%rax,%rsi), %xmm1
	movq	1320(%rsp), %r13
	vmovsd	-8(%rax,%r12), %xmm11
	vmovhpd	-8(%r15,%rdx,8), %xmm1, %xmm1
	movq	1288(%rsp), %r15
	vmovsd	-8(%rax,%r15), %xmm15
	vmovhpd	-8(%r14,%r13), %xmm15, %xmm15
	movq	1296(%rsp), %r13
	vinsertf128	$0x1, %xmm1, %ymm15, %ymm15
	vmovsd	-8(%rax,%r13), %xmm1
	movq	1240(%rsp), %r13
	vmovhpd	-8(%r13,%rdx,8), %xmm1, %xmm1
	movq	1312(%rsp), %r13
	vmovsd	-8(%rax,%r13), %xmm2
	movq	1304(%rsp), %r13
	vmovhpd	-8(%r14,%r13), %xmm2, %xmm2
	movq	1336(%rsp), %r13
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm28
	vmovsd	-8(%rax,%rbx), %xmm1
	vmovhpd	-8(%r14,%r13), %xmm1, %xmm1
	movq	1328(%rsp), %r13
	vmovhpd	-8(%r14,%r13), %xmm11, %xmm11
	movq	1320(%rsp), %r13
	vinsertf128	$0x1, %xmm1, %ymm11, %ymm11
	vmovsd	-8(%rax,%r13), %xmm1
	movq	1168(%rsp), %r13
	vmovsd	-8(%r13,%rdx,8), %xmm4
	vmovhpd	-8(%r14,%rsi), %xmm1, %xmm1
	movq	1296(%rsp), %r13
	vmovhpd	-8(%r14,%r15), %xmm4, %xmm4
	movq	1304(%rsp), %r15
	vinsertf128	$0x1, %xmm1, %ymm4, %ymm4
	vmovsd	-8(%rax,%r15), %xmm1
	movq	1280(%rsp), %r15
	vmovhpd	-8(%r14,%r13), %xmm1, %xmm1
	vmovsd	-8(%r15,%rdx,8), %xmm13
	movq	1312(%rsp), %r13
	vmovhpd	-8(%r14,%r13), %xmm13, %xmm13
	vinsertf128	$0x1, %xmm1, %ymm13, %ymm13
	jmp	.L79
	.p2align 4,,10
	.p2align 3
.L76:
	movq	1248(%rsp), %r15
	vmovsd	-16(%rax,%rsi), %xmm0
	movq	1320(%rsp), %r13
	vmovhpd	-16(%r15,%rdx,8), %xmm0, %xmm0
	movq	1288(%rsp), %r15
	vmovsd	-16(%rax,%r15), %xmm1
	vmovhpd	-16(%r14,%r13), %xmm1, %xmm1
	movq	1296(%rsp), %r13
	vinsertf64x2	$0x1, %xmm0, %ymm1, %ymm23
	vmovsd	-16(%rax,%r13), %xmm0
	movq	1240(%rsp), %r13
	vmovhpd	-16(%r13,%rdx,8), %xmm0, %xmm0
	movq	1312(%rsp), %r13
	vmovsd	-16(%rax,%r13), %xmm1
	movq	1304(%rsp), %r13
	vmovhpd	-16(%r14,%r13), %xmm1, %xmm1
	movq	1336(%rsp), %r13
	vinsertf64x2	$0x1, %xmm0, %ymm1, %ymm21
	vmovsd	-16(%rax,%rbx), %xmm0
	vmovsd	-16(%rax,%r12), %xmm1
	vmovhpd	-16(%r14,%r13), %xmm0, %xmm0
	movq	1328(%rsp), %r13
	vmovhpd	-16(%r14,%r13), %xmm1, %xmm1
	movq	1320(%rsp), %r13
	vinsertf64x2	$0x1, %xmm0, %ymm1, %ymm25
	vmovsd	-16(%rax,%r13), %xmm1
	movq	1168(%rsp), %r13
	vmovsd	-16(%r13,%rdx,8), %xmm0
	vmovhpd	-16(%r14,%rsi), %xmm1, %xmm1
	movq	1296(%rsp), %r13
	vmovhpd	-16(%r14,%r15), %xmm0, %xmm0
	movq	1304(%rsp), %r15
	vinsertf128	$0x1, %xmm1, %ymm0, %ymm0
	vmovsd	-16(%rax,%r15), %xmm1
	movq	1280(%rsp), %r15
	vmovhpd	-16(%r14,%r13), %xmm1, %xmm1
	vmovsd	-16(%r15,%rdx,8), %xmm3
	movq	1312(%rsp), %r13
	vmovhpd	-16(%r14,%r13), %xmm3, %xmm3
	vinsertf128	$0x1, %xmm1, %ymm3, %ymm3
	jmp	.L77
	.p2align 4,,10
	.p2align 3
.L72:
	vmovsd	8(%r12), %xmm31
	vmovsd	8(%rbx), %xmm15
	vmovsd	8(%r13), %xmm22
	movq	1328(%rsp), %rax
	vmovhpd	8(%r15), %xmm15, %xmm15
	vmovsd	8(%r11), %xmm16
	vmovsd	8(%r9), %xmm24
	vunpcklpd	%xmm31, %xmm22, %xmm22
	vmovsd	8(%rdi), %xmm21
	vinsertf64x2	$0x1, %xmm15, %ymm22, %ymm22
	vmovsd	8(%r10), %xmm15
	vmovhpd	8(%rax), %xmm15, %xmm15
	movq	1320(%rsp), %rax
	vmovsd	8(%rax), %xmm30
	movq	1336(%rsp), %rax
	vunpcklpd	%xmm30, %xmm16, %xmm16
	vmovsd	8(%rax), %xmm31
	movq	1208(%rsp), %rax
	vinsertf64x2	$0x1, %xmm15, %ymm16, %ymm16
	vmovsd	8(%r8), %xmm15
	vunpcklpd	%xmm31, %xmm24, %xmm24
	vmovhpd	8(%rsi), %xmm15, %xmm15
	vinsertf64x2	$0x1, %xmm15, %ymm24, %ymm24
	vmovsd	8(%rax,%rdx,8), %xmm15
	movq	1200(%rsp), %rax
	vmovhpd	8(%rax,%rdx,8), %xmm15, %xmm15
	movq	1216(%rsp), %rax
	vmovsd	8(%rax,%rdx,8), %xmm30
	movq	1184(%rsp), %rax
	vunpcklpd	%xmm30, %xmm21, %xmm21
	vmovsd	8(%rax,%rdx,8), %xmm30
	movq	1176(%rsp), %rax
	vinsertf64x2	$0x1, %xmm15, %ymm21, %ymm15
	vmovsd	8(%rax,%rdx,8), %xmm31
	movq	1312(%rsp), %rax
	vmovsd	8(%rax,%rdx,8), %xmm21
	movq	1192(%rsp), %rax
	vunpcklpd	%xmm31, %xmm30, %xmm30
	vmovsd	8(%rax,%rdx,8), %xmm31
	vunpcklpd	%xmm31, %xmm21, %xmm21
	vinsertf64x2	$0x1, %xmm30, %ymm21, %ymm21
	jmp	.L73
	.p2align 4,,10
	.p2align 3
.L70:
	vmovsd	(%rbx), %xmm6
	vmovsd	0(%r13), %xmm13
	movq	1328(%rsp), %r14
	vmovsd	(%r11), %xmm10
	vmovhpd	(%r15), %xmm6, %xmm6
	vmovhpd	(%r12), %xmm13, %xmm13
	vmovsd	(%rdi), %xmm15
	vinsertf128	$0x1, %xmm6, %ymm13, %ymm13
	vmovsd	(%r10), %xmm6
	vmovhpd	(%r14), %xmm6, %xmm6
	movq	1320(%rsp), %r14
	vmovhpd	(%r14), %xmm10, %xmm10
	movq	1336(%rsp), %r14
	vinsertf64x2	$0x1, %xmm6, %ymm10, %ymm27
	vmovsd	(%r9), %xmm6
	vmovsd	(%r8), %xmm10
	vmovhpd	(%r14), %xmm6, %xmm6
	movq	1208(%rsp), %r14
	vmovhpd	(%rsi), %xmm10, %xmm10
	vinsertf128	$0x1, %xmm10, %ymm6, %ymm6
	vmovsd	(%r14,%rdx,8), %xmm10
	movq	1200(%rsp), %r14
	vmovhpd	(%r14,%rdx,8), %xmm10, %xmm10
	movq	1216(%rsp), %r14
	vmovhpd	(%r14,%rdx,8), %xmm15, %xmm15
	movq	1184(%rsp), %r14
	vinsertf64x2	$0x1, %xmm10, %ymm15, %ymm26
	vmovsd	(%r14,%rdx,8), %xmm15
	movq	1176(%rsp), %r14
	vmovhpd	(%r14,%rdx,8), %xmm15, %xmm15
	movq	1312(%rsp), %r14
	vmovsd	(%r14,%rdx,8), %xmm10
	movq	1192(%rsp), %r14
	vmovhpd	(%r14,%rdx,8), %xmm10, %xmm10
	vinsertf128	$0x1, %xmm15, %ymm10, %ymm10
	jmp	.L71
	.p2align 4,,10
	.p2align 3
.L68:
	vmovsd	-16(%rbx), %xmm1
	vmovsd	-16(%r13), %xmm14
	movq	1328(%rsp), %r14
	vmovsd	-16(%r11), %xmm4
	vmovhpd	-16(%r15), %xmm1, %xmm1
	vmovhpd	-16(%r12), %xmm14, %xmm14
	vmovsd	-16(%r9), %xmm7
	vmovsd	-16(%rdi), %xmm2
	vinsertf128	$0x1, %xmm1, %ymm14, %ymm14
	vmovsd	-16(%r10), %xmm1
	vmovhpd	-16(%r14), %xmm1, %xmm1
	movq	1320(%rsp), %r14
	vmovhpd	-16(%r14), %xmm4, %xmm4
	movq	1336(%rsp), %r14
	vinsertf128	$0x1, %xmm1, %ymm4, %ymm4
	vmovsd	-16(%r8), %xmm1
	vmovhpd	-16(%r14), %xmm7, %xmm7
	movq	1208(%rsp), %r14
	vmovhpd	-16(%rsi), %xmm1, %xmm1
	vinsertf128	$0x1, %xmm1, %ymm7, %ymm7
	vmovsd	-16(%r14,%rdx,8), %xmm1
	movq	1200(%rsp), %r14
	vmovhpd	-16(%r14,%rdx,8), %xmm1, %xmm1
	movq	1216(%rsp), %r14
	vmovhpd	-16(%r14,%rdx,8), %xmm2, %xmm2
	movq	1184(%rsp), %r14
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm29
	vmovsd	-16(%r14,%rdx,8), %xmm1
	movq	1176(%rsp), %r14
	vmovhpd	-16(%r14,%rdx,8), %xmm1, %xmm1
	movq	1312(%rsp), %r14
	vmovsd	-16(%r14,%rdx,8), %xmm11
	movq	1192(%rsp), %r14
	vmovhpd	-16(%r14,%rdx,8), %xmm11, %xmm11
	vinsertf128	$0x1, %xmm1, %ymm11, %ymm11
	jmp	.L69
	.p2align 4,,10
	.p2align 3
.L66:
	vmovsd	-24(%rbx), %xmm0
	vmovsd	-24(%r13), %xmm3
	movq	1328(%rsp), %r14
	vmovsd	-24(%r10), %xmm1
	vmovhpd	-24(%r15), %xmm0, %xmm0
	vmovhpd	-24(%r12), %xmm3, %xmm3
	vmovsd	-24(%r9), %xmm2
	vmovhpd	-24(%r14), %xmm1, %xmm1
	vinsertf128	$0x1, %xmm0, %ymm3, %ymm3
	movq	1320(%rsp), %r14
	vmovsd	-24(%r11), %xmm0
	vmovhpd	-24(%r14), %xmm0, %xmm0
	movq	1336(%rsp), %r14
	vinsertf128	$0x1, %xmm1, %ymm0, %ymm0
	vmovsd	-24(%r8), %xmm1
	vmovhpd	-24(%r14), %xmm2, %xmm2
	movq	1208(%rsp), %r14
	vmovhpd	-24(%rsi), %xmm1, %xmm1
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm25
	vmovsd	-24(%r14,%rdx,8), %xmm1
	movq	1200(%rsp), %r14
	vmovsd	-24(%rdi), %xmm2
	vmovhpd	-24(%r14,%rdx,8), %xmm1, %xmm1
	movq	1216(%rsp), %r14
	vmovhpd	-24(%r14,%rdx,8), %xmm2, %xmm2
	movq	1184(%rsp), %r14
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm20
	vmovsd	-24(%r14,%rdx,8), %xmm1
	movq	1176(%rsp), %r14
	vmovhpd	-24(%r14,%rdx,8), %xmm1, %xmm1
	movq	1312(%rsp), %r14
	vmovsd	-24(%r14,%rdx,8), %xmm2
	movq	1192(%rsp), %r14
	vmovhpd	-24(%r14,%rdx,8), %xmm2, %xmm2
	vinsertf64x2	$0x1, %xmm1, %ymm2, %ymm23
	jmp	.L67
	.p2align 4,,10
	.p2align 3
.L82:
	movq	1248(%rsp), %r15
	vmovsd	16(%rax,%rsi), %xmm8
	vmovsd	16(%rax,%rdi), %xmm9
	vmovhpd	16(%r15,%rdx,8), %xmm8, %xmm8
	movq	1240(%rsp), %r15
	vmovhpd	16(%r14,%r8), %xmm9, %xmm9
	vinsertf64x2	$0x1, %xmm8, %ymm9, %ymm22
	vmovsd	16(%rax,%r11), %xmm8
	vmovsd	16(%rax,%r9), %xmm9
	vmovhpd	16(%r15,%rdx,8), %xmm8, %xmm8
	vmovhpd	16(%r14,%r10), %xmm9, %xmm9
	movq	1336(%rsp), %r15
	vinsertf64x2	$0x1, %xmm8, %ymm9, %ymm20
	vmovsd	16(%rax,%rbx), %xmm8
	vmovsd	16(%rax,%r12), %xmm9
	vmovhpd	16(%r14,%r15), %xmm8, %xmm8
	movq	1328(%rsp), %r15
	vmovhpd	16(%r14,%r15), %xmm9, %xmm9
	movq	1168(%rsp), %r15
	vinsertf64x2	$0x1, %xmm8, %ymm9, %ymm24
	vmovsd	16(%rax,%r8), %xmm8
	vmovsd	16(%r15,%rdx,8), %xmm9
	movq	1280(%rsp), %r15
	vmovhpd	16(%r14,%rsi), %xmm8, %xmm8
	vmovhpd	16(%r14,%rdi), %xmm9, %xmm9
	vinsertf64x2	$0x1, %xmm8, %ymm9, %ymm16
	vmovsd	16(%rax,%r10), %xmm9
	vmovsd	16(%r15,%rdx,8), %xmm8
	vmovhpd	16(%r14,%r11), %xmm9, %xmm9
	vmovhpd	16(%r14,%r9), %xmm8, %xmm8
	vinsertf128	$0x1, %xmm9, %ymm8, %ymm8
	jmp	.L83
	.p2align 4,,10
	.p2align 3
.L41:
	movq	1272(%rsp), %rdi
	movq	1232(%rsp), %rax
	incl	1008(%rsp)
	addq	%rdi, 768(%rsp)
	addq	%rdi, 760(%rsp)
	addq	%rdi, 800(%rsp)
	addq	%rdi, 752(%rsp)
	addq	%rdi, 744(%rsp)
	addq	%rdi, 736(%rsp)
	addq	%rdi, 728(%rsp)
	addq	%rdi, 720(%rsp)
	addq	%rax, 1248(%rsp)
	addq	%rax, 1240(%rsp)
	incl	784(%rsp)
	addq	%rax, 1256(%rsp)
	addq	%rax, 1216(%rsp)
	addq	%rax, 1208(%rsp)
	addq	%rax, 1200(%rsp)
	addq	%rdi, 712(%rsp)
	movl	812(%rsp), %edi
	addq	%rax, 1192(%rsp)
	addq	%rax, 1184(%rsp)
	addq	%rax, 1176(%rsp)
	addq	%rax, 696(%rsp)
	cmpl	%edi, 636(%rsp)
	je	.L280
	movq	1360(%rsp), %rax
	movq	1368(%rsp), %rdx
	movq	%rax, 1368(%rsp)
	movq	1352(%rsp), %rax
	movq	%rax, 1360(%rsp)
	movq	1376(%rsp), %rax
	movq	%rax, 1352(%rsp)
	movq	1344(%rsp), %rax
	movq	%rax, 1376(%rsp)
	movq	856(%rsp), %rax
	movq	%rdx, 856(%rsp)
	movq	%rax, 1344(%rsp)
	movl	%edi, %eax
	jmp	.L63
	.p2align 4,,10
	.p2align 3
.L276:
	movl	708(%rsp), %edi
	cmpl	%edi, 1024(%rsp)
	jl	.L41
	movl	1008(%rsp), %eax
	vmovapd	.LC0(%rip), %ymm6
	vmovapd	%ymm17, 928(%rsp)
	vmovapd	.LC8(%rip), %ymm9
	vmovapd	.LC10(%rip), %ymm8
	vmovapd	%ymm19, 960(%rsp)
	movq	%rax, 792(%rsp)
	movslq	784(%rsp), %rax
	vmovapd	%ymm6, 896(%rsp)
	movq	%rax, 1016(%rsp)
	vmovapd	%ymm18, 640(%rsp)
.L65:
	movl	708(%rsp), %eax
	jmp	.L62
	.p2align 4,,10
	.p2align 3
.L44:
	movq	1288(%rsp), %rax
	vmovsd	72(%rbx), %xmm2
	vmovsd	72(%r12), %xmm7
	vmovsd	72(%r15), %xmm10
	vmovsd	72(%rax), %xmm0
	movq	1080(%rsp), %rax
	vmovhpd	72(%rax), %xmm0, %xmm0
	movq	1104(%rsp), %rax
	vmovhpd	72(%rax), %xmm2, %xmm2
	movq	1144(%rsp), %rax
	vinsertf128	$0x1, %xmm0, %ymm2, %ymm2
	vmovsd	72(%r13), %xmm0
	vmovhpd	72(%rax), %xmm7, %xmm7
	movq	1120(%rsp), %rax
	vmovhpd	72(%rax), %xmm0, %xmm0
	movq	1296(%rsp), %rax
	vinsertf128	$0x1, %xmm7, %ymm0, %ymm0
	vmovsd	72(%rax), %xmm7
	movq	1136(%rsp), %rax
	vmovhpd	72(%rax), %xmm7, %xmm7
	movq	1152(%rsp), %rax
	vmovhpd	72(%rax), %xmm10, %xmm10
	movq	1112(%rsp), %rax
	vinsertf64x2	$0x1, %xmm7, %ymm10, %ymm25
	vmovsd	72(%r10), %xmm7
	vmovsd	72(%r9), %xmm10
	vmovhpd	72(%rax), %xmm7, %xmm7
	movq	1128(%rsp), %rax
	vmovhpd	72(%rax), %xmm10, %xmm10
	movq	1088(%rsp), %rax
	vinsertf128	$0x1, %xmm7, %ymm10, %ymm10
	vmovsd	72(%r8), %xmm7
	vmovhpd	72(%rax), %xmm7, %xmm7
	movq	1096(%rsp), %rax
	vmovapd	%xmm7, %xmm27
	vmovsd	72(%rdi), %xmm7
	vmovhpd	72(%rax), %xmm7, %xmm7
	vinsertf64x2	$0x1, %xmm27, %ymm7, %ymm7
	jmp	.L45
.L42:
	movq	1288(%rsp), %rax
	vmovsd	64(%rbx), %xmm15
	vmovsd	64(%r12), %xmm3
	vmovsd	64(%r15), %xmm4
	vmovsd	64(%rax), %xmm1
	movq	1080(%rsp), %rax
	vmovsd	64(%r9), %xmm13
	vmovhpd	64(%rax), %xmm1, %xmm1
	movq	1104(%rsp), %rax
	vmovhpd	64(%rax), %xmm15, %xmm15
	movq	1144(%rsp), %rax
	vinsertf128	$0x1, %xmm1, %ymm15, %ymm15
	vmovsd	64(%r13), %xmm1
	vmovhpd	64(%rax), %xmm3, %xmm3
	movq	1120(%rsp), %rax
	vmovhpd	64(%rax), %xmm1, %xmm1
	movq	1296(%rsp), %rax
	vinsertf128	$0x1, %xmm3, %ymm1, %ymm1
	vmovsd	64(%rax), %xmm3
	movq	1136(%rsp), %rax
	vmovhpd	64(%rax), %xmm3, %xmm3
	movq	1152(%rsp), %rax
	vmovhpd	64(%rax), %xmm4, %xmm4
	movq	1112(%rsp), %rax
	vinsertf128	$0x1, %xmm3, %ymm4, %ymm4
	vmovsd	64(%r10), %xmm3
	vmovhpd	64(%rax), %xmm3, %xmm3
	movq	1128(%rsp), %rax
	vmovhpd	64(%rax), %xmm13, %xmm13
	movq	1088(%rsp), %rax
	vinsertf64x2	$0x1, %xmm3, %ymm13, %ymm30
	vmovsd	64(%r8), %xmm3
	vmovsd	64(%rdi), %xmm13
	vmovhpd	64(%rax), %xmm3, %xmm3
	movq	1096(%rsp), %rax
	vmovhpd	64(%rax), %xmm13, %xmm13
	vinsertf128	$0x1, %xmm3, %ymm13, %ymm13
	jmp	.L43
.L147:
	movl	$4, 1320(%rsp)
	movl	1384(%rsp), %eax
	xorl	%ecx, %ecx
	jmp	.L22
.L280:
	movq	1360(%rsp), %rax
	cmpl	$3, 1384(%rsp)
	movl	360(%rsp), %r12d
	vmovdqa	288(%rsp), %ymm1
	movq	%rax, 1408(%rsp)
	movq	1352(%rsp), %rax
	movq	328(%rsp), %rbx
	movq	%rax, 1416(%rsp)
	movq	1376(%rsp), %rax
	movq	%rax, 1424(%rsp)
	movq	1344(%rsp), %rax
	movq	%rax, 1432(%rsp)
	movq	856(%rsp), %rax
	movq	%rax, 1440(%rsp)
	movq	1368(%rsp), %rax
	movq	%rax, 1448(%rsp)
	jle	.L87
	movq	80(%rsp), %r8
	movq	88(%rsp), %rdi
	movq	96(%rsp), %rsi
	movq	104(%rsp), %rcx
	movq	368(%rsp), %r9
	movq	1232(%rsp), %r10
	movq	72(%rsp), %r11
	.p2align 4,,10
	.p2align 3
.L86:
	movq	(%r9), %rdx
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L88:
	vmovupd	96(%rdx,%rax,4), %ymm5
	vmovupd	64(%rdx,%rax,4), %ymm4
	vmovupd	32(%rdx,%rax,4), %ymm2
	vmovupd	(%rdx,%rax,4), %ymm0
	vshuff64x2	$0, %ymm4, %ymm0, %ymm3
	vshuff64x2	$3, %ymm4, %ymm0, %ymm0
	vshuff64x2	$0, %ymm5, %ymm2, %ymm4
	vshuff64x2	$3, %ymm5, %ymm2, %ymm2
	vunpckhpd	%ymm2, %ymm0, %ymm5
	vunpcklpd	%ymm2, %ymm0, %ymm0
	vunpckhpd	%ymm4, %ymm3, %ymm2
	vunpcklpd	%ymm4, %ymm3, %ymm3
	vmovupd	%ymm3, 32(%rcx,%rax)
	vmovupd	%ymm2, 32(%rsi,%rax)
	vmovupd	%ymm0, 32(%rdi,%rax)
	vmovupd	%ymm5, 32(%r8,%rax)
	addq	$32, %rax
	cmpq	%rbx, %rax
	jne	.L88
	addq	$8, %r9
	addq	%r10, %rcx
	addq	%r10, %rsi
	addq	%r10, %rdi
	addq	%r10, %r8
	cmpq	%r11, %r9
	jne	.L86
.L87:
	movq	248(%rsp), %r8
	movq	280(%rsp), %r14
	movq	%rbx, 752(%rsp)
	movl	%r12d, %ecx
	movq	272(%rsp), %r10
	movq	256(%rsp), %r11
	movl	%r12d, 760(%rsp)
	movq	%r8, %rdx
	movq	320(%rsp), %r13
	movq	264(%rsp), %rsi
	movq	%r14, 864(%rsp)
	movl	612(%rsp), %edi
	vmovsd	.LC1(%rip), %xmm3
	vmovsd	.LC3(%rip), %xmm2
	vmovsd	.LC5(%rip), %xmm11
	vmovsd	.LC7(%rip), %xmm10
	vmovsd	.LC9(%rip), %xmm9
	vmovsd	.LC11(%rip), %xmm8
	movq	1272(%rsp), %r12
	movq	816(%rsp), %r14
	movq	688(%rsp), %r8
	movq	432(%rsp), %r9
.L101:
	movl	608(%rsp), %eax
	movl	%edi, 1376(%rsp)
	cmpl	%eax, %edi
	jg	.L281
	movl	%ecx, %eax
	incl	%ecx
	andl	$1, %eax
	negq	%rax
	andq	672(%rsp), %rax
	movq	%rax, 960(%rsp)
	movq	%rax, %rbx
	leaq	(%r14,%rax,8), %r15
	movl	%ecx, %eax
	andl	$1, %eax
	negq	%rax
	andq	672(%rsp), %rax
	movq	%rax, 1080(%rsp)
	leaq	(%r14,%rax,8), %rax
	movq	%rax, 1160(%rsp)
	movl	1384(%rsp), %eax
	testl	%eax, %eax
	jle	.L89
	movq	424(%rsp), %rax
	movq	%r13, 848(%rsp)
	movq	%rdx, 1136(%rsp)
	leaq	24(%rax,%rsi), %rax
	movq	%r10, 1144(%rsp)
	addq	%r15, %rax
	movq	%r11, 1112(%rsp)
	movq	%rax, 1104(%rsp)
	leaq	(%rbx,%r11), %rax
	salq	$3, %rax
	movq	$0, 1328(%rsp)
	movq	%rax, 1168(%rsp)
	leaq	(%r12,%rdx), %rax
	movq	%rax, 1120(%rsp)
	addq	%rbx, %rax
	salq	$3, %rax
	movq	%rsi, 856(%rsp)
	movq	%rax, 1176(%rsp)
	leaq	(%rbx,%rdx), %rax
	salq	$3, %rax
	movq	%r11, 840(%rsp)
	movq	%rax, 1352(%rsp)
	leaq	(%r12,%r10), %rax
	movq	%rax, 1128(%rsp)
	addq	%rbx, %rax
	salq	$3, %rax
	movq	%r10, 832(%rsp)
	movq	%rax, 1360(%rsp)
	leaq	(%rbx,%r10), %rax
	leaq	0(,%rax,8), %rbx
	movq	1080(%rsp), %rax
	movq	%rbx, 1152(%rsp)
	movl	612(%rsp), %ebx
	addq	%r11, %rax
	addl	$2, %ebx
	movl	%ebx, 1088(%rsp)
	leaq	0(,%rax,8), %rbx
	movq	%rbx, 1096(%rsp)
	addq	%r14, %rbx
	movq	%rbx, 872(%rsp)
	movq	1232(%rsp), %rbx
	leaq	(%rbx,%r13), %rax
	addq	%r15, %rax
	movq	%rax, 928(%rsp)
	movq	864(%rsp), %rax
	addq	%rbx, %rax
	leaq	(%rax,%r15), %rbx
	movq	1160(%rsp), %rax
	movq	%rbx, 896(%rsp)
	leaq	16(%r15,%rsi), %rbx
	movq	%rbx, 1000(%rsp)
	movq	864(%rsp), %rbx
	leaq	32(%rax,%rsi), %rax
	movq	%rax, 880(%rsp)
	leaq	16(%r15,%rbx), %rbx
	movq	%rbx, 992(%rsp)
	leaq	16(%r15,%r13), %rbx
	movq	%r12, %r13
	movq	%rbx, 888(%rsp)
	movq	%rdx, 824(%rsp)
	movl	%edi, 812(%rsp)
	movq	%r8, 800(%rsp)
	movq	%r9, 792(%rsp)
	movl	%ecx, 768(%rsp)
	.p2align 4,,10
	.p2align 3
.L90:
	movl	1376(%rsp), %eax
	movl	%eax, %r11d
	incl	%eax
	cmpl	$1, 1384(%rsp)
	movl	%eax, 1376(%rsp)
	leal	-1(%r11), %eax
	movl	%eax, 1368(%rsp)
	leal	-2(%r11), %eax
	movl	%eax, 1344(%rsp)
	leal	2(%r11), %eax
	movl	%eax, 1336(%rsp)
	je	.L98
	movq	1096(%rsp), %rdi
	movq	1328(%rsp), %rax
	leaq	32(%rax,%rdi), %rax
	movq	1168(%rsp), %rdi
	movq	%rax, %rdx
	leaq	24(%rdi), %rcx
	movq	1176(%rsp), %rdi
	subq	%rcx, %rdx
	movq	%rax, %rcx
	cmpq	$48, %rdx
	leaq	24(%rdi), %rsi
	movq	1352(%rsp), %rdi
	seta	%dl
	subq	%rsi, %rcx
	cmpq	$48, %rcx
	leaq	24(%rdi), %rsi
	movq	1360(%rsp), %rdi
	seta	%cl
	andl	%edx, %ecx
	movq	%rax, %rdx
	subq	%rsi, %rdx
	leaq	24(%rdi), %rsi
	cmpq	$48, %rdx
	seta	%dl
	andl	%ecx, %edx
	movq	%rax, %rcx
	subq	%rsi, %rcx
	cmpq	$48, %rcx
	seta	%cl
	testb	%cl, %dl
	je	.L98
	movq	1152(%rsp), %rdi
	leaq	24(%rdi), %rdx
	subq	%rdx, %rax
	cmpq	$48, %rax
	jbe	.L98
	cmpl	$2, 808(%rsp)
	jbe	.L152
	movq	1168(%rsp), %rax
	movq	%rdi, %r10
	leaq	32(%r14), %rdi
	movq	1176(%rsp), %r9
	leaq	40(%r14), %rdx
	movq	1352(%rsp), %rbx
	leaq	16(%r14), %r8
	vbroadcastsd	.LC1(%rip), %ymm14
	leaq	(%rdi,%rax), %rcx
	leaq	(%rdi,%r9), %rsi
	vbroadcastsd	.LC3(%rip), %ymm13
	vbroadcastsd	.LC5(%rip), %ymm12
	movq	%rcx, 1320(%rsp)
	leaq	(%rdi,%r10), %rcx
	leaq	(%r8,%rax), %r12
	movq	%rcx, 1184(%rsp)
	leaq	24(%r14), %rcx
	movq	%rsi, 1312(%rsp)
	leaq	(%rcx,%rax), %rsi
	movq	%rsi, 1304(%rsp)
	leaq	(%rdx,%rax), %rsi
	movq	%rsi, 1296(%rsp)
	leaq	(%rdi,%rbx), %rsi
	movq	%rax, %rbx
	leaq	48(%r14), %rax
	movq	%r12, 1280(%rsp)
	leaq	(%rax,%rbx), %r12
	leaq	(%rdx,%r9), %rbx
	movq	%rbx, 1240(%rsp)
	leaq	(%rdx,%r10), %rbx
	movq	%rbx, 1216(%rsp)
	movq	1352(%rsp), %rbx
	movq	%rsi, 1288(%rsp)
	movq	1360(%rsp), %rsi
	addq	%r8, %rbx
	movq	%r12, 1256(%rsp)
	leaq	(%rcx,%r9), %r12
	movq	%rbx, 1208(%rsp)
	addq	%rsi, %rdi
	leaq	(%r8,%rsi), %rbx
	movq	1352(%rsp), %rsi
	movq	%r12, 1248(%rsp)
	leaq	(%rcx,%r10), %r12
	addq	%rax, %rsi
	movq	%rsi, 1200(%rsp)
	movq	1360(%rsp), %rsi
	addq	%rax, %rsi
	movq	%rsi, 1192(%rsp)
	leaq	(%r8,%r9), %rsi
	addq	%rax, %r9
	movq	%rsi, 1056(%rsp)
	leaq	(%r8,%r10), %rsi
	addq	%rax, %r10
	movq	1352(%rsp), %rax
	movq	%r9, 1072(%rsp)
	movq	872(%rsp), %r8
	movq	%r10, 1048(%rsp)
	leaq	(%rdx,%rax), %r9
	leaq	(%rcx,%rax), %r10
	movq	1360(%rsp), %rax
	addq	%rax, %rdx
	addq	%rax, %rcx
	movq	1328(%rsp), %rax
	leaq	32(%rax,%r8), %rax
	movq	%rax, 1040(%rsp)
	vbroadcastsd	.LC7(%rip), %ymm7
	xorl	%eax, %eax
	movq	1184(%rsp), %r8
	vbroadcastsd	.LC9(%rip), %ymm6
	movq	%rdx, 1184(%rsp)
	movq	%r13, 1032(%rsp)
	vbroadcastsd	.LC11(%rip), %ymm5
	movq	%r15, 1016(%rsp)
	movq	1072(%rsp), %r13
	movl	%r11d, 1008(%rsp)
	movq	1040(%rsp), %r15
	movq	1056(%rsp), %r11
	movq	%r14, 1056(%rsp)
	movq	1048(%rsp), %r14
	.p2align 4,,10
	.p2align 3
.L92:
	movq	1312(%rsp), %rdx
	vmovupd	(%rdx,%rax), %ymm4
	movq	1304(%rsp), %rdx
	vaddpd	(%r8,%rax), %ymm4, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1296(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1320(%rsp), %rdx
	vmulpd	%ymm14, %ymm0, %ymm0
	vfmadd231pd	(%rdx,%rax), %ymm13, %ymm0
	movq	1288(%rsp), %rdx
	vmovupd	(%rdx,%rax), %ymm4
	movq	1280(%rsp), %rdx
	vaddpd	(%rdi,%rax), %ymm4, %ymm4
	vaddpd	(%rdx,%rax), %ymm4, %ymm4
	movq	1256(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm4, %ymm4
	movq	1248(%rsp), %rdx
	vfmadd132pd	%ymm12, %ymm0, %ymm4
	vmovupd	(%rdx,%rax), %ymm0
	movq	1240(%rsp), %rdx
	vaddpd	(%r12,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1216(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1208(%rsp), %rdx
	vfmadd132pd	%ymm7, %ymm4, %ymm0
	vmovupd	(%rdx,%rax), %ymm4
	movq	1200(%rsp), %rdx
	vaddpd	(%rbx,%rax), %ymm4, %ymm4
	vaddpd	(%rdx,%rax), %ymm4, %ymm4
	movq	1192(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm4, %ymm4
	vfmadd132pd	%ymm6, %ymm0, %ymm4
	vmovupd	(%r11,%rax), %ymm0
	vaddpd	(%rsi,%rax), %ymm0, %ymm0
	vaddpd	0(%r13,%rax), %ymm0, %ymm0
	movq	1184(%rsp), %rdx
	vaddpd	(%r14,%rax), %ymm0, %ymm0
	vaddpd	(%r10,%rax), %ymm0, %ymm0
	vaddpd	(%r9,%rax), %ymm0, %ymm0
	vaddpd	(%rcx,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1264(%rsp), %rdx
	vfmadd132pd	%ymm5, %ymm4, %ymm0
	vmovupd	%ymm0, (%r15,%rax)
	addq	$32, %rax
	cmpq	%rdx, %rax
	jne	.L92
	movl	788(%rsp), %ecx
	movl	1384(%rsp), %eax
	movq	1032(%rsp), %r13
	movq	1016(%rsp), %r15
	movl	1008(%rsp), %r11d
	movq	1056(%rsp), %r14
	cmpl	%eax, %ecx
	je	.L97
	subl	%ecx, %eax
	cmpl	$1, %eax
	je	.L153
	movl	704(%rsp), %esi
	movl	%esi, 1320(%rsp)
.L91:
	movq	1112(%rsp), %rsi
	movq	960(%rsp), %r12
	movq	1128(%rsp), %rbx
	leaq	(%rsi,%rcx), %r10
	movq	1120(%rsp), %rsi
	leaq	4(%r12,%r10), %rdi
	leaq	(%r12,%rsi), %rdx
	vmovupd	(%r14,%rdi,8), %xmm6
	leaq	4(%rcx,%rdx), %rsi
	movq	1144(%rsp), %rdx
	vmovupd	-16(%r14,%rsi,8), %xmm7
	addq	%r12, %rdx
	leaq	4(%rcx,%rdx), %r9
	movq	1136(%rsp), %rdx
	vmovupd	(%r14,%r9,8), %xmm0
	leaq	0(,%r9,8), %r8
	addq	%r12, %rdx
	addq	%rbx, %r12
	movq	1080(%rsp), %rbx
	vaddpd	(%r14,%rsi,8), %xmm0, %xmm0
	leaq	4(%rcx,%rdx), %rdx
	salq	$3, %rdx
	leaq	4(%rcx,%r12), %rcx
	leaq	4(%rbx,%r10), %r10
	salq	$3, %rcx
	vaddpd	-8(%r14,%rdi,8), %xmm0, %xmm0
	vaddpd	8(%r14,%rdi,8), %xmm0, %xmm0
	vmulpd	.LC1(%rip){1to2}, %xmm0, %xmm0
	vfmadd231pd	.LC3(%rip){1to2}, %xmm6, %xmm0
	vmovupd	(%r14,%rdx), %xmm6
	vaddpd	(%r14,%rcx), %xmm6, %xmm4
	vmovupd	-8(%r14,%rsi,8), %xmm6
	vaddpd	-16(%r14,%rdi,8), %xmm4, %xmm4
	vaddpd	16(%r14,%rdi,8), %xmm4, %xmm4
	vfmadd132pd	.LC5(%rip){1to2}, %xmm0, %xmm4
	vaddpd	-8(%r14,%r8), %xmm6, %xmm0
	vmovupd	-16(%r14,%rdx), %xmm6
	vaddpd	8(%r14,%rsi,8), %xmm0, %xmm0
	vaddpd	8(%r14,%r8), %xmm0, %xmm0
	vfmadd132pd	.LC7(%rip){1to2}, %xmm4, %xmm0
	vaddpd	-16(%r14,%rcx), %xmm6, %xmm4
	vaddpd	16(%r14,%rdx), %xmm4, %xmm4
	vaddpd	16(%r14,%rcx), %xmm4, %xmm4
	vfmadd132pd	.LC9(%rip){1to2}, %xmm0, %xmm4
	vaddpd	-16(%r14,%r8), %xmm7, %xmm0
	vaddpd	16(%r14,%rsi,8), %xmm0, %xmm0
	vaddpd	16(%r14,%r8), %xmm0, %xmm0
	vaddpd	-8(%r14,%rdx), %xmm0, %xmm0
	vaddpd	8(%r14,%rdx), %xmm0, %xmm0
	vaddpd	-8(%r14,%rcx), %xmm0, %xmm0
	vaddpd	8(%r14,%rcx), %xmm0, %xmm0
	vfmadd132pd	.LC11(%rip){1to2}, %xmm4, %xmm0
	vmovupd	%xmm0, (%r14,%r10,8)
	testb	$1, %al
	je	.L97
	movl	1320(%rsp), %edi
	andl	$-2, %eax
	addl	%edi, %eax
.L94:
	movslq	1368(%rsp), %rdi
	movslq	%r11d, %r9
	movslq	1376(%rsp), %r11
	movslq	%eax, %rbx
	imulq	%r13, %r9
	leal	-1(%rax), %ecx
	movslq	1336(%rsp), %r10
	imulq	%r13, %rdi
	movslq	%ecx, %rcx
	leal	1(%rax), %edx
	imulq	%r13, %r11
	movslq	%edx, %rdx
	imulq	%r13, %r10
	leaq	(%rbx,%r9), %r12
	leaq	(%rbx,%rdi), %r8
	vmovsd	(%r15,%r8,8), %xmm0
	leaq	(%rbx,%r11), %rsi
	leal	-2(%rax), %r8d
	addl	$2, %eax
	movl	%eax, 1368(%rsp)
	movslq	%r8d, %r8
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	leaq	(%rcx,%r9), %rsi
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	leaq	(%rdx,%r9), %rsi
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	movslq	1344(%rsp), %rsi
	imulq	%r13, %rsi
	vmulsd	%xmm3, %xmm0, %xmm0
	leaq	(%rbx,%rsi), %rax
	addq	%r10, %rbx
	vmovsd	(%r15,%rax,8), %xmm4
	movslq	1368(%rsp), %rax
	vaddsd	(%r15,%rbx,8), %xmm4, %xmm4
	leaq	(%r8,%r9), %rbx
	vfmadd231sd	(%r15,%r12,8), %xmm2, %xmm0
	addq	%rax, %r9
	vaddsd	(%r15,%rbx,8), %xmm4, %xmm4
	leaq	(%rcx,%rdi), %rbx
	vaddsd	(%r15,%r9,8), %xmm4, %xmm4
	leaq	(%rcx,%r11), %r9
	vfmadd132sd	%xmm11, %xmm0, %xmm4
	vmovsd	(%r15,%rbx,8), %xmm0
	leaq	(%r8,%rsi), %rbx
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%rdi), %r9
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%r11), %r9
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%r8,%r10), %r9
	vfmadd132sd	%xmm10, %xmm4, %xmm0
	vmovsd	(%r15,%rbx,8), %xmm4
	vaddsd	(%r15,%r9,8), %xmm4, %xmm4
	leaq	(%rax,%rsi), %r9
	vaddsd	(%r15,%r9,8), %xmm4, %xmm4
	leaq	(%rax,%r10), %r9
	vaddsd	(%r15,%r9,8), %xmm4, %xmm4
	leaq	(%r8,%rdi), %r9
	addq	%r11, %r8
	addq	%rax, %rdi
	addq	%r11, %rax
	vfmadd132sd	%xmm9, %xmm0, %xmm4
	vmovsd	(%r15,%r9,8), %xmm0
	vaddsd	(%r15,%r8,8), %xmm0, %xmm0
	vaddsd	(%r15,%rdi,8), %xmm0, %xmm0
	vaddsd	(%r15,%rax,8), %xmm0, %xmm0
	leaq	(%rcx,%rsi), %rax
	addq	%rdx, %rsi
	addq	%r10, %rcx
	addq	%r10, %rdx
	vaddsd	(%r15,%rax,8), %xmm0, %xmm0
	movq	1160(%rsp), %rax
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	vaddsd	(%r15,%rcx,8), %xmm0, %xmm0
	vaddsd	(%r15,%rdx,8), %xmm0, %xmm0
	vfmadd132sd	%xmm8, %xmm4, %xmm0
	vmovsd	%xmm0, (%rax,%r12,8)
.L97:
	movq	1232(%rsp), %rax
	movl	1376(%rsp), %edi
	addq	%r13, 1112(%rsp)
	addq	%rax, 1104(%rsp)
	addq	%rax, 1168(%rsp)
	addq	%rax, 1176(%rsp)
	addq	%rax, 1352(%rsp)
	addq	%rax, 1360(%rsp)
	addq	%rax, 1152(%rsp)
	addq	%r13, 1120(%rsp)
	addq	%r13, 1144(%rsp)
	addq	%r13, 1136(%rsp)
	addq	%r13, 1128(%rsp)
	addq	%rax, 1328(%rsp)
	cmpl	%edi, 1088(%rsp)
	jne	.L90
	movq	%r13, %r12
	movq	856(%rsp), %rsi
	movq	848(%rsp), %r13
	movq	840(%rsp), %r11
	movq	832(%rsp), %r10
	movq	824(%rsp), %rdx
	movl	812(%rsp), %edi
	movq	800(%rsp), %r8
	movq	792(%rsp), %r9
	movl	768(%rsp), %ecx
.L89:
	movl	596(%rsp), %eax
	subl	$3, %edi
	subq	%r9, 864(%rsp)
	subq	%r9, %rsi
	subq	%r9, %r13
	subq	%r8, %r11
	subq	%r8, %r10
	subq	%r8, %rdx
	cmpl	%eax, %edi
	jne	.L101
	cmpl	$-7, 1384(%rsp)
	movl	760(%rsp), %r12d
	movq	752(%rsp), %rbx
	jl	.L118
	movl	$0, %eax
	je	.L265
	cmpq	$16, 232(%rsp)
	ja	.L282
.L265:
	movq	616(%rsp), %rcx
	movq	152(%rsp), %rsi
	movq	816(%rsp), %rdi
	.p2align 4,,10
	.p2align 3
.L112:
	vmovsd	(%rdi,%rax,8), %xmm0
	movq	%rax, %rdx
	vmovsd	%xmm0, (%rcx,%rax,8)
	incq	%rax
	cmpq	%rdx, %rsi
	jne	.L112
	cmpl	$-7, 1384(%rsp)
	jne	.L114
.L109:
	movq	1232(%rsp), %rax
	movq	616(%rsp), %rdx
	movq	144(%rsp), %rcx
	movq	816(%rsp), %rsi
	.p2align 4,,10
	.p2align 3
.L122:
	vmovsd	(%rsi,%rax), %xmm0
	vmovsd	%xmm0, (%rdx,%rax)
	addq	$8, %rax
	cmpq	%rcx, %rax
	jne	.L122
.L118:
	addl	$4, %r12d
	cmpl	%r12d, 244(%rsp)
	jg	.L21
.L285:
	movl	4(%rsp), %r13d
	movq	672(%rsp), %r15
	leal	-4(%r13), %eax
	shrl	$2, %eax
	leal	4(,%rax,4), %edx
.L20:
	cmpl	%r13d, %edx
	jge	.L125
	movq	1232(%rsp), %rbx
	movl	1384(%rsp), %esi
	shrq	$3, %rbx
	testl	%esi, %esi
	jle	.L125
	movl	%esi, %ecx
	leal	-1(%rsi), %eax
	andl	$-4, %esi
	movq	%rbx, %r11
	shrl	$2, %ecx
	leaq	(%rbx,%rbx), %rdi
	movl	%eax, 992(%rsp)
	salq	$5, %r11
	salq	$5, %rcx
	addq	%rdi, %rax
	movq	%rdi, 1136(%rsp)
	addq	%rbx, %rdi
	movq	%rcx, 1184(%rsp)
	leal	4(%rsi), %ecx
	leaq	0(,%rdi,8), %r8
	movq	816(%rsp), %r14
	movl	%ecx, 880(%rsp)
	movl	612(%rsp), %ecx
	leaq	24(,%rax,8), %r10
	movq	%rbx, %r9
	movl	%esi, 888(%rsp)
	movq	%rbx, %rsi
	vmovsd	.LC1(%rip), %xmm13
	movq	%r15, %rbx
	addl	$2, %ecx
	movq	%r11, 848(%rsp)
	salq	$4, %rsi
	vmovsd	.LC3(%rip), %xmm12
	movl	%ecx, 1168(%rsp)
	vmovsd	.LC5(%rip), %xmm11
	movq	%rsi, %r12
	vmovsd	.LC7(%rip), %xmm10
	vmovsd	.LC9(%rip), %xmm9
	movl	%r13d, 840(%rsp)
	movq	%rdi, 832(%rsp)
	movq	%r8, 824(%rsp)
	movq	%r10, 816(%rsp)
.L126:
	movl	%edx, %eax
	incl	%edx
	movq	1136(%rsp), %r11
	movq	$0, 1336(%rsp)
	andl	$1, %eax
	movq	$0, 1160(%rsp)
	vmovsd	.LC11(%rip), %xmm8
	movq	%r9, %r13
	negq	%rax
	vmovsd	.LC1(%rip), %xmm7
	vmovsd	.LC3(%rip), %xmm6
	movl	$2, 1388(%rsp)
	andq	%rbx, %rax
	vmovsd	.LC5(%rip), %xmm5
	movq	%rax, %rdi
	movq	%rax, 1128(%rsp)
	leaq	0(,%rax,8), %rax
	leaq	(%r14,%rax), %r15
	movq	%rax, 1360(%rsp)
	movl	%edx, %eax
	andl	$1, %eax
	negq	%rax
	andq	%rbx, %rax
	leaq	(%r14,%rax,8), %rsi
	movq	%rax, 1120(%rsp)
	movq	%rax, %rcx
	movq	816(%rsp), %rax
	movq	%rsi, 1176(%rsp)
	addq	%r15, %rax
	movq	%rax, 1152(%rsp)
	leaq	(%rdi,%r11), %rax
	leaq	0(,%rax,8), %rdi
	subq	%r9, %rax
	movq	%rdi, 1200(%rsp)
	leaq	0(,%rax,8), %rdi
	movq	%rdi, 1192(%rsp)
	movq	832(%rsp), %rdi
	movq	%rax, 1072(%rsp)
	addq	%rdi, %rax
	movq	%rax, 1048(%rsp)
	leaq	0(,%rax,8), %rdi
	subq	%r9, %rax
	movq	%rax, 1040(%rsp)
	salq	$3, %rax
	movq	%rax, 1328(%rsp)
	leaq	(%r11,%rcx), %rax
	salq	$3, %rax
	movq	%rdi, 1368(%rsp)
	movq	%rax, 1144(%rsp)
	addq	%r14, %rax
	movq	%rax, 1000(%rsp)
	leaq	(%r15,%r12), %rax
	movq	%rax, 1016(%rsp)
	movq	1232(%rsp), %rax
	addq	%r15, %rax
	movq	%rax, 1008(%rsp)
	movq	824(%rsp), %rax
	addq	%r15, %rax
	movq	%rax, 960(%rsp)
	movq	848(%rsp), %rax
	addq	%r15, %rax
	movq	%rax, 928(%rsp)
	leaq	(%rsi,%r12), %rax
	movq	%rax, 896(%rsp)
	vmovsd	.LC7(%rip), %xmm4
	movq	%rbx, 872(%rsp)
	movq	%r12, 864(%rsp)
	movl	%edx, 856(%rsp)
	.p2align 4,,10
	.p2align 3
.L138:
	movl	1388(%rsp), %eax
	movl	%eax, %r12d
	incl	%eax
	cmpl	$1, 1384(%rsp)
	movl	%eax, 1388(%rsp)
	leal	-1(%r12), %eax
	movl	%eax, 1376(%rsp)
	leal	-2(%r12), %eax
	movl	%eax, 1352(%rsp)
	leal	2(%r12), %eax
	movl	%eax, 1344(%rsp)
	je	.L135
	movq	1144(%rsp), %rdi
	movq	1336(%rsp), %rax
	movq	1192(%rsp), %rcx
	leaq	32(%rax,%rdi), %rax
	movq	1200(%rsp), %rdi
	addq	$24, %rcx
	movq	%rax, %rdx
	subq	%rcx, %rdx
	leaq	24(%rdi), %rsi
	movq	%rax, %rcx
	movq	1360(%rsp), %rdi
	cmpq	$48, %rdx
	seta	%dl
	subq	%rsi, %rcx
	leaq	24(%rdi), %rsi
	cmpq	$48, %rcx
	seta	%cl
	andl	%edx, %ecx
	movq	%rax, %rdx
	subq	%rsi, %rdx
	cmpq	$48, %rdx
	seta	%dl
	andl	%ecx, %edx
	movq	1368(%rsp), %rcx
	leaq	24(%rcx), %rsi
	movq	%rax, %rcx
	subq	%rsi, %rcx
	cmpq	$48, %rcx
	seta	%cl
	testb	%cl, %dl
	je	.L135
	movq	1328(%rsp), %rdi
	leaq	24(%rdi), %rdx
	subq	%rdx, %rax
	cmpq	$48, %rax
	jbe	.L135
	cmpl	$2, 992(%rsp)
	jbe	.L158
	leaq	32(%r14), %rax
	movq	%rdi, %r10
	movq	1200(%rsp), %r11
	movq	1192(%rsp), %r9
	leaq	(%rax,%r10), %rcx
	leaq	40(%r14), %rdx
	movq	1360(%rsp), %r8
	vbroadcastsd	.LC1(%rip), %ymm17
	movq	%rcx, 1080(%rsp)
	leaq	24(%r14), %rcx
	leaq	(%r11,%rax), %rdi
	vbroadcastsd	.LC3(%rip), %ymm16
	leaq	(%r11,%rcx), %rsi
	leaq	(%r11,%rdx), %rbx
	movq	%rdi, 1056(%rsp)
	vbroadcastsd	.LC5(%rip), %ymm15
	movq	%rsi, 1088(%rsp)
	movq	1368(%rsp), %rsi
	leaq	(%rax,%r9), %rdi
	movq	%rbx, 1096(%rsp)
	leaq	(%rax,%r8), %rbx
	addq	%rsi, %rax
	leaq	16(%r14), %rsi
	movq	%rax, 1216(%rsp)
	leaq	(%r11,%rsi), %rax
	movq	%rax, 1224(%rsp)
	leaq	48(%r14), %rax
	addq	%rax, %r11
	movq	%r11, 1240(%rsp)
	leaq	(%rcx,%r9), %r11
	movq	%r11, 1104(%rsp)
	leaq	(%rcx,%r10), %r11
	movq	%r11, 1248(%rsp)
	leaq	(%rdx,%r9), %r11
	movq	%r11, 1256(%rsp)
	leaq	(%rdx,%r10), %r11
	leaq	(%rsi,%r8), %r10
	movq	%r10, 1112(%rsp)
	movq	1368(%rsp), %r10
	movq	%r11, 1264(%rsp)
	leaq	(%rsi,%r10), %r11
	movq	%r11, 1272(%rsp)
	leaq	(%rax,%r8), %r11
	movq	%r9, %r8
	leaq	(%rsi,%r9), %r9
	movq	%r11, 1280(%rsp)
	leaq	(%rax,%r10), %r11
	movq	1328(%rsp), %r10
	movq	%r11, 1288(%rsp)
	leaq	(%rsi,%r10), %r11
	movq	%r11, 1296(%rsp)
	leaq	(%rax,%r8), %r11
	movq	%r11, 1304(%rsp)
	leaq	(%rax,%r10), %r11
	movq	1360(%rsp), %rax
	movq	1368(%rsp), %r10
	movq	%r11, 1312(%rsp)
	leaq	(%rcx,%rax), %r8
	addq	%rdx, %rax
	movq	1336(%rsp), %r11
	movq	%rax, 1320(%rsp)
	addq	%r10, %rdx
	movq	1000(%rsp), %rax
	addq	%r10, %rcx
	vbroadcastsd	.LC7(%rip), %ymm14
	movq	1112(%rsp), %r10
	movq	%rdx, 1208(%rsp)
	leaq	32(%rax,%r11), %rsi
	movq	%r15, 1032(%rsp)
	movq	1104(%rsp), %r11
	xorl	%eax, %eax
	vbroadcastsd	.LC9(%rip), %ymm3
	vbroadcastsd	.LC11(%rip), %ymm2
	movq	%r13, 1024(%rsp)
	movq	1056(%rsp), %r15
	movq	1088(%rsp), %r13
	movl	%r12d, 1112(%rsp)
	movq	%r14, 1104(%rsp)
	movq	1096(%rsp), %r12
	movq	1080(%rsp), %r14
	.p2align 4,,10
	.p2align 3
.L128:
	vmovupd	(%r14,%rax), %ymm1
	movq	1216(%rsp), %rdx
	vaddpd	(%rdi,%rax), %ymm1, %ymm0
	vmovupd	(%rdx,%rax), %ymm1
	movq	1224(%rsp), %rdx
	vaddpd	(%rbx,%rax), %ymm1, %ymm1
	vaddpd	0(%r13,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1240(%rsp), %rdx
	vaddpd	(%r12,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1248(%rsp), %rdx
	vmulpd	%ymm17, %ymm0, %ymm0
	vfmadd231pd	(%r15,%rax), %ymm16, %ymm0
	vfmadd132pd	%ymm15, %ymm0, %ymm1
	vmovupd	(%rdx,%rax), %ymm0
	movq	1256(%rsp), %rdx
	vaddpd	(%r11,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1264(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1272(%rsp), %rdx
	vfmadd132pd	%ymm14, %ymm1, %ymm0
	vmovupd	(%rdx,%rax), %ymm1
	movq	1280(%rsp), %rdx
	vaddpd	(%r10,%rax), %ymm1, %ymm1
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1288(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm1, %ymm1
	movq	1296(%rsp), %rdx
	vfmadd132pd	%ymm3, %ymm0, %ymm1
	vmovupd	(%rdx,%rax), %ymm0
	movq	1304(%rsp), %rdx
	vaddpd	(%r9,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1312(%rsp), %rdx
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1320(%rsp), %rdx
	vaddpd	(%r8,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1208(%rsp), %rdx
	vaddpd	(%rcx,%rax), %ymm0, %ymm0
	vaddpd	(%rdx,%rax), %ymm0, %ymm0
	movq	1184(%rsp), %rdx
	vfmadd132pd	%ymm2, %ymm1, %ymm0
	vmovupd	%ymm0, (%rsi,%rax)
	addq	$32, %rax
	cmpq	%rdx, %rax
	jne	.L128
	movl	1384(%rsp), %eax
	movl	888(%rsp), %ecx
	movq	1032(%rsp), %r15
	movq	1024(%rsp), %r13
	movl	1112(%rsp), %r12d
	movq	1104(%rsp), %r14
	cmpl	%ecx, %eax
	je	.L133
	subl	%ecx, %eax
	cmpl	$1, %eax
	je	.L159
	movl	880(%rsp), %esi
	movl	%esi, 1320(%rsp)
.L127:
	movq	1160(%rsp), %r11
	movq	1072(%rsp), %rsi
	movq	1136(%rsp), %rdi
	movq	1128(%rsp), %rbx
	leaq	(%rsi,%r11), %rdx
	movq	1040(%rsp), %rsi
	leaq	4(%rcx,%rdx), %r10
	leaq	(%rdi,%r11), %r9
	leaq	(%rsi,%r11), %rdx
	vmovupd	(%r14,%r10,8), %xmm0
	addq	%rcx, %r9
	leaq	0(,%r10,8), %r8
	leaq	4(%rcx,%rdx), %rsi
	leaq	4(%rbx,%r9), %rdi
	movq	%rbx, %rdx
	movq	1048(%rsp), %rbx
	vaddpd	(%r14,%rsi,8), %xmm0, %xmm0
	addq	%r11, %rdx
	vmovupd	(%r14,%rdi,8), %xmm3
	addq	%rbx, %r11
	leaq	4(%rcx,%rdx), %rdx
	movq	1120(%rsp), %rbx
	leaq	4(%rcx,%r11), %rcx
	salq	$3, %rdx
	vaddpd	-8(%r14,%rdi,8), %xmm0, %xmm0
	salq	$3, %rcx
	leaq	4(%rbx,%r9), %r9
	vaddpd	8(%r14,%rdi,8), %xmm0, %xmm0
	vmulpd	.LC1(%rip){1to2}, %xmm0, %xmm0
	vfmadd231pd	.LC3(%rip){1to2}, %xmm3, %xmm0
	vmovupd	(%r14,%rcx), %xmm3
	vaddpd	(%r14,%rdx), %xmm3, %xmm1
	vmovupd	-8(%r14,%rsi,8), %xmm3
	vaddpd	-16(%r14,%rdi,8), %xmm1, %xmm1
	vaddpd	16(%r14,%rdi,8), %xmm1, %xmm1
	vfmadd132pd	.LC5(%rip){1to2}, %xmm0, %xmm1
	vaddpd	-8(%r14,%r8), %xmm3, %xmm0
	vmovupd	-16(%r14,%rcx), %xmm3
	vaddpd	8(%r14,%r8), %xmm0, %xmm0
	vaddpd	8(%r14,%rsi,8), %xmm0, %xmm0
	vfmadd132pd	.LC7(%rip){1to2}, %xmm1, %xmm0
	vaddpd	-16(%r14,%rdx), %xmm3, %xmm1
	vmovupd	-16(%r14,%rsi,8), %xmm3
	vaddpd	16(%r14,%rdx), %xmm1, %xmm1
	vaddpd	16(%r14,%rcx), %xmm1, %xmm1
	vfmadd132pd	.LC9(%rip){1to2}, %xmm0, %xmm1
	vaddpd	-16(%r14,%r8), %xmm3, %xmm0
	vaddpd	16(%r14,%r8), %xmm0, %xmm0
	vaddpd	16(%r14,%rsi,8), %xmm0, %xmm0
	vaddpd	-8(%r14,%rdx), %xmm0, %xmm0
	vaddpd	8(%r14,%rdx), %xmm0, %xmm0
	vaddpd	-8(%r14,%rcx), %xmm0, %xmm0
	vaddpd	8(%r14,%rcx), %xmm0, %xmm0
	vfmadd132pd	.LC11(%rip){1to2}, %xmm1, %xmm0
	vmovupd	%xmm0, (%r14,%r9,8)
	testb	$1, %al
	je	.L133
	movl	1320(%rsp), %ecx
	andl	$-2, %eax
	addl	%ecx, %eax
.L130:
	movslq	1388(%rsp), %r11
	movslq	1376(%rsp), %rdi
	movslq	%eax, %rbx
	movslq	%r12d, %r9
	imulq	%r13, %r9
	leal	-1(%rax), %ecx
	movslq	1344(%rsp), %r10
	imulq	%r13, %r11
	movslq	%ecx, %rcx
	leal	1(%rax), %edx
	imulq	%r13, %rdi
	movslq	%edx, %rdx
	imulq	%r13, %r10
	leaq	(%rbx,%r9), %r12
	leaq	(%rbx,%r11), %r8
	vmovsd	(%r15,%r8,8), %xmm0
	leaq	(%rbx,%rdi), %rsi
	leal	-2(%rax), %r8d
	addl	$2, %eax
	movl	%eax, 1376(%rsp)
	leaq	(%rbx,%r10), %rax
	movslq	%r8d, %r8
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	leaq	(%rcx,%r9), %rsi
	vmovsd	(%r15,%rax,8), %xmm1
	movslq	1376(%rsp), %rax
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	leaq	(%rdx,%r9), %rsi
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	movslq	1352(%rsp), %rsi
	imulq	%r13, %rsi
	vmulsd	%xmm13, %xmm0, %xmm0
	addq	%rsi, %rbx
	vaddsd	(%r15,%rbx,8), %xmm1, %xmm1
	leaq	(%r8,%r9), %rbx
	addq	%rax, %r9
	vfmadd231sd	(%r15,%r12,8), %xmm12, %xmm0
	vaddsd	(%r15,%rbx,8), %xmm1, %xmm1
	leaq	(%rcx,%r11), %rbx
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%rcx,%rdi), %r9
	vfmadd132sd	%xmm11, %xmm0, %xmm1
	vmovsd	(%r15,%rbx,8), %xmm0
	leaq	(%r8,%r10), %rbx
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%rdi), %r9
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%rdx,%r11), %r9
	vaddsd	(%r15,%r9,8), %xmm0, %xmm0
	leaq	(%r8,%rsi), %r9
	vfmadd132sd	%xmm10, %xmm1, %xmm0
	vmovsd	(%r15,%rbx,8), %xmm1
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%rax,%rsi), %r9
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%rax,%r10), %r9
	vaddsd	(%r15,%r9,8), %xmm1, %xmm1
	leaq	(%r8,%r11), %r9
	addq	%rdi, %r8
	addq	%rax, %rdi
	addq	%r11, %rax
	vfmadd132sd	%xmm9, %xmm0, %xmm1
	vmovsd	(%r15,%r9,8), %xmm0
	vaddsd	(%r15,%r8,8), %xmm0, %xmm0
	vaddsd	(%r15,%rdi,8), %xmm0, %xmm0
	vaddsd	(%r15,%rax,8), %xmm0, %xmm0
	leaq	(%rcx,%rsi), %rax
	addq	%rdx, %rsi
	addq	%r10, %rcx
	addq	%r10, %rdx
	vaddsd	(%r15,%rax,8), %xmm0, %xmm0
	movq	1176(%rsp), %rax
	vaddsd	(%r15,%rsi,8), %xmm0, %xmm0
	vaddsd	(%r15,%rcx,8), %xmm0, %xmm0
	vaddsd	(%r15,%rdx,8), %xmm0, %xmm0
	vfmadd132sd	%xmm8, %xmm1, %xmm0
	vmovsd	%xmm0, (%rax,%r12,8)
.L133:
	movq	1232(%rsp), %rax
	movl	1168(%rsp), %ecx
	addq	%r13, 1160(%rsp)
	addq	%rax, 1152(%rsp)
	addq	%rax, 1200(%rsp)
	addq	%rax, 1192(%rsp)
	addq	%rax, 1360(%rsp)
	addq	%rax, 1368(%rsp)
	addq	%rax, 1328(%rsp)
	addq	%rax, 1336(%rsp)
	cmpl	%ecx, 1388(%rsp)
	jne	.L138
	movl	856(%rsp), %edx
	movq	872(%rsp), %rbx
	movq	%r13, %r9
	movq	864(%rsp), %r12
	cmpl	%edx, 840(%rsp)
	jne	.L126
.L125:
	movq	1464(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L283
	movq	376(%rsp), %rdi
	vzeroupper
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	jmp	free@PLT
	.p2align 4,,10
	.p2align 3
.L98:
	.cfi_restore_state
	movq	1328(%rsp), %rbx
	movq	1000(%rsp), %rax
	movq	928(%rsp), %rdi
	movq	992(%rsp), %rsi
	movq	888(%rsp), %rcx
	movq	896(%rsp), %rdx
	addq	%rbx, %rax
	movq	880(%rsp), %r11
	vmovsd	.LC5(%rip), %xmm12
	leaq	16(%rbx,%rdi), %rdi
	addq	%rbx, %rsi
	vmovsd	.LC7(%rip), %xmm7
	vmovsd	.LC9(%rip), %xmm6
	addq	%rbx, %rcx
	leaq	16(%rbx,%rdx), %rdx
	vmovsd	.LC11(%rip), %xmm5
	movq	1104(%rsp), %r9
	leaq	(%r11,%rbx), %r8
	.p2align 4,,10
	.p2align 3
.L96:
	vmovsd	16(%rdi), %xmm0
	vmovsd	16(%rcx), %xmm4
	addq	$8, %rax
	addq	$8, %rdi
	addq	$8, %rsi
	addq	$8, %rcx
	addq	$8, %rdx
	addq	$8, %r8
	vaddsd	8(%rsi), %xmm0, %xmm0
	vaddsd	8(%rdx), %xmm4, %xmm4
	vaddsd	(%rax), %xmm0, %xmm0
	vaddsd	-8(%rax), %xmm4, %xmm4
	vaddsd	16(%rax), %xmm0, %xmm0
	vaddsd	24(%rax), %xmm4, %xmm4
	vmulsd	%xmm3, %xmm0, %xmm0
	vfmadd231sd	8(%rax), %xmm2, %xmm0
	vfmadd132sd	%xmm12, %xmm0, %xmm4
	vmovsd	(%rdi), %xmm0
	vaddsd	(%rsi), %xmm0, %xmm13
	vmovsd	-8(%rcx), %xmm0
	vaddsd	16(%rdi), %xmm13, %xmm13
	vaddsd	16(%rsi), %xmm13, %xmm13
	vfmadd132sd	%xmm7, %xmm4, %xmm13
	vaddsd	-8(%rdx), %xmm0, %xmm4
	vmovsd	-8(%rdi), %xmm0
	vaddsd	-8(%rsi), %xmm0, %xmm0
	vaddsd	24(%rcx), %xmm4, %xmm4
	vaddsd	24(%rdi), %xmm0, %xmm0
	vaddsd	24(%rdx), %xmm4, %xmm4
	vaddsd	24(%rsi), %xmm0, %xmm0
	vfmadd132sd	%xmm6, %xmm13, %xmm4
	vaddsd	(%rcx), %xmm0, %xmm0
	vaddsd	16(%rcx), %xmm0, %xmm0
	vaddsd	(%rdx), %xmm0, %xmm0
	vaddsd	16(%rdx), %xmm0, %xmm0
	vfmadd132sd	%xmm5, %xmm4, %xmm0
	vmovsd	%xmm0, -8(%r8)
	cmpq	%r9, %rax
	jne	.L96
	jmp	.L97
	.p2align 4,,10
	.p2align 3
.L135:
	movq	1336(%rsp), %rbx
	movq	960(%rsp), %rcx
	movq	1016(%rsp), %rax
	movq	1008(%rsp), %rdi
	movq	928(%rsp), %rdx
	movq	896(%rsp), %r11
	leaq	16(%rbx,%rcx), %rsi
	leaq	16(%r15,%rbx), %rcx
	vmovsd	.LC9(%rip), %xmm3
	vmovsd	.LC11(%rip), %xmm2
	leaq	16(%rbx,%rax), %rax
	leaq	16(%rbx,%rdi), %rdi
	movq	1152(%rsp), %r9
	leaq	16(%rbx,%rdx), %rdx
	leaq	32(%rbx,%r11), %r8
	.p2align 4,,10
	.p2align 3
.L132:
	vmovsd	16(%rsi), %xmm0
	vmovsd	16(%rdx), %xmm1
	addq	$8, %rax
	addq	$8, %rdi
	addq	$8, %rsi
	addq	$8, %rcx
	addq	$8, %rdx
	addq	$8, %r8
	vaddsd	8(%rdi), %xmm0, %xmm0
	vaddsd	8(%rcx), %xmm1, %xmm1
	vaddsd	(%rax), %xmm0, %xmm0
	vaddsd	-8(%rax), %xmm1, %xmm1
	vaddsd	16(%rax), %xmm0, %xmm0
	vaddsd	24(%rax), %xmm1, %xmm1
	vmulsd	%xmm7, %xmm0, %xmm0
	vfmadd231sd	8(%rax), %xmm6, %xmm0
	vfmadd132sd	%xmm5, %xmm0, %xmm1
	vmovsd	(%rsi), %xmm0
	vaddsd	(%rdi), %xmm0, %xmm14
	vmovsd	-8(%rdx), %xmm0
	vaddsd	16(%rdi), %xmm14, %xmm14
	vaddsd	16(%rsi), %xmm14, %xmm14
	vfmadd132sd	%xmm4, %xmm1, %xmm14
	vaddsd	-8(%rcx), %xmm0, %xmm1
	vmovsd	-8(%rsi), %xmm0
	vaddsd	-8(%rdi), %xmm0, %xmm0
	vaddsd	24(%rcx), %xmm1, %xmm1
	vaddsd	24(%rdi), %xmm0, %xmm0
	vaddsd	24(%rdx), %xmm1, %xmm1
	vaddsd	24(%rsi), %xmm0, %xmm0
	vfmadd132sd	%xmm3, %xmm14, %xmm1
	vaddsd	(%rcx), %xmm0, %xmm0
	vaddsd	16(%rcx), %xmm0, %xmm0
	vaddsd	(%rdx), %xmm0, %xmm0
	vaddsd	16(%rdx), %xmm0, %xmm0
	vfmadd132sd	%xmm2, %xmm1, %xmm0
	vmovsd	%xmm0, -8(%r8)
	cmpq	%rax, %r9
	jne	.L132
	jmp	.L133
.L152:
	movl	$4, 1320(%rsp)
	movl	1384(%rsp), %eax
	xorl	%ecx, %ecx
	jmp	.L91
.L153:
	movl	704(%rsp), %eax
	jmp	.L94
.L282:
	cmpl	$2, 240(%rsp)
	jbe	.L105
	movq	616(%rsp), %rdx
	movq	136(%rsp), %rcx
	movq	816(%rsp), %rsi
.L106:
	vmovupd	(%rsi,%rax), %ymm6
	vmovupd	%ymm6, (%rdx,%rax)
	addq	$32, %rax
	cmpq	%rcx, %rax
	jne	.L106
	testb	$3, 592(%rsp)
	je	.L271
	movl	364(%rsp), %esi
	movl	1384(%rsp), %eax
	subl	%esi, %eax
	movl	%esi, %ecx
	leal	8(%rax), %edx
	cmpl	$-7, %eax
	je	.L284
.L110:
	movq	816(%rsp), %rdi
	movl	%ecx, %eax
	movq	672(%rsp), %rcx
	vmovupd	(%rdi,%rax,8), %xmm0
	addq	%rcx, %rax
	vmovupd	%xmm0, (%rdi,%rax,8)
	movl	%edx, %eax
	andl	$-2, %eax
	addl	%esi, %eax
	andl	$1, %edx
	je	.L114
	cltq
	vmovsd	(%rdi,%rax,8), %xmm0
	movq	616(%rsp), %rdi
	vmovsd	%xmm0, (%rdi,%rax,8)
.L114:
	cmpq	$16, 352(%rsp)
	jbe	.L109
	cmpl	$2, 240(%rsp)
	jbe	.L115
.L108:
	movq	16(%rsp), %rdx
	movq	136(%rsp), %rcx
	xorl	%eax, %eax
	movq	8(%rsp), %rsi
.L116:
	vmovupd	(%rsi,%rax), %ymm6
	vmovupd	%ymm6, (%rdx,%rax)
	addq	$32, %rax
	cmpq	%rcx, %rax
	jne	.L116
	testb	$3, 592(%rsp)
	je	.L118
	movl	1384(%rsp), %eax
	movl	364(%rsp), %edi
	subl	%edi, %eax
	leal	8(%rax), %edx
	cmpl	$-7, %eax
	je	.L157
	movl	%edi, %ecx
	movl	%edi, %eax
.L140:
	movq	1272(%rsp), %rdi
	movq	672(%rsp), %rsi
	addq	%rdi, %rax
	movq	816(%rsp), %rdi
	vmovupd	(%rdi,%rax,8), %xmm0
	addq	%rsi, %rax
	vmovupd	%xmm0, (%rdi,%rax,8)
	testb	$1, %dl
	je	.L118
	movl	%edx, %eax
	andl	$-2, %eax
	addl	%ecx, %eax
.L120:
	movq	1272(%rsp), %rdi
	cltq
	addl	$4, %r12d
	addq	%rdi, %rax
	movq	816(%rsp), %rdi
	vmovsd	(%rdi,%rax,8), %xmm0
	movq	616(%rsp), %rdi
	vmovsd	%xmm0, (%rdi,%rax,8)
	cmpl	%r12d, 244(%rsp)
	jg	.L21
	jmp	.L285
.L148:
	movl	704(%rsp), %eax
	jmp	.L25
.L281:
	incl	%ecx
	jmp	.L89
.L275:
	incl	%edx
	jmp	.L29
.L158:
	movl	$4, 1320(%rsp)
	movl	1384(%rsp), %eax
	xorl	%ecx, %ecx
	jmp	.L127
.L159:
	movl	880(%rsp), %eax
	jmp	.L130
.L149:
	movl	$8, %ecx
	jmp	.L46
.L284:
	movq	816(%rsp), %rdi
	movslq	%esi, %rax
	vmovsd	(%rdi,%rax,8), %xmm0
	movq	616(%rsp), %rdi
	vmovsd	%xmm0, (%rdi,%rax,8)
.L271:
	cmpq	$16, 352(%rsp)
	jbe	.L109
	jmp	.L108
.L115:
	movl	592(%rsp), %edx
	xorl	%ecx, %ecx
	xorl	%eax, %eax
	jmp	.L140
.L105:
	movl	592(%rsp), %edx
	xorl	%ecx, %ecx
	xorl	%esi, %esi
	jmp	.L110
.L157:
	movl	%edi, %eax
	jmp	.L120
.L146:
	xorl	%edx, %edx
	jmp	.L20
.L273:
	call	__stack_chk_fail@PLT
.L283:
	vzeroupper
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE6470:
	.size	vectime, .-vectime
	.section	.rodata.cst32,"aM",@progbits,32
	.align 32
.LC0:
	.long	0
	.long	1069809664
	.long	0
	.long	1069809664
	.long	0
	.long	1069809664
	.long	0
	.long	1069809664
	.set	.LC1,.LC0
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC3:
	.long	0
	.long	1071185920
	.align 8
.LC5:
	.long	0
	.long	-1079771136
	.align 8
.LC7:
	.long	0
	.long	1068498944
	.section	.rodata.cst32
	.align 32
.LC8:
	.long	0
	.long	1064304640
	.long	0
	.long	1064304640
	.long	0
	.long	1064304640
	.long	0
	.long	1064304640
	.set	.LC9,.LC8
	.align 32
.LC10:
	.long	0
	.long	-1081081856
	.long	0
	.long	-1081081856
	.long	0
	.long	-1081081856
	.long	0
	.long	-1081081856
	.set	.LC11,.LC10
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
