#ifndef foreach_hack_H
#define foreach_hack_H

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif
#include <stdbool.h>

/*===========================================================================*/
/*===========================================================================*/


#define FOREACH(ITEM, COUNT, ARRAY, LEN)   \
    for(int _FE_KEEP_ = 1,                 \
            (COUNT)   = 0;                 \
        _FE_KEEP_ && (COUNT) != (LEN);     \
        _FE_KEEP_ = !_FE_KEEP_, ++(COUNT)) \
      for(ITEM = ((ARRAY) + (COUNT)); _FE_KEEP_; _FE_KEEP_ = !_FE_KEEP_)


/*===========================================================================*/


#define FIXED_ARR_SIZE(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY)))

#define foreach_f(ITEM, ARRAY) \
        FOREACH(ITEM, _FE_COUNTER_, (ARRAY), FIXED_ARR_SIZE(ARRAY))

#define foreach_fC(ITEM, COUNT, ARRAY)                    \
        FOREACH(ITEM, COUNT, (ARRAY), FIXED_ARR_SIZE(ARRAY))

#define foreach_l(ITEM, ARRAY, LEN) \
        FOREACH(ITEM, _FE_COUNTER_, ARRAY, (LEN))

#define foreach_lC(ITEM, COUNT, ARRAY, LEN) \
        FOREACH(ITEM, COUNT, ARRAY, (LEN))

/*===========================================================================*/

#if 0
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
#endif


/*===========================================================================*/
/*===========================================================================*/


#if 0
#define FOREACH_COMP(INDEX, ARRAY, ARRAY_TYPE, SIZE)                \
    __extension__({                                                 \
        bool ret = 0;                                               \
        if (__builtin_types_compatible_p(const char *, ARRAY_TYPE)) \
            ret = INDEX < strlen((const char *)ARRAY);              \
        else                                                        \
            ret = INDEX < SIZE;                                     \
        ret;                                                        \
    })

#define FOREACH_ELEM(INDEX, ARRAY, TYPE) \
    __extension__({                      \
        TYPE *tmp_array_ = ARRAY;        \
        &tmp_array_[INDEX];              \
    })

#define FOREACH(VAR, ARRAY)                                                   \
    for (void *array_ = (void *)(ARRAY); array_; array_ = 0)                  \
        for (size_t i_ = 0; i_ && array_ &&                                   \
                            FOREACH_COMP(i_, array_, __typeof__(ARRAY),       \
                                         sizeof(ARRAY) / sizeof((ARRAY)[0])); \
             ++i_)                                                            \
            for (bool b_ = 1; b_; (b_) ? array_ = 0 : 0, b_ = 0)              \
                for (VAR = FOREACH_ELEM(i_, array_, __typeof__((ARRAY)[0]));  \
                     b_;                                                      \
                     b_ = 0)
#endif


/*===========================================================================*/
/*===========================================================================*/


#define lambda(RETURN_TYPE, FUNCTION_BODY)\
    ({ RETURN_TYPE __fn__ FUNCTION_BODY __fn__; })

#define array_len(ARR) (sizeof((ARR))/sizeof((ARR)[0]))

#define foreachnf(TYPE, ITEM, ARR, ARR_LENGTH, FUNC) {\
    void (*action)(TYPE ITEM) = FUNC;\
    for (int _i__ = 0; _i__ < (ARR_LENGTH); ++_i__) action(ARR[_i__]);\
}

#define foreachf(TYPE, ITEM, ARR, FUNC)\
    foreachnf(TYPE, ITEM, ARR, array_len(ARR), FUNC)

#define foreachn(TYPE, ITEM, ARR, ARR_LENGTH, BODY)\
    foreachnf(TYPE, ITEM, ARR, ARR_LENGTH, lambda(void, (TYPE ITEM) BODY))

#define foreach(TYPE, ITEM, ARR, BODY)\
    foreachn(TYPE, ITEM, ARR, array_len(ARR), BODY)


/*===========================================================================*/
/*===========================================================================*/

#endif /* foreach_hack.h */
