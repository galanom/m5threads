/*
    m5threads, a pthread library for the M5 simulator
    Copyright (C) 2009, Stanford University

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/



#ifndef __PTHREAD_DEFS_H__
#define __PTHREAD_DEFS_H__

// non-linux definitions... fill this in?
#ifndef __linux__
  #error "Non-Linux pthread definitions not available"
#endif //!__linux__

#ifndef __GNUC__
#error "This library requires gcc 4.0+ (3.x should work, but you'll need to change pthread_defs.h)"
#elif __GNUC__ < 4
#error "This library requires gcc 4.0+ (3.x should work, but you'll need to change pthread_defs.h)"
#endif

#ifndef  __SIZEOF_PTHREAD_MUTEX_T
#error "This library requires __SIZEOF_PTHREAD_MUTEX_T"
#endif

/* Read-write locks.  */
typedef union {
  struct
  {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;
# ifdef __x86_64__
# define __PTHREAD_RWLOCK_INT_FLAGS_SHARED	1
    int __writer;
    int __shared;
    signed char __rwelision;
# ifndef  __ILP32__
    unsigned char __pad[4];
#    ifndef __PTHREAD_RWLOCK_ELISION_EXTRA
#      define __PTHREAD_RWLOCK_ELISION_EXTRA 0, { 0, 0, 0, 0 }, { 0, 0, 0 }
#    endif
#  else
#    ifndef __PTHREAD_RWLOCK_ELISION_EXTRA
#      define __PTHREAD_RWLOCK_ELISION_EXTRA 0, { 0, 0, 0 }
#    endif
#  endif
    unsigned char __pad1[3];
    unsigned long int __pad2;
    unsigned int __flags;
# else
    unsigned char __flags;
    unsigned char __shared;
    signed char __rwelision;
# define __PTHREAD_RWLOCK_ELISION_EXTRA 0
    unsigned char __pad2;
    int __writer;
# endif
  } __data;
  char __size[__SIZEOF_PTHREAD_RWLOCK_T];
  long int __align;
} m5_rwlock_t;

_Static_assert(sizeof(m5_rwlock_t) == __SIZEOF_PTHREAD_RWLOCK_T, "Incompatible rwlock struct sizes");

typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[__SIZEOF_PTHREAD_COND_T];
  __extension__ long long int __align;
} m5_cond_t;

_Static_assert(sizeof(m5_cond_t) == __SIZEOF_PTHREAD_COND_T, "Incompatible cond struct sizes");

/* Attribute for read-write locks.  */
typedef struct
{
  int __lockkind;
  int __pshared;
} m5_rwlockattr_t;

/* POSIX spinlock data type.  */
typedef volatile int m5_spinlock_t;

/* POSIX barrier. */
typedef union
{
  char __size[__SIZEOF_PTHREAD_BARRIER_T];
  long int __align;
  m5_spinlock_t __arr[__SIZEOF_PTHREAD_BARRIER_T / sizeof(m5_spinlock_t)];
} m5_barrier_t;

_Static_assert(sizeof(m5_barrier_t) == __SIZEOF_PTHREAD_BARRIER_T, "Incompatible barrier struct sizes");

/* barrier attribute */
typedef struct {
  int __pshared;
} m5_barrierattr_t;


#define PTHREAD_MUTEX_T_COUNT __data.__count

#define M5_SPINLOCK_T(integer)  (*(m5_spinlock_t*)&(integer))

#define M5_LOCK(rwlock) ((m5_rwlock_t*)rwlock)
#define PTHREAD_RWLOCK_T_LOCK(rwlock)  M5_SPINLOCK_T(M5_LOCK(rwlock)->__data.__lock)
#define PTHREAD_RWLOCK_T_READERS(rwlock)  M5_SPINLOCK_T(M5_LOCK(rwlock)->__data.__nr_readers)
#define PTHREAD_RWLOCK_T_WRITER(rwlock)  M5_SPINLOCK_T(M5_LOCK(rwlock)->__data.__writer)

#define M5_COND(cond) ((m5_cond_t*)cond)
#define PTHREAD_COND_T_FLAG(cond) M5_SPINLOCK_T(M5_COND(cond)->__data.__lock)
#define PTHREAD_COND_T_THREAD_COUNT(cond) M5_SPINLOCK_T(M5_COND(cond)-> __data.__futex)
#define PTHREAD_COND_T_COUNT_LOCK(cond) M5_SPINLOCK_T(M5_COND(cond)->__data.__nwaiters)

//For tree barriers
//#define PTHREAD_BARRIER_T_NUM_THREADS(barrier)  (*((int*)(barrier->__size+(0*sizeof(int)))))
//#define PTHREAD_BARRIER_T_BARRIER_PTR(barrier) (*(tree_barrier_t**)(barrier->__size+(1*sizeof(int))))

#define M5_BARRIER(barrier) ((m5_barrier_t*)barrier)
#define PTHREAD_BARRIER_T_SPINLOCK(barrier) (M5_BARRIER(barrier)->__arr[0])
#define PTHREAD_BARRIER_T_NUM_THREADS(barrier) (M5_BARRIER(barrier)->__arr[1])
#define PTHREAD_BARRIER_T_COUNTER(barrier) (M5_BARRIER(barrier)->__arr[2])
#define PTHREAD_BARRIER_T_DIRECTION(barrier) (M5_BARRIER(barrier)->__arr[3])

#endif //  __PTHREAD_DEFS_H__
