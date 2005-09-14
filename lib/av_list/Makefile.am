if DEBUG
AM_CFLAGS = -I $(top_srcdir)/include -fstrict-aliasing -D_GNU_SOURCE --std=c99 -Di386 -Wall -W -Wno-unused-parameter -Wno-uninitialized -pedantic -O0 -g
else
AM_CFLAGS = -I $(top_srcdir)/include -fstrict-aliasing -D_GNU_SOURCE --std=c99 -Di386 -Wall -W -Wno-unused-parameter -Wno-uninitialized -pedantic -DNDEBUG
endif

AM_LD_FLAGS = -Wl,--warn-common -Wl,--warn-common

noinst_LTLIBRARIES = libavlist.la
libavlist_la_SOURCES = av_list.c av_call.S

man_MANS = av_list.3

EXTRA_DIST = $(man_MANS)

