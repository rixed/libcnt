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
#ifndef ENV_LOG
#define ENV_LOG

#include <stdlib.h>

/* must be called at first. */

typedef enum {
	LOG_MUSTSEE=0,
	LOG_IMPORTANT,
	LOG_OPTIONAL,
	LOG_DEBUG
} log_warn_level;

int log_init(log_warn_level);
/* must be called at the end */
void log_end(void);
/* for a fatal error (exit with error code = 1) */
void log_fatal_d(char const *, size_t, const char *, ...);
#define log_fatal(...) log_fatal_d(__FILE__, __LINE__, __VA_ARGS__)
/* output a warning */
void log_warning_d(char const *, size_t, log_warn_level, const char *, ...);
#define log_warning(...) log_warning_d(__FILE__, __LINE__, __VA_ARGS__)

#endif
// vi:ts=3:sw=3
