dnl Process this file with autoconf to produce a configure script.

# AC_INIT(tree.c)
# dnl Every other copy of the package version number gets its value from here
# AM_INIT_AUTOMAKE(tree, 0.1.0)
 
AC_PREREQ([2.69])
AC_INIT([tree],[0.1.0],[nop@nop.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_CHECK_HEADERS([limits.h locale.h stdint.h stdlib.h string.h sys/ioctl.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDC
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday setlocale strtol])


AC_ARG_ENABLE([threads],
    AS_HELP_STRING([--enable-threads], [Enable running with threads (dangerous!)])
)
AS_IF([test "x$enable_threads" = "xyes"],
    [AC_DEFINE([USE_THREADS], [1], [Use threads.])]
)


AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
