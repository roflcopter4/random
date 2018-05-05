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
 * These functions are taken from FreeBSD's libc. Tey are provided as libbsd on
 * linux, but are non-standard enough that it is convenient to just include
 * them in source form. These are almost verbatim copies, I only changed a few
 * variable names I thought were a bit too short (osrc to orig_src, etc).
 */

#include <string.h>


/*
 * Copy string src to buffer dst of size dst_size.  At most dst_size-1
 * chars will be copied.  Always NUL terminates (unless dst_size == 0).
 * Returns strlen(src); if retval >= dst_size, truncation occurred.
 */
size_t
strlcpy(char * restrict dst, const char * restrict src, size_t dst_size)
{
        const char *orig_src = src;
        size_t nleft = dst_size;

        /* Copy as many bytes as will fit */
        if (nleft != 0)
                while (--nleft != 0)
                        if ((*dst++ = *src++) == '\0')
                                break;

        /* If not enough room in dst, add NUL and traverse rest of src to find
         * its remaining length. */
        if (nleft == 0) {
                if (dst_size != 0)
                        *dst = '\0';  /* NUL-terminate dst */
                while (*src++ != '\0')
                        ;
        }

        return (src - orig_src - 1);  /* count does not include NUL */
}


/*
 * Appends src to string dst of size dst_size (unlike strncat, dst_size is the
 * full size of dst, not space left).  At most dst_size-1 characters
 * will be copied.  Always NUL terminates (unless dst_size <= strlen(dst)).
 * Returns strlen(src) + MIN(dst_size, strlen(initial dst)).
 * If retval >= dst_size, truncation occurred.
 */
size_t
strlcat(char * restrict dst, const char * restrict src, size_t dst_size)
{
        const char *orig_dst = dst;
        const char *orig_src = src;
        size_t nleft = dst_size;
        size_t dst_len;

        /* Find the end of dst and adjust bytes left but don't go past end. */
        while (nleft-- != 0 && *dst != '\0')
                ++dst;
        dst_len = dst - orig_dst;
        nleft = dst_size - dst_len;

        if (nleft-- == 0)
                return (dst_len + strlen(src));

        while (*src != '\0') {
                if (nleft != 0) {
                        *dst++ = *src;
                        nleft--;
                }
                ++src;
        }
        *dst = '\0';

        return (dst_len + (src - orig_src));  /* count does not include NUL */
}
