#ifndef BSD_FUNCS_H
#define BSD_FUNCS_H

#ifdef __cplusplus
   extern "C" {
#endif

#if (defined(_WIN64) || defined(_WIN32)) && !defined(__CYGWIN__)
   char *strsep(char **stringp, const char *delim);
#endif

#ifdef __cplusplus
   }
#endif

#endif /* bsd_funcs.h */
