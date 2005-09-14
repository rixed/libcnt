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
 * Functions to create and manipulate a sortted list of void*.
 */

#ifndef CNT_SLIST_H
#define CNT_SLIST_H

typedef struct cntSList cntSList;

/* to get a new empty list */
/* size is the total estimated max size of the slist.
 * comp is the comparison function, return -1, 0 or 1 to tell wether its first arg is lower, equal or greater than its second
 */
cntSList *cntSList_new(unsigned sizeof_value, unsigned size, int (*comp)(void *, void *));
/* to delete a slist */
void cntSList_del(cntSList *this);
/* to empty the slist (the slist keeps its current memsize, even if resized) */
void cntSList_clear(cntSList *this);
/* tells the list size */
unsigned cntSList_size(cntSList *this);
/* returns value n (optimised if last accessed value was not far from n) */
void *cntSList_get(cntSList *this, unsigned);
/* reset value at position n (same condition) */
void cntSList_set(cntSList *this, unsigned, void *);
/* remove value at position n (same condition) */
void cntSList_remove(cntSList *this, unsigned);
/* insert value (returns the index where the value was inserted) */
unsigned cntSList_insert(cntSList *this, void *);
void cntSList_remove_by_address(cntSList *this, void *here);
int cntSList_remove_by_value(cntSList *this, void *value);
void *cntSList_next_by_address(const void *v_ptr);
void *cntSList_prev_by_address(const void *v_ptr);

void *cntSList_tmp_alloc(cntSList *this);
void cntSList_tmp_free(cntSList *this, void *here);
unsigned cntSList_tmp_insert(cntSList *this, void *here);

#endif
// vi:ts=3:sw=3
