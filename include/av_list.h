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
#ifndef AV_LIST_H
#define AV_LIST_H
#include <stdlib.h>

typedef struct {
	int nb_parameters;
	int nb_max_parameters;
	void *function;
	void *ret_value;	/* alloc at least the size for an int ! */
	size_t ret_size;
	struct av_parameter {
		void *parameter;
		size_t size;
#ifndef USE_EMPTY_ARRAY
	} parameters[];
#else
	} parameters[0];
#endif
} av_list;

extern av_list *av_new(void*, size_t);
extern int av_arg(av_list*, void*, size_t);	/* in the same order than the prototype */
extern void av_delete(av_list*);
extern int av_call(av_list*, void**);	// this one is assembler, implementation specific

#endif
// vi:ts=3:sw=3
// vi:ts=3:sw=3
