#ifndef foreach_hack_H
#define foreach_hack_H


#define foreach_f(ITEM, ARRAY)                          \
    for(int keep  = 1,                                  \
            count = 0,                                  \
            size  = (sizeof(ARRAY) / sizeof(*(ARRAY))); \
        keep && count != size;                          \
        keep = !keep, count++)                          \
      for(ITEM = (ARRAY) + count; keep; keep = !keep)

#define foreach_fC(ITEM, COUNT, ARRAY)                    \
    for(int keep    = 1,                                  \
            (COUNT) = 0,                                  \
            size    = (sizeof(ARRAY) / sizeof(*(ARRAY))); \
        keep && (COUNT) != size;                          \
        keep = !keep, (COUNT)++)                          \
      for(ITEM = (ARRAY) + (COUNT); keep; keep = !keep)


#define foreach_l(ITEM, ARRAY, LEN) \
    for(int keep  = 1,              \
            count = 0;              \
        keep && count != (LEN);     \
        keep = !keep, count++)      \
      for(ITEM = ((ARRAY) + count); keep; keep = !keep)

#define foreach_lC(ITEM, COUNT, ARRAY, LEN) \
    for(int keep    = 1,                    \
            (COUNT) = 0;                    \
        keep && (COUNT) != (LEN);           \
        keep = !keep, (COUNT)++)            \
      for(ITEM = ((ARRAY) + (COUNT)); keep; keep = !keep)


#endif /* foreach_hack.h */
