#ifndef BSD_FUNCS_H
#define BSD_FUNCS_H

#ifdef __cplusplus
   extern "C" {
#endif

#include <string.h>
#ifndef __GNUC__
#  define restrict __restrict
#endif


size_t ustrlen(const unsigned char *str);
size_t ustrlcpy(unsigned char * restrict dst, const unsigned char * restrict src, size_t dst_size);
size_t ustrlcat(unsigned char * restrict dst, const unsigned char * restrict src, size_t dst_size);


#ifdef __cplusplus
   }
#endif

#endif /* bsd_funcs.h */
