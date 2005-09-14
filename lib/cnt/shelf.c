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
#include "libcnt/shelf.h"
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include <string.h>
#include <assert.h>

/* Data Definitions */

typedef union element {
	union element *next_free;
	unsigned char value[sizeof(union element *)];	// WARNING : false flexible array
} element;

typedef struct memzone {
	struct memzone *next;
	unsigned nb_elements;
	unsigned nb_used_elements;
	element *first_free;
	element data[];
} memzone;

struct cntShelf {
	unsigned nb_used_elements;
	unsigned sizeof_value;
	unsigned sizeof_element;	// = max(sizeof_value,sizeof(element*))
	memzone *each_mz;	// memzone of the last returned entry, or NULL if we ran out of memzones (each returned NULL)
	element *each_next_free;	// next free element after last returned element, or NULL if we ran out of memzones
	element *each_last;	// last returned element, or NULL after reset
	element *each_next;	// next element to be returned, or NULL if unknown
	memzone first_memzone;	// the warning here due to the improper use of flexible array in memzone can safely be ignored (so I guess).
};

/* Private Functions */

static void empty_chain(cntShelf *this, memzone *mz) {
	element *ptr, *ptr_last;
	assert (this && mz);
	ptr_last = (element *)((char *)mz->data + this->sizeof_element*(mz->nb_elements-1));
	mz->first_free = mz->data;
	for (ptr=mz->first_free; ptr!=ptr_last; ) {
		element *ptr_next = (element *)((char *)ptr + this->sizeof_element);
		ptr->next_free = ptr_next;
		ptr = ptr_next;
	}
	ptr_last->next_free = NULL;
}

static void recurs_memzone_del(cntShelf *this, memzone *mz) {
	if (!mz) return;
	recurs_memzone_del(this, mz->next);
	assert(this->nb_used_elements>=mz->nb_used_elements);
	this->nb_used_elements -= mz->nb_used_elements;
	mem_unregister(mz);
}

static void all_memzone_del(cntShelf *this) {
	assert(this);
	recurs_memzone_del(this, this->first_memzone.next);
	this->first_memzone.next = NULL;
	assert(this->nb_used_elements == this->first_memzone.nb_used_elements);
}

/* Public Functions */

cntShelf *cntShelf_new(unsigned sizeof_value, unsigned nb_elements) {
	cntShelf *this;
	unsigned sizeof_element;
	if (!sizeof_value || !nb_elements) return 0;
	sizeof_element = sizeof_value>sizeof(element *)?sizeof_value:sizeof(element *);
	this = mem_alloc(sizeof(*this)+nb_elements*sizeof_element);
	if (!this) return 0;
	this->first_memzone.next = NULL;
	this->first_memzone.nb_elements = nb_elements;
	this->first_memzone.nb_used_elements = 0;
	this->nb_used_elements = 0;
	this->sizeof_value = sizeof_value;
	this->sizeof_element = sizeof_element;
	empty_chain(this, &this->first_memzone);
	return this;
}

void cntShelf_del(cntShelf *this) {
	assert(this);
	all_memzone_del(this);
	mem_unregister(this);
}

void cntShelf_clear(cntShelf *this) {
	assert(this);
	all_memzone_del(this);
	empty_chain(this, &this->first_memzone);
	this->first_memzone.nb_used_elements = 0;
	this->nb_used_elements = 0;
}

void *cntShelf_alloc(cntShelf *this) {
	element *ptr;
	memzone *mz;
	assert(this);
	for (mz=&this->first_memzone; mz->next && !mz->first_free; mz=mz->next) ;
	if (!mz->first_free) {
		/* get a new memzone */
		unsigned nb_elements = ((mz->nb_elements+1)*3)>>1;
		memzone *this_mz = mem_alloc(sizeof(*this_mz)+this->sizeof_element*nb_elements);
		if (!this_mz) {
			log_fatal("Cannot get a new memzone");
		}
		this_mz->next = 0;
		this_mz->nb_elements = nb_elements;
		this_mz->nb_used_elements = 0;
		mz->next = this_mz;
		empty_chain(this, this_mz);
		mz = this_mz;
	}
	ptr = mz->first_free;
	mz->first_free = ptr->next_free;
	if (ptr == this->each_next_free) this->each_next_free = ptr->next_free;
	mz->nb_used_elements++;
	this->nb_used_elements++;
	return ptr;
}

void *cntShelf_put(cntShelf *this, void *value) {
	void *ptr;
	assert(this && value);
	ptr = cntShelf_alloc(this);
	memmove(ptr, value, this->sizeof_value);
	return ptr;
}

int cntShelf_free(cntShelf *this, void *value_) {
	element *value = (element *)value_;
	element *ptr, **previous;
	memzone *mz, *mz_previous;
	assert(this && value);
	assert(this->nb_used_elements>0);
	if (this->each_last == value) {	// If the pointed element is the current element, get the next one right now ; otherwise behaviour is unpredictable
		if (!this->each_next) {
			this->each_next = cntShelf_each(this);
		}
	} else if (this->each_next == value) {
		this->each_next = NULL;
	}
	mz = &this->first_memzone;
	mz_previous = 0;
	do {
		if (value>=mz->data && value<(element *)((char *)mz->data + this->sizeof_element*mz->nb_elements)) {
			break;
		}
		mz_previous = mz;
		mz = mz->next;
	} while (mz);
	if (!mz) return 0;
	assert(mz->nb_used_elements>0);
	if (value > mz->first_free) {
		for (previous=&mz->first_free, ptr=mz->first_free; ptr && ptr<value; previous=&ptr->next_free, ptr=ptr->next_free) ;
		if (ptr == value) return 0;
		*previous = value;
		value->next_free = ptr;
		if (this->each_mz == mz) {
			element *each_ptr = this->each_last;
			if (this->each_next) each_ptr = this->each_next;
			if (each_ptr < value && (!this->each_next_free || this->each_next_free>value)) {
				this->each_next_free = value;
			}
		}
	} else {
		if (value == mz->first_free) return 0;
		value->next_free = mz->first_free;
		if (this->each_next_free == mz->first_free && (
			(this->each_next && this->each_next < value) ||
			(this->each_last < value))
		) {
			this->each_next_free = value;
		}
		mz->first_free = value;
	}
	this->nb_used_elements--;
	mz->nb_used_elements--;
	if (0==mz->nb_used_elements && mz_previous) {
		mz_previous->next = mz->next;
		mem_unregister(mz);
	}
	return 1;
}

unsigned cntShelf_size(cntShelf *this) {
	return this->nb_used_elements;
}

void cntShelf_reset(cntShelf *this) {
	assert(this);
	this->each_mz = &this->first_memzone;
	this->each_next_free = this->first_memzone.first_free;
	this->each_last = NULL;
	this->each_next = NULL;
}

void *cntShelf_each(cntShelf *this) {
	element *ptr;
	if (this->each_next) {
		this->each_last = this->each_next;
		this->each_next = NULL;
		return this->each_last;
	}
	if (!this->each_mz) return NULL;
	if (!this->each_last) {
		ptr = this->each_mz->data;
	} else {
		ptr = (element *)((char *)this->each_last + this->sizeof_element);
	}
again:
	assert(!this->each_next_free || ptr <= this->each_next_free);
	while (ptr == this->each_next_free) {
		this->each_next_free = this->each_next_free->next_free;
		ptr = (element *)((char *)ptr + this->sizeof_element);
	}
	if (ptr == (element *)((char *)this->each_mz->data + this->each_mz->nb_elements*this->sizeof_element)) {
		this->each_mz = this->each_mz->next;
		if (!this->each_mz) return NULL;
		this->each_next_free = this->each_mz->first_free;
		ptr = this->each_mz->data;
		goto again;
	}
	this->each_last = ptr;
	return ptr;
}

int cntShelf_islast(cntShelf *this) {
	if (this->each_next) return 0;
	this->each_next = cntShelf_each(this);
	return this->each_next==0;
}

// vi:ts=3:sw=3
