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
/** \file libcnt/list.h
 * Functions to create and manipulate a list of elements.
 */

#ifndef CNT_LIST_H
#define CNT_LIST_H

#include <libcnt/mem.h>

typedef struct cntList cntList;

/** Allocate + Construct.
 * \param sizeof_value the size of values that will be pushed on the list
 * \param nb_approx_elements hint for the allocation
 * \return the list, or NULL if error
 */
cntList *cntList_new(unsigned sizeof_value, unsigned nb_approx_elements);
/** Constructor.
 * \param this the list
 * \param sizeof_value the size of values that will be pushed on the list
 * \param nb_approx_elements hint for the allocation
 * \return 1 if OK or 0 for error
 */
int cntList_construct(cntList *this, unsigned sizeof_value, unsigned nb_approx_elements);
/** Destruct + Free.
 * \param this the list
 */
void cntList_del(cntList *this);
/** Destructor.
 * \param this the list
 */
int cntList_destruct(cntList *this);
/** Add a value at the end of the list.
 * \param this the list
 * \param value a pointer to the value that's going to be copied onto the list (its type should be pointer to the type of value)
 * \return address of value on the list
 */
void *cntList_push(cntList *this, const void *value);
/** Remove a value from the end of the list.
 * \param this the list
 * \param dest a pointer to the destination where the value should be copied before removed. If NULL, no copy is done.
 */
void cntList_pop(cntList *this, void *dest);
/** Clear the list.
 * \param this the list
 */
void cntList_clear(cntList *this);
/** Get the number of elements in the list.
 * \param this the list
 * \return size
 */
unsigned cntList_size(const cntList *this);
/** Get an element of the list.
 * \param this the list
 * \param index order of the element (0 <= index < size)
 * \return a pointer to the element in the list
 */
void *cntList_get(cntList *this, unsigned index);
/** Set an element of the list.
 * \param this the list
 * \param index order of the element to reset (0 <= index < size)
 * \param value a pointer to the value that must be copied at this location
 * \return NULL if error, address of the element on the list otherwise
 */
void *cntList_set(cntList *this, unsigned index, const void *value);
/** Insert an element into the list.
 * If value is NULL, no value is inserted, space is only allocated and adress returned.
 * \param this the list
 * \param index order before wich the element must be inserted (0 <= index < size)
 * \param value a pointer to the value to be copied into the list
 * \return NULL if error, address of the element on the list otherwise
 */
void *cntList_insert(cntList *this, unsigned index, const void *value);
/** Alloc after given location.
 * It can be usefull to allocate space on the list just after an element known by address.
 * \param this the list
 * \param element pointer to an element of the list.
 * \return the allocated space, or NULL if error
 */
void *cntList_alloc_after_address(cntList *this, void *element);
/** Alloc before given location.
 * It can be usefull to allocate space on the list just before an element known by address.
 * \param this the list
 * \param element pointer to an element of the list.
 * \return the allocated space, or NULL if error
 */
void *cntList_alloc_before_address(cntList *this, void *element);
/** Remove an arbitrary element.
 * \param this the list
 * \param index order of the element to be removed (0 <= index < size)
 */
void cntList_remove(cntList *this, unsigned index);
/** Remove an arbitrary element by address.
 * \param this the list
 * \param element pointer to the element to be removed
 */
void cntList_remove_by_address(cntList *this, void *element);
/** Remove by value.
 * The first element of the list matching the given value will be removed.
 * \param this the list
 * \param value a pointer to the value to compare to
 * \return 0 if no element matching the value was found, 1 otherwise
 */
int cntList_remove_by_value(cntList *this, void *value);
/** Get size of values.
 * In case you forget, this gives you the size of elements.
 * \param this the list
 * \return size of element.
 */
unsigned cntList_sizeof_value(const cntList *this);
/** Get next element.
 * Assuming element is a correct element from a list, return the next element.
 * \param element a pointer to an element of a list
 * \return a pointer to the next element of this list
 * \sa cntList_prev_by_address
 */
void *cntList_next_by_address(const void *element);
/** Get previous element.
 * Assuming element is a correct element from a list, return the previous element.
 * \param element a pointer to an element of a list
 * \return a pointer to the next element of this list
 * \sa cntList_next_by_address
 */
void *cntList_prev_by_address(const void *element);
/** Promote an element of the list.
 * The list will be relinked so that src just precedes dest.
 * \param this the list
 * \param dest a pointer to the destination element into the list
 * \param src a pointer to the source element into the list
 */
void cntList_relink_by_address(cntList *this, void *dest, void *src);

/* -- */

#include <libcnt/shelf.h>

typedef struct element element;

struct cntList {
	unsigned sizeof_value;
	element *first;	// 0 if none
	element *last;		// 0 if none
	unsigned last_accessed_order;	// order into the list
	element *last_accessed_element;	// of this element (0 means none cached)
	cntShelf *shelf;
};

/* -- */

#endif
// vi:ts=3:sw=3
