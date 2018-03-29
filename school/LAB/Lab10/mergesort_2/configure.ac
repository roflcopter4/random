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

dnl Checks for programs.
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_PROG_CC

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/param.h sys/time.h time.h sys/mkdev.h sys/sysmacros.h string.h memory.h fcntl.h dirent.h alloca.h locale.h )

AC_CHECK_HEADERS( [sys/ioctl.h sodium.h], [], [echo; echo "Required header not found. Aborting."; exit 1])

PKG_CHECK_MODULES([mergesort], [libsodium])

jm_CHECK_TYPE_STRUCT_UTIMBUF
AC_HEADER_MAJOR
AC_FUNC_ALLOCA
AC_STRUCT_TM
AC_STRUCT_ST_BLOCKS
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS(mkfifo)
AC_CHECK_FUNC(mknod)

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

#AC_OUTPUT(Makefile src/Makefile src/third_party/Makefile scripts/mergesort.lsm scripts/mergesort.spec)
AC_OUTPUT(Makefile src/Makefile)
