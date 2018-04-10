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
AC_CHECK_LIB([m], [pow])
AC_CHECK_LIB([bsd], [strlcat])

# Checks for header files.
AC_CHECK_HEADERS([limits.h locale.h stdint.h stdlib.h string.h sys/ioctl.h sys/time.h bsd/bsd.h math.h stdbool.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor gettimeofday pow setlocale strtol])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
