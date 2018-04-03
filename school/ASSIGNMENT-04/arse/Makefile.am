#SUBDIRS = src src/third_party
#SUBDIRS = src

bin_PROGRAMS=arse
arse_SOURCES= 	src/main.cpp      \
		src/options.cpp   \
		src/impl.cpp      \
		src/game.cpp      \
		src/display.cpp   \
		src/quicksort.cpp \
		src/utility.c     \
		src/arse.h

arse_LDADD=-lpthread

# EXTRA_DIST=arse.lsm.in arse.spec.in arse.texi

#  if you write a self-test script named `chk', uncomment the
#  following and add `chk' to the EXTRA_DIST list
#TESTS=chk

#  build and install the .info pages
# info_TEXINFOS = doc/arse.texi
# arse_TEXINFOS = doc/gpl.texi

#  install the man pages
# man_MANS=doc/arse.1
