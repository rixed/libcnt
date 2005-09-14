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
#ifndef VEC_H_041123
#define VEC_H_041123

typedef struct Vec Vec;
extern const Vec vec_origin, vec_x, vec_y, vec_z;

static int Vec_construct(Vec *this, double x, double y, double z);
static void Vec_destruct(Vec *this);
Vec *Vec_new(double x, double y, double z);
void Vec_del(Vec *this);
static double Vec_coord(const Vec *this, unsigned i);
static void Vec_coord_set(Vec *this, unsigned i, double value);
double Vec_scalar(const Vec *this, const Vec *that);
double Vec_norm2(const Vec *this);
double Vec_norm(const Vec *this);
void Vec_normalize(Vec *this);
void Vec_scale(Vec *this, double ratio);
void Vec_scale3(Vec *this, const Vec *v, double ratio);
double Vec_dist(const Vec *this, const Vec *that);
void Vec_add(Vec *this, const Vec *v);
void Vec_sub(Vec *this, const Vec *v);
void Vec_add3(Vec *this, const Vec *v1, const Vec *v2);
void Vec_sub3(Vec *this, const Vec *v1, const Vec *v2);
void Vec_add_scale(Vec *this, double ratio, const Vec *that);
void Vec_add_scale3(Vec *this, const Vec *v1, double ratio, const Vec *v2);
void Vec_product(Vec *restrict this, const Vec *v1, const Vec *v2);

#include "vec_i.h"
#endif
// vi:ts=3:sw=3
