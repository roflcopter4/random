#include "stuff.h"
#include "ubsd.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <lzma.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char * uncompress(lzma_stream *strm, FILE *file, const char *filename, FILE *out_stream);

static inline void handle_error(lzma_ret, lzma_stream *, lzma_action, FILE *, const char *);

/* #define FILENAME "stuff.h.xz" */
#define FILENAME "tagfile.xz"
#define OFILE    "shit.h"
// #define LZMADEC

enum lazypipe {READ, WRITE};

struct ustring {
        unsigned char *s;
        size_t len;
};


int
main(void)
{
        lzma_stream lzfp = LZMA_STREAM_INIT;
        if ((lzma_auto_decoder(&lzfp, UINT64_MAX, 0)) != LZMA_OK)
                xerr(1, "Some kinda error or like whatever.\n");

        FILE *comp = fopen(FILENAME, "rb");
        if (!comp)
                xperror("Failed to open file '%s'", FILENAME);

        int pipefd[2];
        pipe(pipefd);
        if (errno)
                xperror("Failed to open pipe");

        FILE *pipefp[2] = { fdopen(pipefd[READ], "rb"), fdopen(pipefd[WRITE], "wb") };

        /* struct ustring *str = uncompress(&lzfp, comp, FILENAME, pipefp[WRITE]); */
        char *str = uncompress(&lzfp, comp, FILENAME, pipefp[WRITE]);

        if (str != NULL) {
                char *tmp = str;
                while (*tmp)
                        fputc((int)(*tmp++), stderr);
        } else {
                fputs("What the fucking fuck?\n", stderr);
        }

        fclose(pipefp[WRITE]);
        close(pipefd[WRITE]);

        int ch;
        while ((ch = fgetc(pipefp[READ])) != EOF)
                putchar(ch);

        lzma_end(&lzfp);
        fclose(comp);

        fclose(pipefp[READ]);
        close(pipefd[READ]);

        free(str);
        
        return 0;
}


static char *
uncompress(lzma_stream *strm,
           FILE *file,
           const char *filename,
           FILE *out_stream)
{
        lzma_ret ret;
        ret = lzma_stream_decoder(strm, UINT64_MAX, LZMA_CONCATENATED);

        /* The only reasonable error here is LZMA_MEM_ERROR. */
        if (ret != LZMA_OK)
                xerr(1, "%s", ret == LZMA_MEM_ERROR
                              ? strerror(ENOMEM)
                              : "Internal error (bug)");

        uint8_t in_buf[BUFSIZ];
        uint8_t out_buf[BUFSIZ];

        struct ustring *str = malloc(sizeof *str);
        str->s = malloc(str->len = 1);

        strm->avail_in  = 0;
        strm->next_out  = out_buf;
        strm->avail_out = BUFSIZ;

        lzma_action action = LZMA_RUN;
        bool first = 1;
        uint8_t *tmp;

        for (;;) {
                if (strm->avail_in == 0) {
                        strm->next_in  = in_buf;
                        strm->avail_in = fread(in_buf, 1, BUFSIZ, file);

                        if (ferror(file))
                                xperror("%s: Error reading input file", filename);
                        if (feof(file))
                                action = LZMA_FINISH;
                }

                ret = lzma_code(strm, action);

#if 0
                        if (i++ == 0) {
                                ustrlcpy(str.s, out_buf, write_size);
                        } else {
                                uint8_t *tmp = realloc(str.s, str.len += BUFSIZ);
                                if (!tmp)
                                        xerr(4, "Realloc failed...\n");
                                ustrlcat((str.s = tmp), out_buf, str.len);
                        }
#endif


                /*
                 * Write and check write error before checking decoder error.
                 * This way as much data as possible gets written to output even
                 * if decoder detected an error.
                 */
                if (strm->avail_out == 0 || ret != LZMA_OK) {
                        const size_t write_size = BUFSIZ - strm->avail_out;
                        eprintf("Size -> %zu, next -> %c\n", write_size, *out_buf);

                        if (!(tmp = realloc(str->s, str->len + write_size)))
                                xerr(4, "Realloc failed...\n");

                        eprintf("Copied %zu chars. Also, len is %zu.\n",
                                ustrlcpy((str->s = tmp) + (first
                                                           ? (first = 0)
                                                           : (str->len - 1)),
                                         out_buf, (str->len - 1) + write_size),
                                str->len + write_size);

                        str->len += write_size;

                        if (fwrite(out_buf, 1, write_size, out_stream) != write_size)
                                xperror("Cannot write to standard output");

                        strm->next_out  = out_buf;
                        strm->avail_out = BUFSIZ;
                }

                if (ret != LZMA_OK) {
                        handle_error(ret, strm, action, file, filename);
                        break;
                }
        }

        str->s[str->len - 1] = '\0';

        tmp = str->s;

        char *buf, *obuf;
        buf = obuf = malloc(str->len);

        while ((*buf++ = (char)(*tmp++)))
                ;

        free(str->s);
        free(str);

        return obuf;
}


static inline void
handle_error(lzma_ret ret, lzma_stream *strm, lzma_action action, FILE *file, const char *filename)
{
        if (ret == LZMA_STREAM_END) {
                assert(strm->avail_in == 0);
                assert(action == LZMA_FINISH);
                assert(feof(file));
                return;
        }

        const char *msg;
        switch (ret) {
        case LZMA_MEM_ERROR:     msg = strerror(ENOMEM);                  break;
        case LZMA_FORMAT_ERROR:  msg = "File format not recognized";      break;
        case LZMA_OPTIONS_ERROR: msg = "Unsupported compression options"; break;
        case LZMA_DATA_ERROR:    msg = "File is corrupt";                 break;
        case LZMA_BUF_ERROR:     msg = "Unexpected end of input";         break;
        default:                 msg = "Internal error (bug)";            break;
        }

        xerr(1, "%s: %s\n", filename, msg);
}
