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
#include "libcnt/stack.h"
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include <assert.h>
#include <string.h>

struct cntStack {
	unsigned next;
	unsigned max_size;
	char resizeable;
	char *entries;
	unsigned sizeof_value;
};

/* TODO : stack should use shelf, they must NOT relocate */

cntStack *cntStack_new(unsigned sizeof_value, unsigned max_size, int resizeable)
{
	cntStack *this;
	this = mem_alloc(sizeof(*this));
	if (!this) return 0;
	this->entries = mem_alloc(max_size*sizeof_value);
	if (!this->entries) {
		mem_unregister(this);
		return 0;
	}
	this->max_size = max_size;
	this->next = 0;
	this->resizeable = resizeable;
	this->sizeof_value = sizeof_value;
	return this;
}

void cntStack_del(cntStack *this)
{
	mem_unregister(this->entries);
	mem_unregister(this);
}

void *cntStack_push(cntStack *this, void const *value) {
	assert(this->next<=this->max_size);
	if (this->next == this->max_size) {
		this->max_size <<= 1;
		if (!(this->entries = mem_realloc(this->entries, this->max_size*this->sizeof_value)) ) {
			log_fatal("Can't resize stack");
		}
	}
	void *ptr = this->entries+(this->next++)*this->sizeof_value;
	if (value) memcpy(ptr, value, this->sizeof_value);
	return ptr;
}

void *cntStack_pop(cntStack *this)
{
	assert(this->next>0);
	this->next --;
	return this->entries+this->next*this->sizeof_value;
}

void cntStack_clear(cntStack *this)
{
	this->next = 0;
}

unsigned cntStack_size(cntStack *this)
{
	return this->next;
}

void *cntStack_get(cntStack *this, unsigned depth) {
	assert(this);
	if (depth>=this->next) return NULL;
	return this->entries+(this->next-depth-1)*this->sizeof_value;
}

// vi:ts=3:sw=3
