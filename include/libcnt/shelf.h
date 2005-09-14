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
/** \file shelf.h
 * A Shelf is a set of elements of common size.
 * Elements are inordered.
 * This is the lowest level countainer, as it is used by
 * all other countainers.
 */

#ifndef CNT_SHELF_H
#define CNT_SHELF_H

#include <stdlib.h>

typedef struct cntShelf cntShelf;

/** Get a new shelf.
 * \param sizeof_value size of each elements
 * \param nb_elements approx number of elements (used to allocate first chunk of memory)
 * \return a pointer to the new shelf, or NULL if error
 */
cntShelf *cntShelf_new(unsigned sizeof_value, unsigned nb_elements);

/** Deletes a shelf.
 * \param this the shelf
 */
void cntShelf_del(cntShelf *this);

/** Copy given datas into the shelf.
 * As seen by the user, this function never fails.
 * \param this the shelf
 * \param data pointer to the data that must be copied into the shelf
 * \return pointer to the data inside the shelf
 */
void *cntShelf_put(cntShelf *this, void *data);

/** Allocate an element.
 * As seen by the user, this function never fails
 * \param this the shelf
 * \return pointer to the allocated element
 */
void *cntShelf_alloc(cntShelf *this);

/** Free an element.
 * Its OK to delete the last element returned by cntShelf_each.
 *	\param this the shelf
 *	\param location pointer to the element to release
 *	\return nbr of removed values (0 or 1)
 *	\sa cntShelf_alloc
 */
int cntShelf_free(cntShelf *this, void *location);

/** Get number of elements.
 * \param this the shelf
 * \return number of elements currently countained in the shelf
 */
unsigned cntShelf_size(cntShelf *this);

/** Empty the shelf.
 * \param this the shelf
 */
void cntShelf_clear(cntShelf *this);

/** Reset shelf cursor to the begining.
 * Beware that using shelf cursor is not thread safe.
 * \param this the shelf
 * \sa cntShelf_each,cntShelf_islast
 */
void cntShelf_reset(cntShelf *this);

/** Get next element.
 * Beware that using shelf cursor is not thread safe.
 * \param this the shelf
 * \return pointer to next element, or NULL if there is no more element
 * \sa cntShelf_clear, cntShelf_islast
 * \note New elements added while reading the cursor may or may not be
 *       sean by the cursor. Removing elements does not affect the cursor.
 */
void *cntShelf_each(cntShelf *this);

/** Tell wether last returned element is the last.
 * Beware that using shelf cursor is not thread safe.
 * \param this the shelf
 * \return 1 if cursor is at the end, 0 if there are still elements further away
 * \sa cntShelf_clear,cntShelf_each
 */
int cntShelf_islast(cntShelf *this);

#endif
// vi:ts=3:sw=3
