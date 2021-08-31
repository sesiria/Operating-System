#ifndef SPINLOCK_H_
#define SPINLOCK_H_

#include <concurrency.h>

#define krlspinlock_cli     x86_spin_lock_disable_irq
#define krlspinunlock_sti   x86_spin_unlock_enabled_irq

/* define the struct of spinlock*/
typedef struct 
{
    volatile u32_t lock; /* prevent from optimized into register file */
} spinlock_t;

/*init spinlock*/
static inline void x86_spin_lock_init(spinlock_t* lock)
{
    lock->lock = 0;      /* zero indicate that */
}

/* normal implementation the spin lock it has the 
   bug that it will be affect by the interrupt */
static inline void x86_spin_lock(spinlock_t* lock)
{
    __asm__ __volatile__ (
                        "1:     \t\n"                   /* set 1 to an arbitrary register */
                        "lock;  xchg %0, %1 \t\n"       /* set lock and exchange the value of the register to variable lock */
                        "cmpl   $0, %0      \t\n"       /* compare the value of the register to 0 */
                        "jnz    2f          \t\n"       /* if not equal to zero then jump to 2:  */
                        "jmp    3f          \t\n"       /* if equal to zero then jump to 3: */
                        "2:                 \t\n"       /* label 2: */
                        "cmpl   $0, %1      \t\n"       /* compare 0 with the value of variable lock */
                        "jne    2b          \t\n"       /* if not equal to zero then jump to label 2: */
                        "jmp    1b          \t\n"       /* if equal to zero then jump to label 1, exchange and lock */
                        "3:                 \t\n"
                        :
                        : "r" (1), "m" (*lock));
}

/* normal implementation the spin unlock it has the
   bug that it will be affect by the interrupt */
static inline void x86_spin_lock(spinlock_t* lock)
{
    __asm__ __volatile__(
                        "movl   $0, %0      \t\n"       /* unlock and zet zero to the lock */
                        :
                        : "m" (*lock));
}

/* spin lock with irq support */
static inline void x86_spin_lock_disable_irq(spinlock_t* lock, cpuflg_t* flags)
{
    __asm__ __volatile__(
                        "pushfq             \t\n"
                        "cli                \t\n"
                        "popq   %0          \t\n"
                        "1:     \t\n"                   /* set 1 to an arbitrary register */
                        "lock;  xchg %0, %1 \t\n"       /* set lock and exchange the value of the register to variable lock */
                        "cmpl   $0, %0      \t\n"       /* compare the value of the register to 0 */
                        "jnz    2f          \t\n"       /* if not equal to zero then jump to 2:  */
                        "jmp    3f          \t\n"       /* if equal to zero then jump to 3: */
                        "2:                 \t\n"       /* label 2: */
                        "cmpl   $0, %1      \t\n"       /* compare 0 with the value of variable lock */
                        "jne    2b          \t\n"       /* if not equal to zero then jump to label 2: */
                        "jmp    1b          \t\n"       /* if equal to zero then jump to label 1, exchange and lock */
                        "3:                 \t\n"
                        : "=m" (*flags)
                        : "r" (1), "m" (*lock));
}

/* spin unlock with irq support */
static inline void x86_spin_unlock_enabled_irq(spinlock_t* lock, cpuflg_t* flags)
{
    __asm__ __volatile__(
                        "movl   $0, %0      \t\n"       /* unlock and zet zero to the lock */
                        "pushq  %1          \t\n"
                        "popfq              \t\n"
                        :
                        : "m" (*lock), "m" (*flags));
}



#endif