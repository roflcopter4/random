#include <stdio.h>

unsigned long elf_hash(const unsigned char *name);


int main(int argc, char **argv)
{
        unsigned long x;

        if (argc == 1)
                return 1;

        x = elf_hash((const unsigned char *)argv[1]);

        printf("The thing for %s is %lu\n", argv[1], x);

        return 0;
}


unsigned long
elf_hash(const unsigned char *name)
{
        unsigned long h = 0, g;

        while (*name) {
                h = (h << 4lu) + *name++;
                if (g = h & 0xf0000000)
                        h ^= g >> 24lu;
                h &= ~g;
        }
        return h;
}
