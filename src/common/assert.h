#ifndef FRB_ASSERT_H
#define FRB_ASSERT_H

#include <cstdio>
#include <cstdlib>

#define DEBUG_ASSERT

#ifdef DEBUG_ASSERT

#define __assert(cond, line, file, str, cond_str) \
        { \
            if(!(cond)) \
            { \
                fprintf( stderr, \
                         "assertion error on `%s' in %s at line %d: %s\n",  \
                         (cond_str), \
                         (file), \
                         (line), \
                         (str) ); \
                exit(1); \
            } \
        }
        
#define __warning(cond, line, file, str, cond_str) \
        { \
            if(!(cond)) \
            { \
                fprintf( stderr, \
                         "warning on `%s' in %s at line %d: %s\n",  \
                         (cond_str), \
                         (file), \
                         (line), \
                         (str) ); \
            } \
        }

#else

#define __assert(cond, line, file, str, cond_str)

#endif


#define frb_assert2(cond, str) __assert((cond), __LINE__, __FILE__, (str), #cond)
#define frb_assert(cond) frb_assert2((cond), "assertion failed")

#define frb_warning2(cond, str) __warning((cond), __LINE__, __FILE__, (str), #cond)
#define frb_warning(cond) frb_warning2((cond), "warning")

#endif

