#ifndef CONCURRENCY_H_
#define CONCURRENCY_H_

#include <type.h>

typedef u32_t cpuflg_t;     /* save the cpu flag register */

/* close the interrupt but save the current eflags into memory */
static inline void hal_save_flags_cli(cpuflg_t* flags)
{
    __asm__ __volatile__(
                        "pushfl     \t\n"   /* push eflags register into the top of the stack */
                        "cli        \t\n"   /* close the interrupt */
                        "popl %0    \t\n"   /* pop the top of the stack into memory address */
                        : "=m" (*flags)
                        :
                        : "memory"
                        );
}

/* restore the interrupt status from flags memory */
static inline void has_restore_flags_sti(cpuflg_t* flags)
{
    __asm__ __volatile__(
                        "pushl %0   \t\n"   /* push the value of flags into the top of the stack */
                        "popfl      \t\n"   /* pop the top of the stack into the eflags register */
                        :
                        : "m" (*flags)
                        : "memory"
                        );
}

#endif