dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([arse],[0.1.0],[nop@nop.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
#AM_INIT_AUTOMAKE([foreign])
AM_INIT_AUTOMAKE([subdir-objects foreign])

AC_SUBST(VERSION)
ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)
AC_CANONICAL_HOST

AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_MKDIR_P

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([locale.h stdint.h stdlib.h string.h sys/ioctl.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday setlocale strtol])

#AC_CONFIG_FILES([Makefile
                 #src/Makefile])
#AC_CONFIG_FILES([Makefile])
#AC_OUTPUT([Makefile src/Makefile])
AC_OUTPUT([Makefile])
