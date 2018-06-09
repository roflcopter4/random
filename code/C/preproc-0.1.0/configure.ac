dnl Process this file with autoconf to produce a configure script.

dnl AC_INIT(tree.c)
dnl dnl Every other copy of the package version number gets its value from here
dnl AM_INIT_AUTOMAKE(tree, 0.1.0)
 
AC_PREREQ([2.69])
AC_INIT([preproc],[1.0.1],[leason@ualberta.ca])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)
AC_SUBST(VERSION)
AC_CANONICAL_HOST
AC_GNU_SOURCE

CFLAGS="-O3 -march=native -Wall ${CFLAGS}"

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# AC_CHECK_LIB([bsd], [strlcpy])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([realpath strtol reallocarray])



dnl AC_ARG_ENABLE([threads],
dnl     AS_HELP_STRING([--enable-threads], [Enable running with threads (dangerous!)])
dnl )
dnl AS_IF([test "x$enable_threads" = "xyes"],
dnl     [AC_DEFINE([USE_THREADS], [1], [Use threads.])]
dnl )

# AM_CONDITIONAL([NEED_BSD_FUNCS], [test "x$ac_cv_func_strlcpy" = "xno"])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
