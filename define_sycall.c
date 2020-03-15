#include <stdio.h>
#include <asm/unistd_64.h>
#include <sys/types.h>


#define __syscall1(type,name,type1,arg1) \
type name(type1 arg1)	\
{ \
	long ret;	\
	__asm__ volatile (		\
		"syscall"	::	"a"	(__NR_##name), "g"((type1)(arg1)));\
		asm("mov %%rax, %0" : "=r"(ret));	\
	return (type)ret;	\
}

#define __syscall2(type,name,type1,arg1, type2, arg2, type3, arg3) \
type name(type1 arg1, type2 arg2)	\
{ \
	long ret;	\
	__asm__ volatile (		\
		"syscall"	::	"a"	(__NR_##name), "g"((type1)(arg1)), "g"((type2)(arg2)));\
		asm("mov %%rax, %0" : "=r"(ret));	\
	return (type)ret;	\
}

#define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
    long ret; \
    __asm__ volatile (      \
        "syscall" :: "a" (__NR_##name), "g"((type1)(arg1)), "g"((type2)(arg2)), \
        			"g"((type3)(arg3)));\
    asm("mov %%rax, %0" : "=r"(ret));                 \
    return (type)ret; \
}

#define __syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name(type1 arg1,type2 arg2,type3 arg3,arg4) \
{ \
    long ret; \
    __asm__ volatile (      \
        "syscall" :: "a" (__NR_##name), "g"((type1)(arg1)), "g"((type2)(arg2)),	\
        			 "g"((type3)(arg3)), "g"((type4)(arg4)));\
    asm("mov %%rax, %0" : "=r"(ret));                 \
    return (type)ret; \
}

#define __syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5) \
type name(type1 arg1,type2 arg2,type3 arg3,arg4,arg5) \
{ \
    long ret; \
    __asm__ volatile (      \
        "syscall" :: "a" (__NR_##name), "g"((type1)(arg1)), "g"((type2)(arg2)),	\
        			 "g"((type3)(arg3)), "g"((type4)(arg4)), "g"((type5)(arg5)));\
    asm("mov %%rax, %0" : "=r"(ret));                 \
    return (type)ret; \
}

#define __syscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5, type6, arg6) \
type name(type1 arg1,type2 arg2,type3 arg3,arg4,arg5,arg6) \
{ \
    long ret; \
    __asm__ volatile (      \
        "syscall" :: "a" (__NR_##name), "g"((type1)(arg1)), "g"((type2)(arg2)),	\
        			 "g"((type3)(arg3)), "g"((type4)(arg4)), "g"((type5)(arg5)),	\
        			 "g"((type6)(arg6)));\
    asm("mov %%rax, %0" : "=r"(ret));                 \
    return (type)ret; \
}

__syscall1(ssize_t, exit, int, status);
__syscall3(ssize_t, write, int, fd, const void *, buf, size_t, count);
//查看man syscall可以看到，x86-64和32，发现参数的顺序思rdi rsi rdi r10 r8 r9这样的顺序，不能变。
//syscall指令的调用指令是rax，返回值也是rax，这样一切就都说通了。
 
void _start(){

    write(1, "I am payload who has hijacked your process!\n", 48);
    exit(0);
}