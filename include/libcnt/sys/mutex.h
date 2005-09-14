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
#ifndef MUTEX_H_040713
#define MUTEX_H_040713
#include <libcnt/sys/mutexi.h>

typedef struct s_sys_mutex sys_mutex;

sys_mutex *sys_mutex_construct(sys_mutex *this);
static inline sys_mutex *sys_mutex_new(void) { return sys_mutex_construct(NULL); }
int sys_mutex_destruct(sys_mutex *this);
void sys_mutex_del(sys_mutex *this);
int sys_mutex_blockget(sys_mutex *this);
int sys_mutex_nonblockget(sys_mutex *this);
int sys_mutex_release(sys_mutex *this);

#endif
// vi:ts=3:sw=3
