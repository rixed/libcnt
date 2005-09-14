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
#include "config.h"
#include "libcnt/vec.h"
#include <stdlib.h>
#include <stdio.h>
#include <libcnt/cnt.h>

int main(void) {
	int ret = EXIT_FAILURE;
	if (!cnt_init(1024, LOG_DEBUG)) return ret;
	atexit(cnt_end);
	{
#		define NB_VECS 10000
		Vec **vecs = mem_alloc(sizeof(*vecs)*NB_VECS);
		unsigned i;
		assert(vecs);
		for (i=0; i<NB_VECS; i++) {
			Vec *v_tmp = Vec_new(1.,1.,1.);
			vecs[i] = Vec_new(1.,2.,3.);
			assert(v_tmp && vecs[i]);
			Vec_del(v_tmp);
		}
		for (i=0; i<NB_VECS; i++) {
			Vec_del(vecs[i]);
		}
		mem_unregister(vecs);
	}
	ret = EXIT_SUCCESS;
	return ret;
}
// vi:ts=3:sw=3
