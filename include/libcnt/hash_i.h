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
#ifndef CNT_HASH_I_H_041128
#define CNT_HASH_I_H_041128

#include <libcnt/hash.h>
#include <libcnt/shelf.h>

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

int cntHash_construct(cntHash *this, unsigned sizeof_value, unsigned nb_elmnts, unsigned collisions, enum cntHash_keysize keytype, unsigned sizeof_binkey);
int cntHash_destruct(cntHash *this);

#endif
// vi:ts=3:sw=3
