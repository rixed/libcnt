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
#ifndef MATRIX_H_051008
#define MATRIX_H_041008

#include <stdbool.h>
#include "libcnt/vec.h"

typedef struct {
	double c[4 /* columns */][4 /* lines */];
} Matrix;
extern const Matrix matrix_identity;

static inline void Matrix_construct(Matrix *this, double const (*c)[4]) { *this = *(Matrix const *)c; }
static inline void Matrix_destruct(Matrix const *this) {}
static inline double Matrix_coord(Matrix const *this, unsigned col, unsigned lin) { return this->c[col][lin]; }
static inline void Matrix_coord_set(Matrix *this, unsigned col, unsigned lin, double value) { this->c[col][lin] = value; }

void Matrix_mul(Matrix *dest, Matrix const *m1, Matrix const *m2);
void Matrix_mulT(Matrix *dest, Matrix const *m1, Matrix const *m2);
void Matrix_Tmul(Matrix *dest, Matrix const *m1, Matrix const *m2);

void Matrix_mul_vec(Vec *dest, Matrix const *m, Vec const *v);
void Matrix_mulT_vec(Vec *dest, Matrix const *m, Vec const *v);

#endif
