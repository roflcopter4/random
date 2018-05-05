dnl Process this file with autoconf to produce a configure script.

# AC_INIT(mergesort.c)
# dnl Every other copy of the package version number gets its value from here
# AM_INIT_AUTOMAKE(mergesort, 0.1.0)
 
AC_PREREQ([2.69])
AC_INIT([mergesort],[1.0.0],[leason@ualberta.ca])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
#AM_INIT_AUTOMAKE

# dnl create a config.h file (Automake will add -DHAVE_CONFIG_H)
# AM_CONFIG_HEADER(config.h)

AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lncurses':
AC_CHECK_LIB([ncurses], [main])
# FIXME: Replace `main' with a function in `-lsodium':
AC_CHECK_LIB([sodium], [main])
# FIXME: Replace `main' with a function in `-ltinfo':
AC_CHECK_LIB([tinfo], [main])

# Checks for header files.
AC_CHECK_HEADERS([limits.h locale.h stdint.h stdlib.h string.h sys/ioctl.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday setlocale strtol])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
