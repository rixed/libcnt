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
#include "libcnt/sys/time.h"
#include "libcnt/log.h"
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <errno.h>

unsigned long long sys_microtime(void) {
	static unsigned long first_call_s = 0;
	static unsigned long first_call_us = 0;

	struct timeval tv;
	if (0!=gettimeofday(&tv, NULL)) {
		log_warning(LOG_IMPORTANT, strerror(errno));
		return 0;
	}
	if (!first_call_s) {
		first_call_s = tv.tv_sec;
		first_call_us = tv.tv_usec;
		return 0;
	} else {
		return (tv.tv_sec-first_call_s)*1000000ULL+(tv.tv_usec-first_call_us);
	}
}

// vi:ts=3:sw=3
