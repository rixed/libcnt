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
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include "config.h"
#include "libcnt/vec.h"
#include "libcnt/shelf.h"

/* Data Definitions */

const Vec vec_origin = { { 0,0,0 } };
const Vec vec_x = { { 1,0,0 } };
const Vec vec_y = { { 0,1,0 } };
const Vec vec_z = { { 0,0,1 } };

static cntShelf *vecs = NULL;

/* Local Functions */

static void free_vecs_atexit(void) {
	if (vecs) {
		cntShelf_reset(vecs);
		Vec *v;
		while ( (v = cntShelf_each(vecs)) ) {
			Vec_destruct(v);
		}
		cntShelf_del(vecs);
		vecs = NULL;
	}
}

static Vec *Vec_alloc(void) {
	if (!vecs) {
		static _Bool atexit_done = false;
		vecs = cntShelf_new(sizeof(Vec), 100);
		if (!vecs) return NULL;
#ifdef HAVE_ATEXIT
		if (0 != atexit(free_vecs_atexit)) {
			free_vecs_atexit();
			return NULL;
		}
		atexit_done = true;
#endif
	}
	return cntShelf_alloc(vecs);
}

static void Vec_free(Vec *this) {
	assert(this);
	cntShelf_free(vecs, this);
	if (0==cntShelf_size(vecs)) {
		free_vecs_atexit();
	}
}


/* Public Functions */

Vec *Vec_new(double x, double y, double z) {
	Vec *this = Vec_alloc();
	if (!this) return NULL;
	if (!Vec_construct(this, x,y,z)) {
		Vec_free(this);
		return NULL;
	}
	return this;
}

void Vec_del(Vec *this) {
	assert(this);
	(void)Vec_destruct(this);
	Vec_free(this);
}

double Vec_scalar(const Vec *this, const Vec *that) {
	assert(this && that);
	double sum = 0;
	for (unsigned i=0; i<3; i++) {
		sum += this->c[i]*that->c[i];
	}
	return sum;
}

double Vec_norm2(const Vec *this) {
	assert(this);
	return Vec_scalar(this, this);
}

double Vec_norm(const Vec *this) {
	assert(this);
	return sqrt(Vec_norm2(this));
}

void Vec_normalize(Vec *this) {
	assert(this);
	double n = Vec_norm(this);
	if (n != 0) {
		Vec_scale(this, 1/n);
	}
}

void Vec_scale(Vec *this, double ratio) {
	Vec_scale3(this, this, ratio);
}

void Vec_scale3(Vec *this, const Vec *v, double ratio) {
	assert(this && v);
	for (unsigned i=0; i<3; i++) {
		this->c[i] = v->c[i] * ratio;
	}
}

double Vec_dist(const Vec *this, const Vec *that) {
	Vec v;
	(void)Vec_construct(&v, that->c[0] - this->c[0], that->c[1] - this->c[1], that->c[2] - this->c[2]);
	double d = Vec_norm(&v);
	Vec_destruct(&v);
	return d;
}

void Vec_add(Vec *this, const Vec *that) {
	assert(this && that);
	for (unsigned i=0; i<3; i++) {
		this->c[i] += that->c[i];
	}
}

void Vec_sub(Vec *this, const Vec *that) {
	assert(this && that);
	for (unsigned i=0; i<3; i++) {
		this->c[i] -= that->c[i];
	}
}

void Vec_add3(Vec *this, const Vec *v1, const Vec *v2) {
	assert(this && v1 && v2);
	for (unsigned i=0; i<3; i++) {
		this->c[i] = v1->c[i] + v2->c[i];
	}
}

void Vec_sub3(Vec *this, const Vec *v1, const Vec *v2) {
	assert(this && v1 && v2);
	for (unsigned i=0; i<3; i++) {
		this->c[i] = v1->c[i] - v2->c[i];
	}
}

void Vec_add_scale(Vec *this, double ratio, const Vec *that) {
	assert(this && that);
	for (unsigned i=0; i<3; i++) {
		this->c[i] += ratio*that->c[i];
	}
}

void Vec_add_scale3(Vec *this, const Vec *v1, double ratio, const Vec *v2) {
	assert(this && v1 && v2);
	for (unsigned i=0; i<3; i++) {
		this->c[i] = v1->c[i] + ratio*v2->c[i];
	}
}

void Vec_product(Vec *restrict this, const Vec *v1, const Vec *v2) {
	assert(this && v1 && v2 && this!=v1 && this!=v2);
	for (unsigned i=0; i<3; i++) {
		this->c[i] = v1->c[(i+1)%3] * v2->c[(i+2)%3] - v1->c[(i+2)%3] * v2->c[(i+1)%3];
	}
}


// vi:ts=3:sw=3
