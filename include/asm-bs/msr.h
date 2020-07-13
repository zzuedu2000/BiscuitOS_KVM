#ifndef _BISCUITOS_MSR_H
#define _BISCUITOS_MSR_H

#define MSR_IA32_SYSENTER_CS_BS		0x174
#define MSR_IA32_SYSENTER_ESP_BS	0x175
#define MSR_IA32_SYSENTER_EIP_BS	0x176

#define MSR_IA32_DEBUGCTLMSR_BS		0x1d9
#define MSR_IA32_LASTBRANCHFROMIP_BS	0x1db
#define MSR_IA32_LASTBRANCHTOIP_BS	0x1dc
#define MSR_IA32_LASTINTFROMIP_BS	0x1dd
#define MSR_IA32_LASTINTTOIP_BS		0x1de

/* AMD Defined MSRs */
#define MSR_K6_STAR_BS			0xC0000081

/*
 * Access to machine-specific registers (available on 586 and better only)
 * Note: the rd* operations modify the parameters directly (without using
 * pointer indirection), this allows gcc to optimize better
 */

#define rdmsr_bs(msr,val1,val2)					\
	__asm__ __volatile__("rdmsr"				\
			: "=a" (val1), "=d" (val2)		\
			: "c" (msr))

#define rdmsrl_bs(msr,val)					\
do {								\
	unsigned long l__,h__;					\
	rdmsr_bs(msr, l__, h__);				\
	val = l__;						\
	val |= ((u64)h__ << 32);				\
} while (0)

/* rdmsr with exception handling */
#define rdmsr_safe_bs(msr,a,b)					\
({								\
	int ret__;						\
								\
	asm volatile("2: rdmsr\n\r"				\
		     "	 xorl %0, %0\n\r"			\
		     "1:\n\r"					\
		     ".section .fixup,\"ax\"\n\r"		\
		     "3: movl %4, %0\n\r"			\
		     "   jmp 1b\n\r"				\
		     ".previous\n\r"				\
		     ".section __ex_table,\"a\"\n\r"		\
		     "   .align 4\n\r"				\
		     "   .long 2b, 3b\n\r"			\
		     ".previous"				\
		     : "=r" (ret__), "=a" (*(a)), "=d" (*(b))	\
		     : "c" (msr), "i" (-EFAULT));		\
	ret__;							\
})

#endif
