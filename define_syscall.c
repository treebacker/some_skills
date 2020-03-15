#include <stdio.h>
#include <stdlib.h>
#include <asm/unistd_64.h>
#include <sys/types.h>


#define __syscall3(type,name,type1,arg1,type2,arg2,type3,arg3) \
type name(type1 arg1,type2 arg2,type3 arg3) \
{ \
    long ret; \
    __asm__ volatile (      \
        "mov $2, %%rax\n" \
        "syscall" : : "g"(arg1), "g"(arg2), "g"(arg3))\
                     \
    asm("mov %%rax, %0"):"=a"(ret); \
    return (type)ret; \
}

__syscall3(size_t, write, int, fd, const void *, buf, size_t, count);
//查看man syscall可以看到，x86-64和32，发现参数的顺序思rdi rsi rdi r10 r8 r9这样的顺序，不能变。
//syscall指令的调用指令是rax，返回值也是rax，这样一切就都说通了。
 
int _start(){
    write(1, "I am payload who has hijacked your process!\n", 48);
    return 0;
}