AM_CFLAGS = -Wall -W -I $(top_srcdir)/include --std=c99 -Di386 -O0 -g
AM_LD_FLAGS = -Wl,--warn-common

check_PROGRAMS = avlist_check shelf_check vec_check tree_check

shelf_check_SOURCES = shelf_check.c
shelf_check_LDADD = ../lib/libcnt.la -lm -lpthread
shelf_check_LDFLAGS = -all-static

tree_check_SOURCES = tree_check.c
tree_check_LDADD = ../lib/libcnt.la -lm -lpthread
tree_check_LDFLAGS = -all-static

vec_check_SOURCES = vec_check.c
vec_check_LDADD = ../lib/libcnt.la -lm -lpthread
vec_check_LDFLAGS = -all-static

avlist_check_SOURCES = avlist_check.c
avlist_check_LDADD = ../lib/libcnt.la -lpthread
avlist_check_LDFLAGS = -all-static

TESTS = avlist_check shelf_check tree_check vec_check

