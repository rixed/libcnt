/* This file is part of libcnt.
 *
 * Copyright (C) 2005 Cedric Cellier.
 *
 * Libcnt is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * Libcnt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MicroModel; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
/*
 * This section is for GNU assembler only
 * And the C compilator used to compile the others functions of the av_list library should follow the
 * convention for passing parameters of your architecture.
 * When unclear, we use GNU CC conventions (for structs and unions).
 */
.text
	.align 4
.globl av_call

#ifdef i386

av_call:
	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %ebp
	movl 20(%esp),%ebx	/* ebx = addy of av_list */
	movl (%ebx),%ecx	/* ecx = nb_parameters */
	movl %esp,%ebp	/* save esp into ebp */
.L1:	/* from last to first */
	subl $1,%ecx
	jl .Lcall
	movl 20(%ebx,%ecx,8),%eax	/* eax = addy of the parameter */
	movl 24(%ebx,%ecx,8),%edx	/* edx = size in bytes of the parameter */
.Lpush:
	pushl (%eax)	/* OK for types <= 4 bytes, little endian rulez ! */
	addl $4,%eax
	subl $4,%edx
	jg .Lpush
	jmp .L1
.Lcall:
	call *8(%ebx)
	movl %ebp,%esp	/* restore esp */
	movl 16(%ebx),%ecx	/* ecx = size of ret_value */
	or %ecx,%ecx			/* if zero, skip */
	jz .Lend
	movl 12(%ebx),%edx	/* edx = addy of ret_value */
	movl %eax,(%edx)		/* write ret_value */
	cmp $4,%ecx
	jbe .Lend
	/* multi dword return value - where do I get it ?? */
.Lend:
	movl 24(%esp),%eax	/* eax = addy of pointer to ret value */
	movl %edx,(%eax)	/* write the addy of returned value on the caller's void* */
	movl $1,%eax
	popl %ebp
	popl %edx
	popl %ecx
	popl %ebx
	ret

#else
#error NO CODE FOR THIS MACHINE
#endif
