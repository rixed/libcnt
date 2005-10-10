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
#include "config.h"
#include "libcnt/matrix.h"

/*
 * Data Definitions
 */

const Matrix matrix_identity = {
	.c = {
		{ 1.,0.,0.,0. }, 
		{ 0.,1.,0.,0. },
		{ 0.,0.,1.,0. },
		{ 0.,0.,0.,1. }
	}
};

/*
 * Private Functions
 */

/*
 * Public Functions
 */

void Matrix_mul(Matrix *const dest, Matrix const *const m1, Matrix const *const m2) {
	assert(dest && m1 && m2);
	for (unsigned col=0; col<4; col++) {
		for (unsigned lin=0; lin<4; lin++) {
			dest->c[col][lin] = 0.;
			for (unsigned k=0; k<4; k++) {
				dest->c[col][lin] += m1->c[k][lin]*m2->c[col][k];
			}
		}
	}
}

void Matrix_mulT(Matrix *const dest, Matrix const *const m1, Matrix const *const m2) {	// m1 * transp(m2)
	assert(dest && m1 && m2);
	for (unsigned col=0; col<4; col++) {
		for (unsigned lin=0; lin<4; lin++) {
			dest->c[col][lin] = 0.;
			for (unsigned k=0; k<4; k++) {
				dest->c[col][lin] += m1->c[k][lin]*m2->c[k][col];
			}
		}
	}
}

void Matrix_Tmul(Matrix *const dest, Matrix const *const m1, Matrix const *const m2) {	// transp(m1) * m2
	assert(dest && m1 && m2);
	for (unsigned col=0; col<4; col++) {
		for (unsigned lin=0; lin<4; lin++) {
			dest->c[col][lin] = 0.;
			for (unsigned k=0; k<4; k++) {
				dest->c[col][lin] += m1->c[lin][k]*m2->c[col][k];
			}
		}
	}
}

void Matrix_mul_vec(Vec *const dest, Matrix const *const m, Vec const *const v) {
	for (unsigned lin=0; lin<3; lin++) {
		double d = m->c[3][lin];
		for (unsigned k=0; k<3; k++) {
			d += m->c[k][lin] * Vec_coord(v, k);
		}
		Vec_coord_set(dest, lin, d);
	}
}

void Matrix_mulT_vec(Vec *const dest, Matrix const *const m, Vec const *const v) {
	for (unsigned lin=0; lin<3; lin++) {
		double d = m->c[lin][3];
		for (unsigned k=0; k<3; k++) {
			d += m->c[lin][k] * Vec_coord(v, k);
		}
		Vec_coord_set(dest, lin, d);
	}
}

