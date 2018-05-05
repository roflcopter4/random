#include "stuff.h"
#include <errno.h>
#include <lzma.h>
#include <stdlib.h>
#include <string.h>

static char * uncompress(lzma_stream *strm, FILE *file, const char *filename);
static size_t ustrncpy(char * restrict dst, const unsigned char * restrict src, size_t dst_size);

/* #define FILENAME "tagfile.xz" */
#define FILENAME "__home__bml__.vim__dein__repos__github.com__c0r73x__neotags.nvim__neotags_bin__src.tags.xz"
#define OFILE    "shit.h"

int
main(void)
{
        lzma_stream lzfp = LZMA_STREAM_INIT;
        if ((lzma_auto_decoder(&lzfp, UINT64_MAX, 0)) != LZMA_OK)
                xerr(1, "Some kinda error or like whatever.\n");

        FILE *comp = fopen(FILENAME, "rb");
        if (!comp)
                xperror("Failed to open file '%s'", FILENAME);

        char *str = uncompress(&lzfp, comp, FILENAME);
        fputs(str, stdout);

        lzma_end(&lzfp);
        fclose(comp);
        free(str);
        
        return 0;
}


static char *
uncompress(lzma_stream *strm, FILE *file, const char *filename)
{
        lzma_ret ret = lzma_stream_decoder(strm, UINT64_MAX, LZMA_CONCATENATED);

        if (ret != LZMA_OK)
                xerr(1, "%s", ret == LZMA_MEM_ERROR
                     ? strerror(ENOMEM) : "Internal error (bug)");

        uint8_t in_buf[BUFSIZ], out_buf[BUFSIZ];

        strm->avail_in     = 0;
        strm->next_out     = out_buf;
        strm->avail_out    = BUFSIZ;
        lzma_action action = LZMA_RUN;
        struct string str  = { malloc(1), 1 };
        bool first         = 1;

        while (ret == LZMA_OK)
        {
                if (strm->avail_in == 0) {
                        strm->next_in  = in_buf;
                        strm->avail_in = fread(in_buf, 1, BUFSIZ, file);

                        if (ferror(file))
                                xperror("%s: Error reading input file", filename);
                        if (feof(file))
                                action = LZMA_FINISH;
                }

                ret = lzma_code(strm, action);

                if (strm->avail_out == 0 || ret != LZMA_OK)
                {
                        const size_t write_size = BUFSIZ - strm->avail_out;
                        char *tmp;

                        /* eprintf("Trying to realloc from size %zu to %zu\n",
                                str.len, str.len + write_size); */

                        if (!(tmp = realloc(str.s, str.len + write_size)))
                                xerr(4, "Realloc failed...\n");

                        /* eprintf("Copied %zu chars. Also, len is %zu.\n",
                                ustrncpy((str.s = tmp) + (first ? (first = 0) : str.len - 1),
                                         out_buf, write_size),
                                str.len + write_size); */

                        ustrncpy((str.s = tmp) + (first ? (first = 0) : str.len - 1),
                                 out_buf, write_size);

                        str.len        += write_size;
                        strm->next_out  = out_buf;
                        strm->avail_out = BUFSIZ;
                }

                if (ret != LZMA_OK && ret != LZMA_STREAM_END)
                        xperror("lzma error, panicing!\n");
        }

        str.s[str.len - 1] = '\0';
        return str.s;
}


static size_t
ustrncpy(char * restrict dst, const unsigned char * restrict src, size_t dst_size)
{
        const unsigned char *orig_src = src;

        if (src == NULL)
                return 0;

        /* Copy exactly as many as specified */
        if (dst_size > 0)
                while (dst_size-- > 0)
                        *dst++ = (char)(*src++);

        return (src - orig_src);
}
