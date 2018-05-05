/*
 * These functions are taken from FreeBSD's libc. They are provided as libbsd
 * on linux, but are non-standard enough that it is convenient to just include
 * them in source form. These are almost verbatim copies, I only changed a few
 * variable names I thought were a bit too short (osrc to orig_src, etc). The
 * copyright notices were copied entirely unchanged.
 */

#include "ubsd.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/*============================================================================*/
/*============================================================================*/
/* strlcpy */

/* $OpenBSD: strlcpy.c,v 1.12 2015/01/15 03:54:12 millert Exp $ */
/*
 * Copyright (c) 1998, 2015 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/*
 * Copy string src to buffer dst of size dst_size.  At most dst_size-1
 * chars will be copied.  Always NUL terminates (unless dst_size == 0).
 * Returns strlen(src); if retval >= dst_size, truncation occurred.
 */
size_t
ustrlcpy(unsigned char * restrict dst,
        const unsigned char * restrict src,
        size_t dst_size)
{
        const unsigned char *orig_src = src;
        size_t nleft = dst_size;

        if (src == NULL)
                return 0L;

        /* Copy as many bytes as will fit */
        if (nleft != 0)
                while (nleft-- != 0)  /* Break when nleft is 1 -> room for null */
                        *dst++ = *src++;

        return (src - orig_src);
}

/*============================================================================*/
/*============================================================================*/
/* strlcat */

/* $OpenBSD: strlcat.c,v 1.15 2015/03/02 21:41:08 millert Exp $ */
/*
 * Copyright (c) 1998, 2015 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Appends src to string dst of size dst_size (unlike strncat, dst_size is the
 * full size of dst, not space left).  At most dst_size-1 unsigned characters
 * will be copied.  Always NUL terminates (unless dst_size <= strlen(dst)).
 * Returns strlen(src) + MIN(dst_size, strlen(initial dst)).
 * If retval >= dst_size, truncation occurred.
 */
size_t
ustrlcat(unsigned char * restrict dst,
         const unsigned char * restrict src,
         size_t dst_size)
{
        const unsigned char *orig_dst = dst;
        const unsigned char *orig_src = src;
        size_t nleft = dst_size;
        size_t dst_len;

        /* Find the end of dst and adjust bytes left but don't go past end. */
        while (nleft-- != 0 && *dst != '\0')
                ++dst;
        dst_len = dst - orig_dst;
        nleft = dst_size - dst_len;

        if (nleft-- == 0)
                return (dst_len + ustrlen(src));

#if 0
        while (*src != '\0') {
                if (nleft != 0) {
                        *dst++ = *src;
                        nleft--;
                }
                ++src;
        }
#endif
        while (*src && --nleft != 0)
                *dst++ = *src++;

        *dst = '\0';

        return (dst_len + (src - orig_src));  /* count does not include NUL */
}


size_t
ustrlen(const unsigned char *str)
{
        const unsigned char *tmp = str;

        while (*tmp)
                ++tmp;

        return tmp - str;
}
