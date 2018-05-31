#include "archive_util.h"
#include "neotags.h"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using uint = unsigned int;

#define safe_stat(PATH, ST)                                     \
     do {                                                       \
             if ((stat((PATH), (ST)) != 0))                     \
                     err(1, "Failed to stat file '%s", (PATH)); \
     } while (0)


static void ll_strsep      (vector<struct lldata> *tags, char *buf);
static void plain_getlines (vector<struct lldata> *tags, const char *filename);
static void gz_getlines    (vector<struct lldata> *tags, const char *filename);
#ifdef LZMA_SUPPORT
static void xz_getlines    (vector<struct lldata> *tags, const char *filename);
#endif

/* ========================================================================== */


int
getlines(vector<struct lldata> *tags,
         const string *comptype,
         const string *filename)
{
        if (*comptype == "none")
                plain_getlines(tags, filename->c_str());
        else if (*comptype == "gzip")
                gz_getlines(tags, filename->c_str());
#ifdef LZMA_SUPPORT
        else if (*comptype == "lzma")
                xz_getlines(tags, filename->c_str());
#endif
        else {
                cerr << "Unknown compression type " << comptype << "!" << endl;
                return 0;
        }
        return 1; /* 1 indicates success here... */
}


static void
ll_strsep(vector<struct lldata> *tags, char *buf)
{
        char *bak = buf;
        char *tok;

        while ((tok = strsep(&buf, "\n")) != nullptr) {
                if (*tok == '\0')
                        continue;
                struct lldata tmp = { string(tok), '\0' };
                tags->push_back(tmp);
        }

        delete[] bak;
}


#define DEBUG
#ifdef DEBUG
    static inline void
    report_size(struct archive_size *size)
    {
            cerr << "Using a buffer of size "   << size->uncompressed
                 << " for output; filesize is " << size->archive << endl;
    }
#else
#   define report_size(...)
#endif


/* ========================================================================== */
/* PLAIN */


static void
plain_getlines(vector<struct lldata> *tags, const char *filename)
{
        FILE *fp = safe_fopen(filename, "rb");
        struct stat st;
        safe_stat(filename, &st);

        const uint size = static_cast<const uint>(st.st_size);
        char *buffer = new char[size + 1LLU];

        if (fread(buffer, 1LLU, size, fp) != size || ferror(fp))
                err(1, "Error reading file %s", filename);

        buffer[size] = '\0';

        fclose(fp);
        ll_strsep(tags, buffer);
}


/* ========================================================================== */
/* GZIP */

#include <zlib.h>


static void
gz_getlines(vector<struct lldata> *tags, const char *filename)
{
        struct archive_size size;
        gzip_size(&size, filename);
        report_size(&size);

        int ret;
        auto *in_buf  = new uint8_t[size.archive];
        auto *out_buf = new uint8_t[size.uncompressed + 1];
        FILE *fp      = safe_fopen(filename, "rb");

        /* Read the data from the file in one go. */
        fread(in_buf, 1, size.archive, fp);
        if (ferror(fp))
                err(1, "%s: Error in read operation", filename);
        fclose(fp);

        /* Setup the zlib stream. */
        z_stream strm;
        strm.avail_in  = size.archive;
        strm.avail_out = size.uncompressed;
        strm.next_in   = in_buf;
        strm.next_out  = out_buf;
        strm.opaque    = Z_NULL;
        strm.zalloc    = Z_NULL;
        strm.zfree     = Z_NULL;

        if ((ret = inflateInit2(&strm, 32)) != Z_OK)
                errx(1, "Something broke during init (%d) -> %s\n",
                     ret, strm.msg);

        if ((ret = inflate(&strm, Z_FINISH)) != Z_STREAM_END)
                errx(1, "Something broke during decompression (%d) -> %s\n",
                     ret, strm.msg);

        ret = inflateEnd(&strm);
        assert(ret == Z_OK);
        delete[] in_buf;

        /* Always remember to null terminate the thing. */
        out_buf[size.uncompressed] = '\0';
        ll_strsep(tags, reinterpret_cast<char *>(out_buf));
}


/* ========================================================================== */
/* XZ */

#define LZMA_SUPPORT
#ifdef LZMA_SUPPORT
#   include <lzma.h>
extern const char * message_strm(lzma_ret);


static void
xz_getlines(vector<struct lldata> *tags, const char *filename)
{
        struct archive_size size;
        xz_size(&size, filename);
        report_size(&size);

        auto *in_buf  = new uint8_t[size.archive + 1LLU];
        auto *out_buf = new uint8_t[size.uncompressed + 1LLU];

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
        delete[] in_buf;

        ll_strsep(tags, reinterpret_cast<char *>(out_buf));
}
#endif
