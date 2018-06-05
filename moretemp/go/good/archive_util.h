#ifndef ARCHIVE_UTIL_H
#define ARCHIVE_UTIL_H

#define _GNU_SOURCE

#include <err.h>
#include <lzma.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zlib.h>

struct archive_size {
        size_t archive;
        size_t uncompressed;
};

extern char * gz_getlines (const char *filename);
extern char * xz_getlines (const char *filename);

extern void gzip_size(struct archive_size *size, const char *name);
extern void xz_size(struct archive_size *size, const char *filename);
extern const char * message_strm(lzma_ret);

extern void * xmalloc(const size_t size);
extern void * xcalloc(const int num, const size_t size);
extern void * xrealloc(void *ptr, const size_t size);
extern FILE * safe_fopen(const char * const restrict filename,
                         const char * const restrict mode);


#endif /* archive_util.h */
