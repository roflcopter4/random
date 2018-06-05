#include "archive_util.h"

#define safe_stat(PATH, ST)                                     \
     do {                                                       \
             if ((stat((PATH), (ST)) != 0))                     \
                     err(1, "Failed to stat file '%s", (PATH)); \
     } while (0)


static inline bool
file_is_reg(const char *filename)
{
        struct stat st;
        safe_stat(filename, &st);
        return S_ISREG(st.st_mode);
}


FILE *
safe_fopen(const char * const restrict filename,
           const char * const restrict mode)
{
        FILE *fp = fopen(filename, mode);
        if (!fp)
                err(1, "Failed to open file '%s'", filename);
        if (!file_is_reg(filename))
                errx(1, "Invalid filetype '%s'\n", filename);
        return fp;
}


void *
xmalloc(const size_t size)
{
        void *tmp = malloc(size);
        if (tmp == NULL)
                err(100, "Malloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


void *
xcalloc(const int num, const size_t size)
{
        void *tmp = calloc(num, size);
        if (tmp == NULL)
                err(125, "Calloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


void *
xrealloc(void *ptr, const size_t size)
{
        void *tmp = realloc(ptr, size);
        if (tmp == NULL)
                err(150, "Realloc call failed - attempted %zu bytes\n", size);
        return tmp;
}


int
find_num_cpus(void)
{
#ifdef DOSISH
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
#elif MACOS
        int nm[2];
        size_t len = 4;
        uint32_t count;

        nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);

        if (count < 1) {
                nm[1] = HW_NCPU;
                sysctl(nm, 2, &count, &len, NULL, 0);
                if (count < 1) { count = 1; }
        }
        return count;
#else
        return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}
