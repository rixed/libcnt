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
#include "libcnt/list.h"
#include "libcnt/shelf.h"
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include <string.h>
#include <assert.h>
#include <stddef.h>

struct element {
	element *next;	// next=0 means last
	element *previous;	// previous = 0 means first
	char value[];
};

int cntList_construct(cntList *this, unsigned sizeof_value, unsigned nb_elements)
{
	assert(this);
	this->shelf = cntShelf_new(sizeof(element)+sizeof_value, nb_elements);
	if (!this->shelf) {
		return 0;
	}
	this->sizeof_value = sizeof_value;
	this->first = this->last = 0;
	this->last_accessed_element = NULL;
	return 1;
}

cntList *cntList_new(unsigned sizeof_value, unsigned nb_elements)
{
	cntList *this;
	this = mem_alloc(sizeof(*this));
	if (!this) return 0;
	if (!cntList_construct(this, sizeof_value, nb_elements)) {
		mem_unregister(this);
		return 0;
	}
	return this;
}

int cntList_destruct(cntList *this)
{
	assert(this);
	cntShelf_del(this->shelf);
	return 1;
}

void cntList_del(cntList *this)
{
	assert(this);
	(void)cntList_destruct(this);
	mem_unregister(this);
}

void *cntList_push(cntList *this, const void *value)
{
	element *ptr;
	assert(this);
	ptr = cntShelf_alloc(this->shelf);
	if (value) memmove(ptr->value, value, this->sizeof_value);
	ptr->next = 0;
	ptr->previous = this->last;
	if (this->last) this->last->next = ptr;
	else this->first = ptr;
	this->last = ptr;
	return ptr->value;
}

void cntList_pop(cntList *this, void *value)
{
	element *ptr;
	assert(this && this->last);
	ptr = this->last;
	if (ptr == this->last_accessed_element)
		this->last_accessed_element = NULL;
	this->last = ptr->previous;
	if (this->last) this->last->next = 0;
	else this->first = 0;
	if (value) memmove(value, ptr->value, this->sizeof_value);
	cntShelf_free(this->shelf, ptr);
}

static element *order_to_index(cntList *this, unsigned order)
{
	unsigned length = cntList_size(this);
	unsigned dend, dlast;
	unsigned dexterm, o;
	element *ptr;
	assert(order<length);
	dend = length-1-order;
	if (order < dend) {
		dexterm = order;
	} else {
		dexterm = dend;
	}
	if (!this->last_accessed_element)
		goto from_extrem;
	
	if (order < this->last_accessed_order) {
		dlast = this->last_accessed_order - order;
	} else {
		dlast = order - this->last_accessed_order;
	}
	if (dexterm < dlast)
		goto from_extrem;
	/* search index from last accessed */
	ptr = this->last_accessed_element;
	if (order > this->last_accessed_order) {
		while (dlast) {
			ptr = ptr->next;
			dlast--;
		}
	} else {
		while (dlast) {
			ptr = ptr->previous;
			dlast--;
		}
	}
cache_exit:
	this->last_accessed_order = order;
	this->last_accessed_element = ptr;
	assert(ptr);
	return ptr;
from_extrem:
	/* search index from extremums */
	o = dexterm;
	if (order < dend) {	/* from beginning */
		ptr = this->first;
		while (o) {
			ptr = ptr->next;
			o --;
		}
	} else {	/* from the end */
		ptr = this->last;
		while (o) {
			ptr = ptr->previous;
			o --;
		}
	}
	if (dexterm>5) goto cache_exit;
	assert(ptr);
	return ptr;
}

void *cntList_get(cntList *this, unsigned order)
{
	assert(this);
	if (order >= cntList_size(this)) return NULL;
	return order_to_index(this, order)->value;
}

void *cntList_set(cntList *this, unsigned order, const void *value)
{
	element *ptr;
	assert(this);
	if (order > cntList_size(this)) return NULL;
	if (order == cntList_size(this)) return cntList_push(this, value);
	ptr = order_to_index(this, order);
	if (value) memmove(ptr->value, value, this->sizeof_value);
	return ptr->value;
}

void *cntList_alloc_after_address(cntList *this, void *v_ptr)
{
	element *ptr;
	assert(this && v_ptr);
	ptr = (element *)((char*)v_ptr - offsetof(element, value));
	if (ptr->next) {
		ptr = ptr->next;
		return cntList_alloc_before_address(this, ptr->value);
	} else {
		return cntList_push(this, NULL);
	}
}

void *cntList_alloc_before_address(cntList *this, void *v_ptr)
{
	element *new, *previous, *ptr;
	assert(this && v_ptr);
	ptr = (element *)((char*)v_ptr - offsetof(element, value));
	new = cntShelf_alloc(this->shelf);
	previous = ptr->previous;
	new->previous = previous;
	new->next = ptr;
	ptr->previous = new;
	if (previous) previous->next = new;
	else this->first = new;
	this->last_accessed_element = NULL;
	return new->value;
}

static void *cntList_alloc(cntList *this, unsigned order)
{
	unsigned size;
	assert(this);
	size = cntList_size(this);
	if (order==size) {	/* we asked for a push */
		return cntList_push(this, NULL);
	} else {
		if (order>=size) return 0;
		void *ret;
		element *ptr;
		ptr = order_to_index(this, order);
		ret = cntList_alloc_before_address(this, &ptr->value);
		/* as we change the order of elements after this one... */
		this->last_accessed_element = (element *)((char*)ret - offsetof(element, value));
		this->last_accessed_order = order;
		return ret;
	}
}

void *cntList_insert(cntList *this, unsigned order, const void *value)
{
	assert(this);
	void *ptr = cntList_alloc(this, order);
	if (ptr && value) memmove(ptr, value, this->sizeof_value);
	return ptr;
}

static element *remove_by_element(cntList *this, element *ptr)
{
	assert(this);
	element *previous, *next;
	previous = ptr->previous;
	next = ptr->next;
	if (previous) previous->next = next;
	else this->first = next;
	if (next) next->previous = previous;
	else this->last = previous;
	cntShelf_free(this->shelf, ptr);
	/* as we change the order of elements after this one... */
	this->last_accessed_element = NULL;
	return next;
}

void cntList_remove(cntList *this, unsigned order)
{
	element *ptr;
	assert(this);
	ptr = order_to_index(this, order);
	this->last_accessed_element = remove_by_element(this, ptr);
	this->last_accessed_order = order;
}

void cntList_clear(cntList *this)
{
	assert(this);
	this->first = this->last = NULL;
	this->last_accessed_element = NULL;
	cntShelf_clear(this->shelf);
}

unsigned cntList_size(const cntList *this)
{
	return cntShelf_size(this->shelf);
}

int cntList_remove_by_value(cntList *this, void *value)
{
	unsigned s, size;
	assert(this);
	size = cntList_size(this);
	for (s=0; s<size; s++) {
		void *val = cntList_get(this, s);
		if (0==memcmp(val, value, this->sizeof_value)) {
			cntList_remove(this, s);
			break;
		}
	}
	return s<size;
}

void cntList_remove_by_address(cntList *this, void *v_ptr)
{
	assert(this);
	element *ptr = (element *)((char*)v_ptr - offsetof(element, value));
	remove_by_element(this, ptr);
}

void *cntList_next_by_address(const void *v_ptr)
{
	assert(v_ptr);
	const element *ptr = (const element *)((const char*)v_ptr - offsetof(element, value));
	if (!ptr->next) return 0;
	else return ptr->next->value;
}

void *cntList_prev_by_address(const void *v_ptr)
{
	assert(v_ptr);
	const element *ptr = (const element *)((const char*)v_ptr - offsetof(element, value));
	if (!ptr->previous) return 0;
	else return ptr->previous->value;
}


unsigned cntList_sizeof_value(const cntList *this)
{
	return this->sizeof_value;
}

void cntList_relink_by_address(cntList *this, void *dest, void *src) {
	// relink src so that it comes just before dest
	assert(this && dest && src);
	if (dest==src) return;
	element *ptr_s = (element *)((char*)src - offsetof(element, value));
	element *ptr_d = (element *)((char*)dest - offsetof(element, value));
	if (ptr_s->previous) ptr_s->previous->next = ptr_s->next;
	else {
		assert(this->first == ptr_s);
		this->first = ptr_s->next;
	}
	if (ptr_s->next) ptr_s->next->previous = ptr_s->previous;
	else {
		assert(this->last == ptr_s);
		this->last = ptr_s->previous;
	}
	ptr_s->previous = ptr_d->previous;
	ptr_s->next = ptr_d;
	ptr_d->previous = ptr_s;
	ptr_s->previous->next = ptr_s;
	if (this->first == ptr_d) this->first = ptr_s;
	/* as we change the order of elements after ptr_s... */
	this->last_accessed_element = NULL;
}

// vi:ts=3:sw=3
