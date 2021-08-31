#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <type.h>

/* define basic atomic type */
typedef struct s_ATOMIC {
    volatile s32_t a_count; // the volatile will prevent the value from being optimized to register file.
} atomic_t;

/* atomic read */
static inline s32_t atomic_read(const atomic_t *v)
{
    return (*(volatile u32_t*)&(v)->a_count);
}

/* atomic write */
static inline void atomic_write(atomic_t *v, int i)
{
    v->a_count = i;
}

/* atomic add */
static inline void atomic_add(int i, atomic_t * v)
{
    __asm__ __volatile__("lock;" "addl %1,%0"
                        : "+m" (v->a_count)
                        : "ir" (i));
}

/* atomic sub */
static inline void atomic_sub(int i, atomic_t *v)
{
    __asm__ __volatile__("lock;" "subl %1,%0"
                        : "+m" (v->a_count)
                        : "ir" (i));
}

/* atomic increament */
static inline void atomic_inc(atomic_t *v)
{
    __asm__ __volatile__("lock;" "incl %0"
                        : "+m" (v->a_count));
}

/* atomic decreament */
static inline void atomic_dec(atomic_t *v)
{
    __asm__ __volatile__("lock;" "decl %0"
                        : "+m" (v->a_count));
}
#endif