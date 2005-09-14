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
#include "libcnt/hash.h"
#include "libcnt/shelf.h"
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

typedef struct cnth_element {
	cntHashkey key;
	struct cnth_element *next;	/* 0 if last */
	char value[];
} cnth_element;

struct s_cntHash {
	cntShelf *shelf;
	unsigned sizeof_value;
	unsigned sizeof_binkey;
	unsigned nb_lines;
	char keytype;
	cnth_element *lines[];
};

cntHash *cntHash_new(unsigned sizeof_value, unsigned total, unsigned per_line, enum cntHash_keysize keytype, unsigned sizeof_binkey) {
	unsigned nb_lines;
	cntHash *this;
	assert(total && per_line);
	nb_lines = 1 + total/per_line;
	this = mem_alloc(sizeof(*this)+nb_lines*sizeof(*this->lines));
	if (!this) return NULL;
	this->nb_lines = nb_lines;
	this->sizeof_value = sizeof_value;
	this->shelf = cntShelf_new(sizeof_value+sizeof(cnth_element), total);
	if (!this->shelf) {
		mem_unregister(this);
		return NULL;
	}
	this->keytype = keytype;
	this->sizeof_binkey = sizeof_binkey;
	for (unsigned i=0; i<nb_lines; i++) {
		this->lines[i] = NULL;
	}
	return this;
}

void cntHash_del(cntHash *this) {
	assert(this);
	cntShelf_del(this->shelf), 1;
	mem_unregister(this);
}

void cntHash_clear(cntHash *this) {
	cntShelf_clear(this->shelf);
	unsigned i;
	for (i=0; i<this->nb_lines; i++) {
		this->lines[i] = NULL;
	}
}

static unsigned hash_func(cntHash *this, cntHashkey key) {
	unsigned index = 1;
	unsigned c;
	if (this->keytype == cntHash_STRKEYS) {
		const char *str = key.str;
		c = 0;
		while (str[c]!='\0') {	/* we could limit c to, say, 10 chars, but if the string is a full pathname, it wont be smart. In the opposite direction, same problem */
			index = 31*index + str[c++];
		}
	} else if (this->keytype == cntHash_BINKEYS) {
		unsigned char *bin = key.bin;
		c = 0;
		while (c < this->sizeof_binkey) {
			index = 31*index + bin[c++];
		}
	} else {	// INTKEYS or PTRKEYS
		intptr_t k;
		if (this->keytype == cntHash_INTKEYS) k = key.i;
		else k = (intptr_t)key.ptr;
		for (c=0; c<sizeof(k); c++) {
			index = 31*index + (k&0xFF);
			k >>= 8;
		}
	}
	index %= this->nb_lines;
	return index;
}

static int keys_match(cntHash *this, cntHashkey k1, cntHashkey k2) {
	if (this->keytype == cntHash_STRKEYS) {
		return 0 == strcmp(k1.str, k2.str);
	} else if (this->keytype == cntHash_BINKEYS) {
		return 0 == memcmp(k1.bin, k2.bin, this->sizeof_binkey);
	} else if (this->keytype == cntHash_PTRKEYS) {
		return k1.ptr == k2.ptr;
	} else {
		return k1.i == k2.i;
	}
}

void *cntHash_put(cntHash *this, cntHashkey key, void const *value) {
	assert(this);
	unsigned line = hash_func(this, key);
	cnth_element **eptr_location;
	cnth_element *eptr, *old_next;
	if (this->keytype == cntHash_STRKEYS) {
		/* register the new key */
		//mem_register(key.str);
	}
	eptr_location = &this->lines[line];
	while ( (eptr = *eptr_location) !=0) {
		if (keys_match(this, eptr->key, key)) {	/* update */
			if (value) memmove(eptr->value, value, this->sizeof_value);
			if (this->keytype == cntHash_STRKEYS) {
				/* unregister the removed key */
				//mem_unregister(eptr->key.str);
			}
			eptr->key = key;
			return eptr->value;
		}
		eptr_location = &eptr->next;
	}
	/* insert */
	eptr = cntShelf_alloc(this->shelf);
	old_next = *eptr_location;
	*eptr_location = eptr;
	eptr->next = old_next;
	eptr->key = key;
	if (value) memmove(eptr->value, value, this->sizeof_value);
	return eptr->value;
}

void *cntHash_get(cntHash *this, const cntHashkey key) {
	assert(this);
	unsigned line = hash_func(this, key);
	cnth_element *eptr = this->lines[line];
	while (eptr) {
		if (keys_match(this, eptr->key, key)) {
			return eptr->value;
		}
		eptr = eptr->next;
	}
	/* asking for an inexistent key */
	return NULL;
}

int cntHash_remove(cntHash *this, const cntHashkey key) {
	assert(this);
	unsigned line = hash_func(this, key);
	cnth_element *eptr = this->lines[line];
	cnth_element *old_eptr = 0;
	while (eptr) {
		if (keys_match(this, eptr->key, key)) {
			if (this->keytype == cntHash_STRKEYS) {
				/* unregister the removed key */
				//mem_unregister(eptr->key.str);
			}
			if (old_eptr)
				old_eptr->next = eptr->next;
			else
				this->lines[line] = eptr->next;
			cntShelf_free(this->shelf, eptr);
			return 1;
		}
		old_eptr = eptr;
		eptr = eptr->next;
	}
	return 0;
}

int cntHash_remove_by_address(cntHash *this, void *addr) {
	assert(this && addr);
	cnth_element *rem_eptr = (cnth_element *)((char *)addr - offsetof(cnth_element, value));
	unsigned line = hash_func(this, rem_eptr->key);
	cnth_element *eptr = this->lines[line];
	if (!eptr) return 0;
	cnth_element *old_eptr = 0;
	while (eptr) {
		if (eptr == rem_eptr) {
			if (this->keytype == cntHash_STRKEYS) {
				/* unregister the removed key */
				//mem_unregister(eptr->key.str);
			}
			if (old_eptr)
				old_eptr->next = eptr->next;
			else
				this->lines[line] = eptr->next;
			cntShelf_free(this->shelf, eptr);
			return 1;
		}
		old_eptr = eptr;
		eptr = eptr->next;
	}
	return 0;
}

unsigned cntHash_size(cntHash *this) {
	assert(this);
	return cntShelf_size(this->shelf);
}

void cntHash_reset(cntHash *this) {
	assert(this);
	cntShelf_reset(this->shelf);
}

int cntHash_each(cntHash *this, cntHashkey *key, void **value) {
	assert(this);
	cnth_element *eptr;
	eptr = cntShelf_each(this->shelf);
	if (eptr) {
		if (key) *key = eptr->key;
		if (value) *value = &eptr->value;
		return 1;
	}
	return 0;
}

int cntHash_islast(cntHash *this) {
	assert(this);
	return cntShelf_islast(this->shelf);
}

void cntHash_move_key(cntHash *this, void *addr, const cntHashkey new_key) {
	assert(this);
	assert(this->keytype==cntHash_BINKEYS || this->keytype==cntHash_PTRKEYS);
	cnth_element *eptr = (cnth_element *)((char *)addr - offsetof(cnth_element, value));
	assert(keys_match(this, eptr->key, new_key));
	eptr->key = new_key;
}

// vi:ts=3:sw=3
