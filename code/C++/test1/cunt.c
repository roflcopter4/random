#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void * xmalloc(size_t size)
{
        void *ret;
        if (!(ret = malloc(size)))
                perror("Malloc failed"), abort();
        return ret;
}

static inline void * xrealloc(void *ptr, size_t size)
{
        void *ret;
        if (!(ret = realloc(ptr, size)))
                perror("Realloc failed"), abort();
        return ret;
}


int main(void)
{
        FILE *fp   = fopen("BSDmakefile", "rb");
        size_t max = 128;
        size_t num = 0;
        char **lst = xmalloc(max * sizeof(*lst));

        for (bool done = false; !done;) {
                if (num == max)
                        lst = xrealloc(lst, (max += 64) * sizeof(*lst));

                size_t n = 128;
                lst[num] = xmalloc(n);

                if ((done = (getline(&lst[num++], &n, fp) == -1)))
                        free(lst[--num]);
        }

        for (unsigned i = 0; i < num; ++i) {
                printf("%3u:  %s", i + 1, lst[i]);
                free(lst[i]);
        }

        free(lst);
        fclose(fp);
}
