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
 * Functions to create and manipulate a hash.
 * The key is an union{unsigned;char*}, the value a void*.
 * NOTE: a single hash is not thread proof !
 *
 */

#ifndef CNT_HASH_H_041027
#define CNT_HASH_H_041027

enum cntHash_keysize {
	cntHash_INTKEYS,	/*< keys are implementation specific ints */
	cntHash_STRKEYS,	/*< keys are strings, not unsigned */
	cntHash_BINKEYS,	/*< keys are list of bytes (stored client side) */
	cntHash_PTRKEYS,	/*< keys are addresses */
};

typedef struct s_cntHash cntHash;

typedef union {
	unsigned i;
	void *ptr;
	const char *str;
	void *bin;
} cntHashkey;

/** hash constructors.
 * If you use strings for keys you are responsible for keeping alive the char* pointer for the lifetime of the hash.
 * \param nb_elmnts/collisions should be prime
 */
cntHash *cntHash_new(unsigned sizeof_value, unsigned nb_elmnts, unsigned collisions, enum cntHash_keysize keytype, unsigned sizeof_binkey);
/** to delete a hash */
void cntHash_del(cntHash *this);
/** to delete all values currently stored in te hash */
void cntHash_clear(cntHash *this);
/** to put a new pair of key/value or update the value associated with the key. returns allocated space */
void *cntHash_put(cntHash *this, const cntHashkey, void const *);
/** to get the value associated with a key (in value). return 0 if key does not exists */
void *cntHash_get(cntHash *this, const cntHashkey);
/** remove the value associated to the key */
int cntHash_remove(cntHash *this, const cntHashkey);
int cntHash_remove_by_address(cntHash *this, void *addr);
/** tells how many keys are defined */
unsigned cntHash_size(cntHash *this);
/** reset the read pointer */
void cntHash_reset(cntHash *this);
/** read next pair (key,value). return 0 if there are no more values.
 * It's OK if key or value are NULL
 */
int cntHash_each(cntHash *this, cntHashkey *key, void **value);
/** tells wether this element is the last one */
int cntHash_islast(cntHash *this);
/** For STR or BIN keys, we sometimes want to change the 'location' of the key value,
 * (not the value of course). Copy first the value, then declare the new location with this :
 */
void cntHash_move_key(cntHash *this, void *addr, const cntHashkey new_key);
/* TODO : add a cursor object to scan the hash without semaphore ? */

#endif
// vi:ts=3:sw=3
