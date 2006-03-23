#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT()
AM_INIT_AUTOMAKE(libcnt, 2.1)

BACK_COMPAT=1
VERSION_INFO=`echo $VERSION | sed -e 's/\./:/'`:$BACK_COMPAT
AC_SUBST(VERSION_INFO)

AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER(config.h)

AM_CONDITIONAL(DEBUG, test "$enable_debug" = yes)

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CC
AM_PROG_AS
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
#AM_PROG_AS

# Checks for libraries.
AC_CHECK_LIB([m], [sin])
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([efence], [malloc])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h string.h arpa/inet.h netinet/in.h stdint.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_C_VOLATILE

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([gettimeofday memmove memset select socket strerror atexit])

AC_CONFIG_FILES([Makefile
                 checks/Makefile
                 lib/Makefile
                 lib/av_list/Makefile
                 lib/cnt/Makefile
                 lib/cnt/sys/Makefile
                 include/Makefile
                 include/libcnt/Makefile
                 include/libcnt/sys/Makefile])
AC_OUTPUT
