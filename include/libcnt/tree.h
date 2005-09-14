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
#ifndef CNT_TREE_H
#define CNT_TREE_H

typedef struct cntTree cntTree;

int cntTree_construct(cntTree *this, unsigned sizeof_value, unsigned nb_nodes, void *root_value);
int cntTree_destruct(cntTree *this);
cntTree *cntTree_new(unsigned sizeof_value, unsigned nb_nodes, void *root_value);
void cntTree_del(cntTree *);
// If father = NULL, set the root value. value can be NULL
void *cntTree_add_node(cntTree *this, void *father, void *value);	// returns the address of the added value
unsigned cntTree_size(cntTree *this);
unsigned cntTree_nb_sons(void *node_val);
void *cntTree_get_son(const void *node_val, unsigned order);
static void *cntTree_root(cntTree *this);
static void cntTree_reset(cntTree *this);
static void *cntTree_each(cntTree *this);

/* --- */
#include <assert.h>
#include <stddef.h>
#include <libcnt/list.h>

struct cntTree {
	cntList *list;
	unsigned sizeof_value;
	unsigned each_order;
};

typedef struct cntNode cntNode;
struct cntNode {
	cntNode *father;
	cntNode *brother_l, *brother_r;
	char value[];
};

static inline void *cntTree_root(cntTree *this) {
	assert(this);
	return ((cntNode*)(cntList_get(this->list, 0)))->value;
}
static inline void cntTree_reset(cntTree *this) {
	assert(this);
	this->each_order = 0;
}
static inline void *cntTree_each(cntTree *this) {
	assert(this);
	if (this->each_order < cntList_size(this->list)) {
		return ((cntNode *)cntList_get(this->list, this->each_order++))->value;
	} else return NULL;
}

#endif
// vi:ts=3:sw=3
