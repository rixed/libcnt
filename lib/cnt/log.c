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
#include "libcnt/log.h"
#include "libcnt/sys/thread.h"
#include "libcnt/sys/time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#ifdef _WIN32
#include <windows.h>
#endif

static int min_pid;
static int max_pid;
static unsigned long long first_time;
static log_warn_level warn_level=LOG_IMPORTANT;

int log_init(log_warn_level level) {
	warn_level = level;
	min_pid = max_pid = sys_pid();
	first_time = sys_microtime();
	return 1;
}

void log_end() {}

static void writeA(const char *msg, va_list args) {
	vfprintf(stderr, msg, args);
}

static void writeA2(const char *msg, ...) {
	va_list args;
	va_start(args, msg);
	writeA(msg, args);
	va_end(args);
}

static void writeB(const char *msg) {
	fputs(msg, stderr);
}

static void writeHead(void) {
	// append a header that distinguishes distinct threads
	int pid = sys_pid();
	if (pid < min_pid) min_pid = pid;
	if (pid > max_pid) max_pid = pid;
	if (min_pid != max_pid) {
		writeA2("\e[3%cm%09llu:%d:\e[0m", '1'+pid%9, sys_microtime()-first_time, pid);
	}
}

void log_fatal_d(char const *file, size_t line, const char *msg, ...) {
	va_list args;
	va_start(args, msg);
	writeHead();
	writeA2("FATAL : %s:%u: ", file, line);
	writeA(msg, args);
	writeB("\n");
	va_end(args);
	abort();
}

void log_warning_d(char const *file, size_t line, log_warn_level level, const char *msg, ...) {
	va_list args;
	if (level<=warn_level) {
		va_start(args, msg);
		writeHead();
		writeA2("Warning : %s:%u: ", file, line);
		writeA(msg, args);
		writeB("\n");
		va_end(args);
	}
}

// vi:ts=3:sw=3
