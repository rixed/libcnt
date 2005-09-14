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
#ifndef VEC_I_H_041123
#define VEC_I_H_041123

#include "vec.h"

struct Vec {
	double c[3];
};

#include <assert.h>
static inline int Vec_construct(Vec *this, double x, double y, double z) {
	assert(this);
	this->c[0] = x;
	this->c[1] = y;
	this->c[2] = z;
	return 1;
}

static inline void Vec_destruct(Vec *this) { assert(this); }

static inline double Vec_coord(const Vec *this, unsigned i) { assert(this && i<3); return this->c[i]; }
static inline void Vec_coord_set(Vec *this, unsigned i, double value) { assert(this && i<3); this->c[i] = value; }

#endif
// vi:ts=3:sw=3
