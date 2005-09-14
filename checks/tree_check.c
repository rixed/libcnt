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
#include <stdlib.h>
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include "libcnt/tree.h"

static int check_tree(void )
{
	int i = 0, *f;
	cntTree *tree;
	log_warning(LOG_DEBUG, "testing tree");
	tree = cntTree_new(sizeof(int), 10, &i);
	if (!tree) return 0;
	for (i=0, f=cntTree_root(tree); i<100; i++) {
		int *a;
		assert(f);
		a = cntTree_add_node(tree, f, &i);
		if (i&3) f = a;	// change le fils
	}
	log_warning(LOG_DEBUG, "OK, deleting the tree");
	cntTree_del(tree);
	return 1;
}

int main(void)
{
	int ret = EXIT_FAILURE;
	log_init(LOG_DEBUG);
	mem_init(5000);
	if (check_tree()) ret = EXIT_SUCCESS;
	mem_end();
	log_end();
	return EXIT_SUCCESS;
}
// vi:ts=3:sw=3
