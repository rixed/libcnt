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
#include "libcnt/mem.h"
#include "libcnt/shelf.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	if (!mem_init(1024)) {
		printf("Cannot mem_init\n");
		return EXIT_FAILURE;
	}
	cntShelf *shelf = cntShelf_new(1, 10);
	if (!shelf) return EXIT_FAILURE;
	cntShelf_reset(shelf);
	void *ptr = cntShelf_each(shelf);
	if (ptr!=NULL) return EXIT_FAILURE;
	cntShelf_del(shelf);
	mem_end();
	return EXIT_SUCCESS;
}

// vi:ts=3:sw=3
