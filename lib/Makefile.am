SUBDIRS = cnt av_list

lib_LTLIBRARIES = libcnt.la
libcnt_la_SOURCES = autobug.c
libcnt_la_LIBADD = cnt/libcnt.la cnt/sys/libsys.la av_list/libavlist.la
libcnt_la_LDFLAGS = -version-info @VERSION_INFO@

