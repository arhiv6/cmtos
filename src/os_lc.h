#ifndef OS_LC_H
#define OS_LC_H

/*
*   Локальные продолжения (Local Сontinuations - LC)
*
*   LC_INIT  - инициализация LC или сброс ранее инициализированого
*   LC_BEGIN - объявление начала использования LC
*   LC_YIELD - передача управления (выход из функции, возврат в это же место)
*   LC_END   - объявление окончания использования LC
*/

#if defined(OS_LC_USE_SWICH)        /* LC, основанные на switch/case */

    #include  <stdint.h>

    typedef uintptr_t lc_t;

    #define LC_INIT(s)      *(s) = 0
    #define LC_BEGIN(s)     switch (*(s)) { case 0: (void)0
    #define LC_YIELD(s)     *(s) = __LINE__; return; case __LINE__ : (void)0
    #define LC_END()        } (void)0

#elif defined(OS_LC_USE_GOTO)       /* LC, основанные на goto/label */

    #include <stddef.h>

    typedef void *lc_t;

    #define LC_CONCAT(id)   lc_label_ ## id
    #define LC_LABEL(id)    LC_CONCAT(id)

    #define LC_INIT(s)      *(s) = NULL
    #define LC_BEGIN(s)     if (*(s)) goto *(*(s))
    #define LC_YIELD(s)     *(s) = &&LC_LABEL(__LINE__); return; LC_LABEL(__LINE__) : (void)0
    #define LC_END()

#elif defined(OS_LC_USE_SETJMP)     /* LC, основанные на setjmp/longjmp */

    #include <stdbool.h>
    #include <setjmp.h>

    typedef struct
    {
        jmp_buf buf;
        bool is_init;
    }  lc_t;

    #define LC_INIT(s)      (s)->is_init = false
    #define LC_BEGIN(s)     if ((s)->is_init) longjmp((s)->buf, 0); else (s)->is_init = true
    #define LC_YIELD(s)     if (setjmp((s)->buf) == 0) return
    #define LC_END()

#else
    #error OS_LC_USE_x not defined
#endif

#endif /* OS_LC_H */
