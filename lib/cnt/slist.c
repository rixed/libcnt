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
#include "libcnt/slist.h"
#include "libcnt/list.h"
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include <assert.h>
#include <string.h>

/* heavy use of cntList */

struct cntSList {
	cntList *list;
	int (*comp)(void *, void *);
};

cntSList *cntSList_new(unsigned sizeof_value, unsigned total, int (*comp)(void *, void *))
{
	cntSList *this;
	this = mem_alloc(sizeof(*this));
	if (!this) return 0;
	this->list = cntList_new(sizeof_value, total);
	if (!this->list) {
		mem_unregister(this);
		return 0;
	}
	this->comp = comp;
	return this;
}

void cntSList_del(cntSList *this)
{
	cntList_del(this->list);
	mem_unregister(this);
}

void cntSList_clear(cntSList *this)
{
	cntList_clear(this->list);
}

unsigned cntSList_size(cntSList *this)
{
	return cntList_size(this->list);
}

void *cntSList_get(cntSList *this, unsigned order)
{
	return cntList_get(this->list, order);
}

/* to keep ordering, we remove then insert back */
void cntSList_set(cntSList *this, unsigned order, void *value)
{
	cntSList_remove(this, order);
	cntSList_insert(this, value);
}

void cntSList_remove(cntSList *this, unsigned order)
{
	cntList_remove(this->list, order);
}

unsigned cntSList_insert(cntSList *this, void *value)
{
	unsigned size = cntList_size(this->list);
	unsigned index;
	for (index=0; index<size; index++) {
		int c = this->comp(cntList_get(this->list, index), value);
		if (c>=0) break;
	}
	cntList_insert(this->list, index, value);
	return index;
}

void cntSList_remove_by_address(cntSList *this, void *here) {
	assert(this);
	cntList_remove_by_address(this->list, here);
}

int cntSList_remove_by_value(cntSList *this, void *value) {
	unsigned s, size;
	assert(this);
	size = cntList_size(this->list);
	for (s=0; s<size; s++) {
		void *val = cntList_get(this->list, s);
		if (this->comp(val, value)>0) return 0;
		if (0==memcmp(val, value, cntList_sizeof_value(this->list))) {
			cntList_remove(this->list, s);
			break;
		}
	}
	return s<size;
}

void *cntSList_next_by_address(const void *v_ptr) {
	return cntList_next_by_address(v_ptr);
}
void *cntSList_prev_by_address(const void *v_ptr) {
	return cntList_prev_by_address(v_ptr);
}

void *cntSList_tmp_alloc(cntSList *this) {
	assert(this);
	return cntList_insert(this->list, cntList_size(this->list), NULL);
}
void cntSList_tmp_free(cntSList *this, void *here) {
	assert(this && here);
	cntList_remove_by_address(this->list, here);
}
unsigned cntSList_tmp_insert(cntSList *this, void *here) {
	assert(this && here);
	unsigned size = cntList_size(this->list);
	unsigned index;
	void *there;
	for (index=0; index<size; index++) {
		there = cntList_get(this->list, index);
		int c = this->comp(there, here);
		if (c>=0) break;
	}
	assert(index<size);
	cntList_relink_by_address(this->list, there, here);
	return index;
}

// vi:ts=3:sw=3
