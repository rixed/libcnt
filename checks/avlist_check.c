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
#include <stdio.h>
#include "av_list.h"

int foo(char c, unsigned i) {
	printf("hello world! (%c, %u)\n",c,i);
	return 666;
}

void bar(void) {
	printf("world, hello!\n");
}

int main(void) {
	int *ret;	/* must match the return type of foo */
	unsigned i = 33;
	char c = 'x';
	av_list *list = av_new(foo, sizeof(int));
	if (!(
				av_arg(list, &c, sizeof(char)) &&
				av_arg(list, &i, sizeof(unsigned))
		  )) {
		printf("av_arg error\n");
		return EXIT_FAILURE;
	}
	void *ptr;
	if (!list || !av_call(list, &ptr)) {
		printf("error!\n");
		return EXIT_FAILURE;
	}
	ret = ptr;
	printf("ret = %d\n", *ret);
	if (*ret != 666) {
		printf("not 666?\n");
		return EXIT_FAILURE;
	}
	av_delete(list);
	list = av_new(bar, 0);
	if (!list || !av_call(list, &ptr)) {
		printf("error!\n");
		return EXIT_FAILURE;
	}
	av_delete(list);
	printf("OK\n");
	return EXIT_SUCCESS;
}

// vi:ts=3:sw=3
