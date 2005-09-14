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
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include "libcnt/sys/mutex.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct head {
	unsigned mark;
	unsigned size;
	unsigned ref_count;
	struct head *next, *prec;
#ifndef NDEBUG
	char const *caller_file;
   size_t caller_line;
#endif
	char block[];
};

static sys_mutex mutex;	// MUST NOT be a mutex* (we cannot allocate memory yet)
static volatile unsigned size = 0;
static volatile unsigned used, max_used;
static struct head * volatile first, * volatile last;

int mem_init(unsigned new_size) {
	assert(size==0);
	used = max_used = 0;
	size = new_size;
	first = last = NULL;
	if (! sys_mutex_construct(&mutex) ) return 0;
	return 1;
}

void mem_end() {
	if (used) {
		log_warning(LOG_MUSTSEE, "some memory left but asking to end !");
#ifndef NDEBUG
		struct head *ptr = first;
		while (ptr) {
			log_warning(LOG_MUSTSEE, "still %u at %p for %s:%u", ptr->size, ptr, ptr->caller_file, ptr->caller_line);
			ptr = ptr->next;
		};
		log_warning(LOG_MUSTSEE, "still %u at %p for %s:%u", ptr->size, ptr, ptr->caller_file, ptr->caller_line);
#endif
	}
	size = 0;
	log_warning(LOG_DEBUG, "max used : %u", max_used);
	max_used = 0;
	sys_mutex_destruct(&mutex);
}

unsigned mem_blocksize(const void *mem) {
	assert(mem && size);
	mem = (const char*)mem - sizeof(const struct head);
	return ((const struct head*)mem)->size;
}

unsigned mem_refcount(void *mem) {
	assert(mem && size);
	mem = (char*)mem - sizeof(struct head);
	return ((struct head*)mem)->ref_count;
}

static char mem_isvalid(const void *mem) {
	assert(mem && size);
	mem = (const char*)mem - sizeof(struct head);
	return ((const struct head*)mem)->mark==0x280176;
}

void *mem_alloc_d(unsigned requested_size, char const *file, size_t line, bool catcherr) {
	char *mem;
	assert(size);
	mem = malloc(requested_size+sizeof(struct head));
	if (mem) {
		sys_mutex_blockget(&mutex);
		if (!first) {
			first = (struct head*)mem;
		} else {
			((struct head*)last)->next = (struct head*)mem;
		}
		((struct head*)mem)->mark = 0x280176;
		((struct head*)mem)->size = requested_size;
		((struct head*)mem)->ref_count = 1;
		((struct head*)mem)->prec = last;
		((struct head*)mem)->next = NULL;
#ifndef NDEBUG
		((struct head*)mem)->caller_file = file;
		((struct head*)mem)->caller_line = line;
#endif
		last = (struct head*)mem;
		sys_mutex_release(&mutex);
		log_warning(LOG_DEBUG, "alloc %d bytes at %p (used = %d) for %s:%u", requested_size, mem, used, file, line);
		mem += sizeof(struct head);
		sys_mutex_blockget(&mutex);
		used += requested_size;
		if (used > max_used) max_used = used;
		sys_mutex_release(&mutex);
	} else if (catcherr) {
		log_fatal("Out of memory");
	}
	return (void*)mem;
}

void *mem_realloc(void *mem, unsigned new_size) {
	char *ptr;
	struct head *p2;
	assert(size);
	if (!mem) {
		return mem_alloc(new_size);
	}
	assert(mem_isvalid(mem));
	mem = (char*)mem - sizeof(struct head);
	ptr = realloc(mem, new_size+sizeof(struct head));
	if (!ptr) return 0;
	sys_mutex_blockget(&mutex);
	used += (new_size - ((struct head*)ptr)->size);
	if (used > max_used) max_used = used;
	((struct head*)ptr)->size = new_size;
	if (mem!=first) {
		p2 = ((struct head*)ptr)->prec;
		assert(p2->mark==0x280176);
		p2->next = (struct head*)ptr;
	} else {
		first = (struct head*)ptr;
	}
	if (mem!=last) {
		p2 = ((struct head*)ptr)->next;
		assert(p2->mark==0x280176);
		p2->prec = (struct head*)ptr;
	} else {
		last = (struct head*)ptr;
	}
	sys_mutex_release(&mutex);
	log_warning(LOG_DEBUG, "realloc %p at %p for %d bytes (used = %d)", mem, ptr, new_size, used);
	return ptr+sizeof(struct head);
}

void mem_unregister(void *mem) {
	struct head *p2;
	assert(size);
	if (!mem) return;
	log_warning(LOG_DEBUG, "unregister at %p", mem);
	assert(mem_isvalid(mem));
	mem = (char*)mem - sizeof(struct head);
	if (0 == --((struct head*)mem)->ref_count) {
		sys_mutex_blockget(&mutex);
		used -= ((struct head*)mem)->size;
		if (mem!=first) {
			p2 = ((struct head*)mem)->prec;
			assert(p2->mark==0x280176);
			p2->next = ((struct head*)mem)->next;
		} else {
			first = ((struct head*)mem)->next;
		}
		if (mem!=last) {
			p2 = ((struct head*)mem)->next;
			assert(p2->mark==0x280176);
			p2->prec = ((struct head*)mem)->prec;
		} else {
			last = ((struct head*)mem)->prec;
		}
		sys_mutex_release(&mutex);
		log_warning(LOG_DEBUG, "free at %p (used = %d)", mem, used);
#ifndef NDEBUG
		memset(((struct head*)mem)->block, 0xce, ((struct head*)mem)->size);
#endif
		free(mem);
	}
}

int mem_register(void *mem) {
	/* mem MUST be the adress of the first byte of a block */
	assert(size && mem);
	assert(mem_isvalid(mem));
	mem = (char*)mem - sizeof(struct head);
	((struct head*)mem)->ref_count++;
	return 1;
}

int mem_consumption() {
	assert(size);
	return (100L*used)/size;
}

#ifndef NDEBUG
// the two followings can be used in gdb
#include <stdio.h>
void mem_print() {
	assert(size);
	if (!first) return;
	struct head *ptr = first;
	unsigned hop = 0;
	do {
		assert(ptr->mark == 0x280176);
		printf("%u@%p : %u bytes created by %s:%u, referenced %u time(s)\n", hop, (void *)ptr, ptr->size, ptr->caller_file, ptr->caller_line, ptr->ref_count);
		if (ptr==last) break;
		ptr = ptr->next;
		hop ++;
	} while (1);
	printf("Max memory used = %u bytes\n", max_used);
}

void mem_whohas(void *addr) {	// tells who own a given address (not necessarily beginning of block)
	struct head *ptr = first;
	unsigned hop = 0;
	while (ptr) {
		if ((char *)addr>=(char *)ptr && (char *)addr<=(char *)ptr + sizeof(struct head) + ptr->size) {	// we take <= here because we want max bound to be valid (especially when size=0)
			printf("found in %s of block %u@%p of size %u, created by %s:%u\n", ((char *)addr < (char *)ptr->block ? "HEADER":"datas"), hop, (void *)ptr, ptr->size, ptr->caller_file, ptr->caller_line);
			return;
		}
		ptr = ptr->next;
		hop ++;
	}
	printf("not found\n");
}

#endif
// vi:ts=3:sw=3
