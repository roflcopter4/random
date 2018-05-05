#include "stuff.h"
#include <bzlib.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


#define err(...)    fprintf(stderr, __VA_ARGS__)
#define eputs(STR_) fputs((STR_), stderr)

#define FILENAME "__home__bml__.vim__dein__repos__github.com__c0r73x__neotags.nvim__neotags_bin__src.tags.bz2"
#define DTYPE char


int
main(void)
{
        /* BZFILE *bzfp = BZ2_bzopen(FILENAME, "rb");
        printf("avail_out is %u\n", bzfp->avail_out); */

        mallopt(M_PERTURB, 0xdeadbeef);

        int bzerror, numread, end;
        errno    = 0;
        bool brk = 0;
        FILE *fp = fopen(FILENAME, "rb");

        if (errno)
                perror("Failed to open file: "), exit(1);

        BZFILE *bzfp  = BZ2_bzReadOpen(&bzerror, fp, 0, 0, NULL, 0);
        uint16_t line = (uint16_t)(__LINE__ - 1);

        if (bzerror) {
                err("Error %d from BZ2_bzReadOpen, line %u\n", bzerror, line);
                exit(2);
        }

        int bufsize = BUFSIZ; /* leave room for null */
        DTYPE *buf = malloc(sizeof *buf * BUFSIZ + 1);
        if (!buf)
                exit(10);

        for (;;) {
                numread = BZ2_bzRead(&bzerror, bzfp,
                                     buf + (bufsize - BUFSIZ), BUFSIZ);

                switch (bzerror) {
                case BZ_OK:
                        eputs("bzerror returned BZ_OK\n");
                        break;
                case BZ_RUN_OK:
                        eputs("bzerror returned BZ_RUN_OK\n");
                        break;
                case BZ_STREAM_END:
                        eputs("bzerror returned BZ_STREAM_END\n");
                        brk = true;
                        break;
                default:
                        err("Unhandled error (%d) in read operation.\n", bzerror);
                        exit(1);
                }

                if (numread == BUFSIZ && !brk) {
                        DTYPE *tmp = realloc(buf, (bufsize += BUFSIZ));
                        if (!tmp)
                                exit(11);
                        buf = tmp;
                } else {
                        err("Read %d bytes, expected %d bytes. Breaking.\n", numread, BUFSIZ);
                        break;
                }
        }

        end = bufsize - (BUFSIZ - numread);
        buf[end] = '\0';

        err("Read %d chars, bufsize %d.\n", end, bufsize);

        /* puts(buf); */
        puts("\n\n\033[1m\033[33mAttempting write:\033[0m\n");
        fwrite(buf, 1, end, stdout);

        BZ2_bzReadClose(NULL, bzfp);
        fclose(fp);
        free(buf);

        puts("\n\n\033[1m\033[33mAttempting write after free:\033[0m\n");
        fwrite(buf, 1, end, stdout);
        puts("\n\n\033[1m\033[33mAttempting puts after free:\033[0m\n");
        puts(buf);

        unsigned char *tmp = (unsigned char *)buf;

        for (int i = 0; i < end; i += 4) {
                uint8_t ch[4];
                ch[0] = *tmp++;
                ch[1] = *tmp++;
                ch[2] = *tmp++;
                ch[3] = *tmp++;

                /* unsigned ans; */
                int ans;
                ans = (ch[0] << 8) | ch[1];
                ans = (ans   << 8) | ch[2];
                ans = (ans   << 8) | ch[3];

                printf("%#x ", ans);
        }

        /* puts("\n\n\033[1m\033[33mAttempting memset after free:\033[0m\n"); */
        /* memset(buf, '!', (size_t)end); */
        fwrite(buf, 1, end, stdout);

        return 0;
}
