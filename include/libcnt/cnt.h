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
#ifndef CNT_CNT_H_041027
#define CNT_CNT_H_041027

#include <libcnt/mem.h>
#include <libcnt/log.h>

static inline int cnt_init(unsigned pool_size, log_warn_level level) {
	if (! mem_init(pool_size)) return 0;
	if (! log_init(level)) {
		mem_end();
		return 0;
	}
	return 1;
}

// suitable for atexit
static inline void cnt_end(void) {
	log_end(); mem_end();
}

#define sizeof_array(x) (sizeof(x)/sizeof(*x))

#endif
// vi:ts=3:sw=3
