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
#include "libcnt/sys/mutexi.h"
#include "libcnt/mem.h"
#include "libcnt/log.h"
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>

sys_mutex *sys_mutex_construct(sys_mutex *this) {
	if (!this) {
		this = mem_alloc(sizeof(*this));
		if (!this) return NULL;
	}
	pthread_mutex_init(&this->m, NULL);
	return this;
}

int sys_mutex_destruct(sys_mutex *this) {
	assert(this);
	pthread_mutex_destroy(&this->m);
	return 1;
}

void sys_mutex_del(sys_mutex *this) {
	assert(this);
	(void)sys_mutex_destruct(this);
	mem_unregister(this);
}

int sys_mutex_blockget(sys_mutex *this) {
	int ret = pthread_mutex_lock(&this->m);
	if (ret) log_warning(LOG_MUSTSEE, "sys_mutex_blockget : %s", strerror(ret));
	return ret==0;
}

int sys_mutex_nonblockget(sys_mutex *this) {
	int ret = pthread_mutex_trylock(&this->m);
	if (ret && ret!=EBUSY) log_warning(LOG_MUSTSEE, "sys_mutex_nonblockget : %s", strerror(ret));
	return ret==0;
}

int sys_mutex_release(sys_mutex *this) {
	int ret = pthread_mutex_unlock(&this->m);
	if (ret) log_warning(LOG_MUSTSEE, "sys_mutex_release : %s", strerror(ret));
	return ret==0;
}

// vi:ts=3:sw=3
