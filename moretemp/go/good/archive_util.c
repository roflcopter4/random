#include "archive_util.h"
#include <assert.h>
#include <errno.h>
#include <locale.h>
#include <string.h>

#define safe_stat(PATH, ST)                                     \
     do {                                                       \
             if ((stat((PATH), (ST)) != 0))                     \
                     err(1, "Failed to stat file '%s", (PATH)); \
     } while (0)


/* static void plain_getlines (struct datalist *tags, const char *filename); */

/* ========================================================================== */


static inline void
report_size(struct archive_size *size)
{
        setlocale(LC_NUMERIC, "");
        warnx("Using a buffer of size %'zu for output; filesize is %'zu\n",
              size->uncompressed, size->archive);
}


/* ========================================================================== */
/* GZIP */


char *
gz_getlines(const char *filename)
{
        struct archive_size size;
        gzip_size(&size, filename);
        report_size(&size);

        gzFile gfp = gzopen(filename, "rb");
        if (!gfp)
                err(1, "Failed to open file");

        /* Magic macros to the rescue. */
        uint8_t *out_buf = xmalloc(size.uncompressed + 1);
        int64_t numread  = gzread(gfp, out_buf, size.uncompressed);

        assert (numread == 0 || numread == (int64_t)size.uncompressed);
        gzclose(gfp);

        /* Always remember to null terminate the thing. */
        out_buf[size.uncompressed] = '\0';
        return (char *)out_buf;
}


/* ========================================================================== */
/* XZ */


/* It would be nice if there were some magic macros to read an xz file too. */
char *
xz_getlines(const char *filename)
{
        struct archive_size size;
        xz_size(&size, filename);
        report_size(&size);

        uint8_t *in_buf  = xmalloc(size.archive + 1);
        uint8_t *out_buf = xmalloc(size.uncompressed + 1);

        /* Setup the stream and initialize the decoder */
        lzma_stream strm[] = {LZMA_STREAM_INIT};
        if ((lzma_auto_decoder(strm, UINT64_MAX, 0)) != LZMA_OK)
                errx(1, "Unhandled internal error.");

        lzma_ret ret = lzma_stream_decoder(strm, UINT64_MAX, LZMA_CONCATENATED);
        if (ret != LZMA_OK)
                errx(1, "%s\n", ret == LZMA_MEM_ERROR ?
                        strerror(ENOMEM) : "Internal error (bug)");

        /* avail_in is the number of bytes read from a file to the strm that
         * have not yet been decoded. avail_out is the number of bytes remaining
         * in the output buffer in which to place decoded bytes.*/
        strm->next_out     = out_buf;
        strm->next_in      = in_buf;
        strm->avail_out    = size.uncompressed;
        strm->avail_in     = 0;
        lzma_action action = LZMA_RUN;
        FILE *fp           = safe_fopen(filename, "rb");

        /* We must read the size of the input buffer + 1 in order to
         * trigger an EOF condition.*/
        strm->avail_in = fread(in_buf, 1, size.archive + 1, fp);

        if (ferror(fp))
                err(1, "%s: Error reading input size", filename);
        if (feof(fp))
                action = LZMA_FINISH;
        else
                errx(1, "Error reading file: buffer too small.");

        ret = lzma_code(strm, action);

        if (ret != LZMA_STREAM_END)
                errx(5, "Unexpected error on line %d in file %s: %d => %s",
                        __LINE__, __FILE__, ret, message_strm(ret));

        out_buf[size.uncompressed] = '\0';
        fclose(fp);
        lzma_end(strm);
        free(in_buf);

        return (char *)out_buf;
}
