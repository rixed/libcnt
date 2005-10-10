if DEBUG
AM_CFLAGS = -I $(top_srcdir)/include -fstrict-aliasing -D_GNU_SOURCE -std=c99 -Wall -W -Wno-unused-parameter -Wno-uninitialized -pedantic -O0 -g
else
AM_CFLAGS = -I $(top_srcdir)/include -fstrict-aliasing -D_GNU_SOURCE -std=c99 -Wall -W -Wno-unused-parameter -Wno-uninitialized -pedantic -DNDEBUG
endif
AM_LD_FLAGS = -Wl,--warn-common

noinst_LTLIBRARIES = libcnt.la
libcnt_la_SOURCES = \
	hash.c \
	list.c \
	mem.c \
	shelf.c \
	slist.c \
	stack.c \
	tree.c \
	log.c \
	vec.c \
	matrix.c

SUBDIRS = sys

