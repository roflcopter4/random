#include "neotags.h"
#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

#include <cinttypes>
#include <zlib.h>

#define STARTSIZE 2048
#define INCSIZE   256
#define GUESS 100
#define INC   10

#define safe_stat(PATH, ST)                                     \
     do {                                                       \
             if ((stat((PATH), (ST)) != 0))                     \
                     err(1, "Failed to stat file '%s", (PATH)); \
     } while (0)


FILE *
safe_fopen(const char * const __restrict filename,
           const char * const __restrict mode)
{
        FILE *fp = fopen(filename, mode);
        if (!fp)
                err(1, "Failed to open file '%s'", filename);
        if (!file_is_reg(filename))
                errx(1, "Invalid filetype '%s'\n", filename);
        return fp;
}


bool
file_is_reg(const char *filename)
{
        struct stat st;
        safe_stat(filename, &st);
        return S_ISREG(st.st_mode);
}


void *
xmalloc(const size_t size)
{
        void *tmp = malloc(size);
        if (tmp == nullptr)
                err(100, "Malloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


void *
xcalloc(const size_t num, const size_t size)
{
        void *tmp = calloc(num, size);
        if (tmp == nullptr)
                err(101, "Calloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


void *
xrealloc(void *ptr, const size_t size)
{
        void *tmp = realloc(ptr, size);
        if (tmp == nullptr)
                err(102, "Realloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


int64_t
__xatoi(char *str, const bool strict)
{
        char *endptr;
        const auto val = strtol(str, &endptr, 10);

        if ((endptr == str) || (strict && *endptr != '\0'))
                errx(30, "Invalid integer '%s'.\n", str);

        return val;
}


void
__dump_list(char **list, FILE *fp, const char *varname)
{
        char *tmp, **lcpy = list;
        fprintf(fp, "Dumping list: %s\n", varname);
        while ((tmp = *lcpy++) != nullptr)
                fprintf(fp, "        %s\n", tmp);
}


void
__dump_string(char *str, const char *filename, FILE *fp, const char *varname)
{
        FILE *log;

        if (fp != nullptr)
                log = fp;
        else if (filename != nullptr)
                log = safe_fopen(filename, "wb");
        else
                errx(1, "%s at line %d: %s", __FILE__, __LINE__,
                     strerror(EINVAL));

        eprintf("Dumping string '%s' to file '%s'.\n",
                varname, (filename == nullptr ? "anon" : filename));

        fputs(str, log);

        if (filename != nullptr)
                fclose(log);
}


void
__free_all(void *ptr, ...)
{
        va_list ap;
        va_start(ap, ptr);

        do free(ptr);
        while ((ptr = va_arg(ap, void *)) != nullptr);

        va_end(ap);
}


#ifdef DOSISH
char *
basename(char *path)
{
        assert(path != nullptr && *path != '\0');
        size_t len = strlen(path);
        char *ptr = path + len;
        while (*ptr != '/' && *ptr != '\\' && ptr != path)
                --ptr;
        
        return (*ptr == '/' || *ptr == '\\') ? ptr + 1 : ptr;
}
#endif


#ifndef HAVE_ERR
void
_warn(const bool print_err, const char *const __restrict fmt, ...)
{
        va_list ap;
        char *buf;
        va_start(ap, fmt);

#ifdef HAVE_ASPRINTF
        if (print_err)
                asprintf(&buf, "%s: %s: %s\n", program_name, fmt,
                         strerror(errno));
        else
                asprintf(&buf, "%s: %s\n", program_name, fmt);
#else
        size_t size;
        if (print_err) {
                char tmp[BUFSIZ];
                /* strerror() is guarenteed to be less than 8192, strcpy is fine. */
                strcpy(tmp, strerror(errno));
                size = strlen(fmt) + strlen(program_name) + strlen(tmp) + 6;
                buf = new char[size];
                snprintf(buf, size, "%s: %s: %s\n", program_name, fmt, tmp);
        } else {
                size = strlen(fmt) + strlen(program_name) + 4;
                buf = new char[size];
                snprintf(buf, size, "%s: %s\n", program_name, fmt);
        }
#endif

        if (buf == nullptr) {
                /* Allocation failed: print the original format and a \n. */
                vfprintf(stderr, fmt, ap);
                fputc('\n', stderr);
        } else {
                vfprintf(stderr, buf, ap);
                delete[] buf;
        }

        va_end(ap);
}
#endif


int
find_num_cpus(void)
{
#ifdef _WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
#elif MACOS
        int nm[2];
        size_t len = 4;
        uint32_t count;

        nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
        sysctl(nm, 2, &count, &len, nullptr, 0);

        if (count < 1) {
                nm[1] = HW_NCPU;
                sysctl(nm, 2, &count, &len, nullptr, 0);
                if (count < 1) { count = 1; }
        }
        return count;
#else
        return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}
