#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <spinlock.h>

#define SEM_FLG_MUTEX       0
#define SEM_FLG_MULTI       1
#define SEM_MUTEX_ONE_LOCK  1
#define SEM_MULTI_LOCK      0

/* wait list of the semaphore */
typedef struct s_KWLST
{
    spinlock_t  wl_lock;
    u32_t       wl_tdnr;
    list_h_t    wl_list;
}kwlst_t;

/* struct of the semaphore */
typedef struct s_SEM
{
    spinlock_t  sem_lock;       /* spinlock used by the semaphore */
    u32_t       sem_flg;        /* flag of the semaphore */
    s32_t       sem_count;      /* counter of the semaphore */
    kwlst_t     sem_waitlst;    /* waitlist struct of the semaphore */
}sem_t;

/* acquire semaphore */
void krlsem_down(sem_t* sem)
{
    cpuflg_t    cpufg;
start_step:
    krlspinlock_cli(&sem->sem_lock, &cpufg);
    /* if the counter of the semaphore is less than 1, then let the thread to sleep */
    if (sem->sem_count < 1)
    {   
        krlwlst_wait(&sem->sem_waitlst);
        krlspinunlock_sti(&sem->sem_lock, &cpufg);
        krlschedul();
        goto start_step;
    }
    sem->sem_count--;           /* decrease the counter of the semaphore indicate that success to acquire the semaphore */
    krlspinunlock_sti(&sem->sem_lock, &cpufg);
    return;
}

/* release semaphore*/
void krlsem_up(sem_t* sem)
{
    cpuflg_t    cpufg;
    krlspinlock_cli(&sem->sem_lock, &cpufg);
    sem->sem_count++;           /* release semaphore */
    /* if the counter is less than 1, indicate there is an error , halt the system */
    if (sem->sem_count < 1)
    {
        krlspinunlock_sti(&sem->sem_lock, &cpufg);
        hal_sysdie("sem up err");
    }
    /* wake up the threads from the waitlist of the semaphore */
    krlwlst_allup(&sem->sem_waitlst);
    krlspinunlock_sti(&sem->sem_lock, &cpufg);
    krlsched_set_schedflgs();
    return;
}

#endif