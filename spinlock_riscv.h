#ifndef __SPINLOCK_RISCV_H__
#define __SPINLOCK_RISCV_H__

// Based on sample code from "RISC-V User-Level ISA" V2.2, par 7.3

static __inline__ void spin_lock (volatile int *lock) {
	int tmp;
	__asm__ __volatile__ (
"1:	amoswap.w.aq %0, %1, (%2) \n\t" \
"	bnez %0, 1b \n\t"
	: "=r"(tmp)
	: "0"(1), "r" (lock)
	: "memory"
	);
}

static __inline__ void spin_unlock(volatile int *lock) {
	__asm__ __volatile__ (
"	amoswap.w.rl zero, zero, (%0) \n\t"
	:
	: "r" (lock)
	: "memory"
	);
}

static __inline__ int trylock (volatile int *lock) {
	int tmp;
	__asm__ __volatile__ (
"	amoswap.w.aq %0, %1, (%2) \n\t"
	: "=r"(tmp)
	: "0"(1), "r" (lock)
	: "memory"
	);
	return tmp == 0;
}

#endif
