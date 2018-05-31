#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

int
main(void)
{
        GString *str1 = g_string_new(NULL);

        g_string_printf(str1, "Overwrite! %o", 23);
        puts(str1->str);

        g_string_append_printf(str1, "This is adding to that one 0x%X", 5342);
        puts(str1->str);

        g_string_free(str1, TRUE);
}
