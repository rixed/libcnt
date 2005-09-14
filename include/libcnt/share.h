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
#ifndef ENV_SHARE_H
#define ENV_SHARE_H
#include <assert.h>

/* base object to be herited by shareable objects */
/* sample : Obj1 references Obj2, which is shareable.
 * destructor of Obj1 calls the destructor of Obj2.
 * destructor of Obj2 starts by calling share_destruct on its share.
 * iif true, Obj2 proceed with its autodestruction.
 */

typedef unsigned cntShare;

inline static void cntShare_construct(cntShare *this) { *this=0; }
inline static void cntShare_link(cntShare *this) { (*this)++; }
inline static void cntShare_unlink(cntShare *this) { assert(0<*this); (*this)--; }
inline static int cntShare_destruct(cntShare *this) { return 0==*this; }

#endif
// vi:ts=3:sw=3
