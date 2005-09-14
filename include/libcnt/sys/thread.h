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
#ifndef THREAD_H_040712
#define THREAD_H_040712
#include <pthread.h>

typedef pthread_t sys_thread;

sys_thread *sys_thread_construct(sys_thread *this, void *(*routine)(void *), void *arg);
/** Wait for thread termination
 * returns the ret value of the thread
 */
void *sys_thread_wait(sys_thread *this);
void sys_thread_destruct(sys_thread *this);
void sys_thread_sleep(unsigned seconds);
int sys_pid(void);

#endif
// vi:ts=3:sw=3
