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
#include "libcnt/tree.h"
#include "libcnt/list.h"
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include <assert.h>
#include <string.h>
#include <stddef.h>

int cntTree_construct(cntTree *this, unsigned sizeof_value, unsigned nb_nodes, void *root_value) {
	assert(this);
	cntNode *node;
	assert(sizeof_value>0 && nb_nodes>0);
	this->list = cntList_new(sizeof_value+sizeof(cntNode), nb_nodes);
	if (!this->list) return 0;
	this->sizeof_value = sizeof_value;
	this->each_order = 0;
	node = cntList_insert(this->list, 0, NULL);
	node->father = node->brother_l = node->brother_r = NULL;
	if (root_value) memcpy(node->value, root_value, sizeof_value);
	return 1;
}

cntTree *cntTree_new(unsigned sizeof_value, unsigned nb_nodes, void *root_value) {
	cntTree *this;
	this = mem_alloc(sizeof(*this));
	if (!this) return NULL;
	if (!cntTree_construct(this, sizeof_value, nb_nodes, root_value)) {
		mem_unregister(this);
		return NULL;
	}
	return this;
}

int cntTree_destruct(cntTree *this) {
	assert(this);
	cntList_del(this->list);
	return 1;
}
void cntTree_del(cntTree *this) {
	assert(this);
	(void)cntTree_destruct(this);
	mem_unregister(this);
}

void *cntTree_add_node(cntTree *this, void *father_val, void *value) {
	cntNode *node, *son, *father;
	assert(this);
	if (!father_val) {
		node = (cntNode *)cntList_get(this->list, 0);
	} else {
		father = (cntNode*)((char *)father_val - offsetof(cntNode, value));
		son = cntList_next_by_address(father);
		node = cntList_alloc_after_address(this->list, father);
		if (!node) {
			return NULL;
		}
		node->father = father;
		node->brother_l = NULL;
		if (son && son->father == father) {
			node->brother_r = son;
			son->brother_l = node;
		} else node->brother_r = NULL;
	}
	if (value) memcpy (node->value, value, this->sizeof_value);
	return node->value;
}

unsigned cntTree_size(cntTree *this) {
	return (cntList_size(this->list));
}

unsigned cntTree_nb_sons(void *node_val) {
	cntNode *node, *ptr;
	unsigned i;
	assert(node_val);
	node = (cntNode*)((char*)node_val-offsetof(cntNode, value));
	ptr = cntList_next_by_address(node);
	if (!ptr || ptr->father!=node) return 0;
	for (i=0 ; ptr; ptr=ptr->brother_r) i++;
	return i;
}

void *cntTree_get_son(const void *node_val, unsigned order) {
	cntNode *node, *ptr;
	unsigned i;
	assert(node_val);
	node = (cntNode*)((char*)node_val-offsetof(cntNode, value));
	ptr = cntList_next_by_address(node);
	if (!ptr || ptr->father!=node) return NULL;
	for (i=0 ; ptr && i<order; ptr=ptr->brother_r) i++;
	if (!ptr) return NULL;
	return ptr->value;
}

// vi:ts=3:sw=3
