#include "../include/bls12_381.hpp"

namespace bls12_381
{

#ifdef __x86_64__
void _add(fp* z, const fp* x, const fp* y)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // y => %rdx
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("push %r15;");
    asm("push %r14;");
    asm("push %r13;");
    asm("push %r12;");
    asm("push %rbx;");
    asm("mov     (%rsi),%r8;");
    asm("mov 0x08(%rsi),%r9;");
    asm("mov 0x10(%rsi),%r10;");
    asm("mov 0x18(%rsi),%r11;");
    asm("mov 0x20(%rsi),%r12;");
    asm("mov 0x28(%rsi),%r13;");
    asm("add     (%rdx),%r8;");
    asm("adc 0x08(%rdx),%r9;");
    asm("adc 0x10(%rdx),%r10;");
    asm("adc 0x18(%rdx),%r11;");
    asm("adc 0x20(%rdx),%r12;");
    asm("adc 0x28(%rdx),%r13;");
    asm("mov %r8,%r14;");
    asm("mov %r9,%r15;");
    asm("mov %r10,%rcx;");
    asm("mov %r11,%rdx;");
    asm("mov %r12,%rsi;");
    asm("mov %r13,%rbx;");
    asm("mov $0xb9feffffffffaaab,%rax;");
    asm("sub %rax,%r14;");
    asm("mov $0x1eabfffeb153ffff,%rax;");
    asm("sbb %rax,%r15;");
    asm("mov $0x6730d2a0f6b0f624,%rax;");
    asm("sbb %rax,%rcx;");
    asm("mov $0x64774b84f38512bf,%rax;");
    asm("sbb %rax,%rdx;");
    asm("mov $0x4b1ba7b6434bacd7,%rax;");
    asm("sbb %rax,%rsi;");
    asm("mov $0x1a0111ea397fe69a,%rax;");
    asm("sbb %rax,%rbx;");
    asm("cmovae %r14,%r8;");
    asm("cmovae %r15,%r9;");
    asm("cmovae %rcx,%r10;");
    asm("cmovae %rdx,%r11;");
    asm("cmovae %rsi,%r12;");
    asm("cmovae %rbx,%r13;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %r12,0x20(%rdi);");
    asm("mov %r13,0x28(%rdi);");
    asm("pop %rbx;");
    asm("pop %r12;");
    asm("pop %r13;");
    asm("pop %r14;");
    asm("pop %r15;");
}
#else
void _add(fp* z, const fp* x, const fp* y)
{
    uint64_t carry, _;

    tie(z->d[0], carry) = Add64(x->d[0], y->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], y->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], y->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], y->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], y->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], y->d[5], carry);

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}
#endif

#ifdef __x86_64__
void _addAssign(fp* x, const fp* y)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // x => %rdi
    // y => %rsi
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("push %r15;");
    asm("push %r14;");
    asm("push %r13;");
    asm("push %r12;");
    asm("push %rbx;");
    asm("mov     (%rdi),%r8;");
    asm("mov 0x08(%rdi),%r9;");
    asm("mov 0x10(%rdi),%r10;");
    asm("mov 0x18(%rdi),%r11;");
    asm("mov 0x20(%rdi),%r12;");
    asm("mov 0x28(%rdi),%r13;");
    asm("add     (%rsi),%r8;");
    asm("adc 0x08(%rsi),%r9;");
    asm("adc 0x10(%rsi),%r10;");
    asm("adc 0x18(%rsi),%r11;");
    asm("adc 0x20(%rsi),%r12;");
    asm("adc 0x28(%rsi),%r13;");
    asm("mov %r8,%r14;");
    asm("mov %r9,%r15;");
    asm("mov %r10,%rcx;");
    asm("mov %r11,%rdx;");
    asm("mov %r12,%rsi;");
    asm("mov %r13,%rbx;");
    asm("mov $0xb9feffffffffaaab,%rax;");
    asm("sub %rax,%r14;");
    asm("mov $0x1eabfffeb153ffff,%rax;");
    asm("sbb %rax,%r15;");
    asm("mov $0x6730d2a0f6b0f624,%rax;");
    asm("sbb %rax,%rcx;");
    asm("mov $0x64774b84f38512bf,%rax;");
    asm("sbb %rax,%rdx;");
    asm("mov $0x4b1ba7b6434bacd7,%rax;");
    asm("sbb %rax,%rsi;");
    asm("mov $0x1a0111ea397fe69a,%rax;");
    asm("sbb %rax,%rbx;");
    asm("cmovae %r14,%r8;");
    asm("cmovae %r15,%r9;");
    asm("cmovae %rcx,%r10;");
    asm("cmovae %rdx,%r11;");
    asm("cmovae %rsi,%r12;");
    asm("cmovae %rbx,%r13;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %r12,0x20(%rdi);");
    asm("mov %r13,0x28(%rdi);");
    asm("pop %rbx;");
    asm("pop %r12;");
    asm("pop %r13;");
    asm("pop %r14;");
    asm("pop %r15;");
}
#else
void _addAssign(fp* x, const fp* y)
{
    uint64_t carry, _;

    tie(x->d[0], carry) = Add64(x->d[0], y->d[0], 0);
    tie(x->d[1], carry) = Add64(x->d[1], y->d[1], carry);
    tie(x->d[2], carry) = Add64(x->d[2], y->d[2], carry);
    tie(x->d[3], carry) = Add64(x->d[3], y->d[3], carry);
    tie(x->d[4], carry) = Add64(x->d[4], y->d[4], carry);
    tie(x->d[5], _)     = Add64(x->d[5], y->d[5], carry);

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(x->d[5] < fp::MODULUS.d[5] || (x->d[5] == fp::MODULUS.d[5] && (x->d[4] < fp::MODULUS.d[4] || (x->d[4] == fp::MODULUS.d[4] && (x->d[3] < fp::MODULUS.d[3] || (x->d[3] == fp::MODULUS.d[3] && (x->d[2] < fp::MODULUS.d[2] || (x->d[2] == fp::MODULUS.d[2] && (x->d[1] < fp::MODULUS.d[1] || (x->d[1] == fp::MODULUS.d[1] && (x->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b;
        tie(x->d[0], b) = Sub64(x->d[0], fp::MODULUS.d[0], 0);
        tie(x->d[1], b) = Sub64(x->d[1], fp::MODULUS.d[1], b);
        tie(x->d[2], b) = Sub64(x->d[2], fp::MODULUS.d[2], b);
        tie(x->d[3], b) = Sub64(x->d[3], fp::MODULUS.d[3], b);
        tie(x->d[4], b) = Sub64(x->d[4], fp::MODULUS.d[4], b);
        tie(x->d[5], _) = Sub64(x->d[5], fp::MODULUS.d[5], b);
    }
}
#endif

#ifdef __x86_64__
void _ladd(fp* z, const fp* x, const fp* y)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // y => %rdx
    // callee needs to restore registers r15, r14, r13, r12, rbx (if clobbed) before returning
    asm("mov     (%rsi),%r8;");
    asm("mov 0x08(%rsi),%r9;");
    asm("mov 0x10(%rsi),%r10;");
    asm("mov 0x18(%rsi),%r11;");
    asm("mov 0x20(%rsi),%rcx;");
    asm("mov 0x28(%rsi),%rax;");
    asm("add     (%rdx),%r8;");
    asm("adc 0x08(%rdx),%r9;");
    asm("adc 0x10(%rdx),%r10;");
    asm("adc 0x18(%rdx),%r11;");
    asm("adc 0x20(%rdx),%rcx;");
    asm("adc 0x28(%rdx),%rax;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %rcx,0x20(%rdi);");
    asm("mov %rax,0x28(%rdi);");
}
#else
void _ladd(fp* z, const fp* x, const fp* y)
{
    uint64_t carry, _;
    tie(z->d[0], carry) = Add64(x->d[0], y->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], y->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], y->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], y->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], y->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], y->d[5], carry);
}
#endif

#ifdef __x86_64__
void _laddAssign(fp* x, const fp* y)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // x => %rdi
    // y => %rsi
    // callee needs to restore registers r15, r14, r13, r12, rbx (if clobbed) before returning
    asm("mov     (%rdi),%r8;");
    asm("mov 0x08(%rdi),%r9;");
    asm("mov 0x10(%rdi),%r10;");
    asm("mov 0x18(%rdi),%r11;");
    asm("mov 0x20(%rdi),%rcx;");
    asm("mov 0x28(%rdi),%rax;");
    asm("add     (%rsi),%r8;");
    asm("adc 0x08(%rsi),%r9;");
    asm("adc 0x10(%rsi),%r10;");
    asm("adc 0x18(%rsi),%r11;");
    asm("adc 0x20(%rsi),%rcx;");
    asm("adc 0x28(%rsi),%rax;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %rcx,0x20(%rdi);");
    asm("mov %rax,0x28(%rdi);");
}
#else
void _laddAssign(fp* x, const fp* y)
{
    uint64_t carry, _;
    tie(x->d[0], carry) = Add64(x->d[0], y->d[0], 0);
    tie(x->d[1], carry) = Add64(x->d[1], y->d[1], carry);
    tie(x->d[2], carry) = Add64(x->d[2], y->d[2], carry);
    tie(x->d[3], carry) = Add64(x->d[3], y->d[3], carry);
    tie(x->d[4], carry) = Add64(x->d[4], y->d[4], carry);
    tie(x->d[5], _)     = Add64(x->d[5], y->d[5], carry);
}
#endif

#ifdef __x86_64__
void _double(fp* z, const fp* x)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("push %r15;");
    asm("push %r14;");
    asm("push %r13;");
    asm("push %r12;");
    asm("push %rbx;");
    asm("mov     (%rsi),%r8;");
    asm("mov 0x08(%rsi),%r9;");
    asm("mov 0x10(%rsi),%r10;");
    asm("mov 0x18(%rsi),%r11;");
    asm("mov 0x20(%rsi),%r12;");
    asm("mov 0x28(%rsi),%r13;");
    asm("add %r8,%r8;");
    asm("adc %r9,%r9;");
    asm("adc %r10,%r10;");
    asm("adc %r11,%r11;");
    asm("adc %r12,%r12;");
    asm("adc %r13,%r13;");
    asm("mov %r8,%r14;");
    asm("mov %r9,%r15;");
    asm("mov %r10,%rcx;");
    asm("mov %r11,%rdx;");
    asm("mov %r12,%rsi;");
    asm("mov %r13,%rbx;");
    asm("mov $0xb9feffffffffaaab,%rax;");
    asm("sub %rax,%r14;");
    asm("mov $0x1eabfffeb153ffff,%rax;");
    asm("sbb %rax,%r15;");
    asm("mov $0x6730d2a0f6b0f624,%rax;");
    asm("sbb %rax,%rcx;");
    asm("mov $0x64774b84f38512bf,%rax;");
    asm("sbb %rax,%rdx;");
    asm("mov $0x4b1ba7b6434bacd7,%rax;");
    asm("sbb %rax,%rsi;");
    asm("mov $0x1a0111ea397fe69a,%rax;");
    asm("sbb %rax,%rbx;");
    asm("cmovae %r14,%r8;");
    asm("cmovae %r15,%r9;");
    asm("cmovae %rcx,%r10;");
    asm("cmovae %rdx,%r11;");
    asm("cmovae %rsi,%r12;");
    asm("cmovae %rbx,%r13;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %r12,0x20(%rdi);");
    asm("mov %r13,0x28(%rdi);");
    asm("pop %rbx;");
    asm("pop %r12;");
    asm("pop %r13;");
    asm("pop %r14;");
    asm("pop %r15;");
}
#else
void _double(fp* z, const fp* x)
{
    uint64_t carry, _;

    tie(z->d[0], carry) = Add64(x->d[0], x->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], x->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], x->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], x->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], x->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], x->d[5], carry);

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}
#endif

#ifdef __x86_64__
void _doubleAssign(fp* z)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("push %r15;");
    asm("push %r14;");
    asm("push %r13;");
    asm("push %r12;");
    asm("push %rbx;");
    asm("mov     (%rdi),%r8;");
    asm("mov 0x08(%rdi),%r9;");
    asm("mov 0x10(%rdi),%r10;");
    asm("mov 0x18(%rdi),%r11;");
    asm("mov 0x20(%rdi),%r12;");
    asm("mov 0x28(%rdi),%r13;");
    asm("add %r8,%r8;");
    asm("adc %r9,%r9;");
    asm("adc %r10,%r10;");
    asm("adc %r11,%r11;");
    asm("adc %r12,%r12;");
    asm("adc %r13,%r13;");
    asm("mov %r8,%r14;");
    asm("mov %r9,%r15;");
    asm("mov %r10,%rcx;");
    asm("mov %r11,%rdx;");
    asm("mov %r12,%rsi;");
    asm("mov %r13,%rbx;");
    asm("mov $0xb9feffffffffaaab,%rax;");
    asm("sub %rax,%r14;");
    asm("mov $0x1eabfffeb153ffff,%rax;");
    asm("sbb %rax,%r15;");
    asm("mov $0x6730d2a0f6b0f624,%rax;");
    asm("sbb %rax,%rcx;");
    asm("mov $0x64774b84f38512bf,%rax;");
    asm("sbb %rax,%rdx;");
    asm("mov $0x4b1ba7b6434bacd7,%rax;");
    asm("sbb %rax,%rsi;");
    asm("mov $0x1a0111ea397fe69a,%rax;");
    asm("sbb %rax,%rbx;");
    asm("cmovae %r14,%r8;");
    asm("cmovae %r15,%r9;");
    asm("cmovae %rcx,%r10;");
    asm("cmovae %rdx,%r11;");
    asm("cmovae %rsi,%r12;");
    asm("cmovae %rbx,%r13;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %r12,0x20(%rdi);");
    asm("mov %r13,0x28(%rdi);");
    asm("pop %rbx;");
    asm("pop %r12;");
    asm("pop %r13;");
    asm("pop %r14;");
    asm("pop %r15;");
}
#else
void _doubleAssign(fp* z)
{
    uint64_t carry, _;

    tie(z->d[0], carry) = Add64(z->d[0], z->d[0], 0);
    tie(z->d[1], carry) = Add64(z->d[1], z->d[1], carry);
    tie(z->d[2], carry) = Add64(z->d[2], z->d[2], carry);
    tie(z->d[3], carry) = Add64(z->d[3], z->d[3], carry);
    tie(z->d[4], carry) = Add64(z->d[4], z->d[4], carry);
    tie(z->d[5], _)     = Add64(z->d[5], z->d[5], carry);

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}
#endif

#ifdef __x86_64__
void _ldouble(fp* z, const fp* x)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // callee needs to restore registers r15, r14, r13, r12, rbx (if clobbed) before returning
    asm("mov     (%rsi),%r8;");
    asm("mov 0x08(%rsi),%r9;");
    asm("mov 0x10(%rsi),%r10;");
    asm("mov 0x18(%rsi),%r11;");
    asm("mov 0x20(%rsi),%rcx;");
    asm("mov 0x28(%rsi),%rax;");
    asm("add %r8,%r8;");
    asm("adc %r9,%r9;");
    asm("adc %r10,%r10;");
    asm("adc %r11,%r11;");
    asm("adc %rcx,%rcx;");
    asm("adc %rax,%rax;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %rcx,0x20(%rdi);");
    asm("mov %rax,0x28(%rdi);");
}
#else
void _ldouble(fp* z, const fp* x)
{
    uint64_t carry, _;

    tie(z->d[0], carry) = Add64(x->d[0], x->d[0], 0);
    tie(z->d[1], carry) = Add64(x->d[1], x->d[1], carry);
    tie(z->d[2], carry) = Add64(x->d[2], x->d[2], carry);
    tie(z->d[3], carry) = Add64(x->d[3], x->d[3], carry);
    tie(z->d[4], carry) = Add64(x->d[4], x->d[4], carry);
    tie(z->d[5], _)     = Add64(x->d[5], x->d[5], carry);
}
#endif

#ifdef __x86_64__
void _sub(fp* z, const fp* x, const fp* y)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // y => %rdx
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("push %r15;");
    asm("push %r14;");
    asm("push %r13;");
    asm("push %r12;");
    asm("push %rbx;");
    asm("xor %rax,%rax;");
    asm("mov     (%rsi),%r8;");
    asm("mov 0x08(%rsi),%r9;");
    asm("mov 0x10(%rsi),%r10;");
    asm("mov 0x18(%rsi),%r11;");
    asm("mov 0x20(%rsi),%r12;");
    asm("mov 0x28(%rsi),%r13;");
    asm("sub     (%rdx),%r8;");
    asm("sbb 0x08(%rdx),%r9;");
    asm("sbb 0x10(%rdx),%r10;");
    asm("sbb 0x18(%rdx),%r11;");
    asm("sbb 0x20(%rdx),%r12;");
    asm("sbb 0x28(%rdx),%r13;");
    asm("mov $0xb9feffffffffaaab,%r14;");
    asm("mov $0x1eabfffeb153ffff,%r15;");
    asm("mov $0x6730d2a0f6b0f624,%rcx;");
    asm("mov $0x64774b84f38512bf,%rdx;");
    asm("mov $0x4b1ba7b6434bacd7,%rsi;");
    asm("mov $0x1a0111ea397fe69a,%rbx;");
    asm("cmovae %rax,%r14;");
    asm("cmovae %rax,%r15;");
    asm("cmovae %rax,%rcx;");
    asm("cmovae %rax,%rdx;");
    asm("cmovae %rax,%rsi;");
    asm("cmovae %rax,%rbx;");
    asm("add %r14,%r8;");
    asm("adc %r15,%r9;");
    asm("adc %rcx,%r10;");
    asm("adc %rdx,%r11;");
    asm("adc %rsi,%r12;");
    asm("adc %rbx,%r13;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %r12,0x20(%rdi);");
    asm("mov %r13,0x28(%rdi);");
    asm("pop %rbx;");
    asm("pop %r12;");
    asm("pop %r13;");
    asm("pop %r14;");
    asm("pop %r15;");
}
#else
void _sub(fp* z, const fp* x, const fp* y)
{
    uint64_t b;
    tie(z->d[0], b) = Sub64(x->d[0], y->d[0], 0);
    tie(z->d[1], b) = Sub64(x->d[1], y->d[1], b);
    tie(z->d[2], b) = Sub64(x->d[2], y->d[2], b);
    tie(z->d[3], b) = Sub64(x->d[3], y->d[3], b);
    tie(z->d[4], b) = Sub64(x->d[4], y->d[4], b);
    tie(z->d[5], b) = Sub64(x->d[5], y->d[5], b);
    if(b != 0)
    {
        uint64_t c, _;
        tie(z->d[0], c) = Add64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], c) = Add64(z->d[1], fp::MODULUS.d[1], c);
        tie(z->d[2], c) = Add64(z->d[2], fp::MODULUS.d[2], c);
        tie(z->d[3], c) = Add64(z->d[3], fp::MODULUS.d[3], c);
        tie(z->d[4], c) = Add64(z->d[4], fp::MODULUS.d[4], c);
        tie(z->d[5], _) = Add64(z->d[5], fp::MODULUS.d[5], c);
    }
}
#endif

#ifdef __x86_64__
void _subAssign(fp* z, const fp* x)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("push %r15;");
    asm("push %r14;");
    asm("push %r13;");
    asm("push %r12;");
    asm("push %rbx;");
    asm("xor %rax,%rax;");
    asm("mov     (%rdi),%r8;");
    asm("mov 0x08(%rdi),%r9;");
    asm("mov 0x10(%rdi),%r10;");
    asm("mov 0x18(%rdi),%r11;");
    asm("mov 0x20(%rdi),%r12;");
    asm("mov 0x28(%rdi),%r13;");
    asm("sub     (%rsi),%r8;");
    asm("sbb 0x08(%rsi),%r9;");
    asm("sbb 0x10(%rsi),%r10;");
    asm("sbb 0x18(%rsi),%r11;");
    asm("sbb 0x20(%rsi),%r12;");
    asm("sbb 0x28(%rsi),%r13;");
    asm("mov $0xb9feffffffffaaab,%r14;");
    asm("mov $0x1eabfffeb153ffff,%r15;");
    asm("mov $0x6730d2a0f6b0f624,%rcx;");
    asm("mov $0x64774b84f38512bf,%rdx;");
    asm("mov $0x4b1ba7b6434bacd7,%rsi;");
    asm("mov $0x1a0111ea397fe69a,%rbx;");
    asm("cmovae %rax,%r14;");
    asm("cmovae %rax,%r15;");
    asm("cmovae %rax,%rcx;");
    asm("cmovae %rax,%rdx;");
    asm("cmovae %rax,%rsi;");
    asm("cmovae %rax,%rbx;");
    asm("add %r14,%r8;");
    asm("adc %r15,%r9;");
    asm("adc %rcx,%r10;");
    asm("adc %rdx,%r11;");
    asm("adc %rsi,%r12;");
    asm("adc %rbx,%r13;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %r12,0x20(%rdi);");
    asm("mov %r13,0x28(%rdi);");
    asm("pop %rbx;");
    asm("pop %r12;");
    asm("pop %r13;");
    asm("pop %r14;");
    asm("pop %r15;");
}
#else
void _subAssign(fp* z, const fp* x)
{
    uint64_t b;
    tie(z->d[0], b) = Sub64(z->d[0], x->d[0], 0);
    tie(z->d[1], b) = Sub64(z->d[1], x->d[1], b);
    tie(z->d[2], b) = Sub64(z->d[2], x->d[2], b);
    tie(z->d[3], b) = Sub64(z->d[3], x->d[3], b);
    tie(z->d[4], b) = Sub64(z->d[4], x->d[4], b);
    tie(z->d[5], b) = Sub64(z->d[5], x->d[5], b);
    if(b != 0)
    {
        uint64_t c, _;
        tie(z->d[0], c) = Add64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], c) = Add64(z->d[1], fp::MODULUS.d[1], c);
        tie(z->d[2], c) = Add64(z->d[2], fp::MODULUS.d[2], c);
        tie(z->d[3], c) = Add64(z->d[3], fp::MODULUS.d[3], c);
        tie(z->d[4], c) = Add64(z->d[4], fp::MODULUS.d[4], c);
        tie(z->d[5], _) = Add64(z->d[5], fp::MODULUS.d[5], c);
    }
}
#endif

#ifdef __x86_64__
void _lsubAssign(fp* z, const fp* x)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("mov     (%rdi),%r8;");
    asm("mov 0x08(%rdi),%r9;");
    asm("mov 0x10(%rdi),%r10;");
    asm("mov 0x18(%rdi),%r11;");
    asm("mov 0x20(%rdi),%rcx;");
    asm("mov 0x28(%rdi),%rax;");
    asm("sub     (%rsi),%r8;");
    asm("sbb 0x08(%rsi),%r9;");
    asm("sbb 0x10(%rsi),%r10;");
    asm("sbb 0x18(%rsi),%r11;");
    asm("sbb 0x20(%rsi),%rcx;");
    asm("sbb 0x28(%rsi),%rax;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %rcx,0x20(%rdi);");
    asm("mov %rax,0x28(%rdi);");
}
#else
void _lsubAssign(fp* z, const fp* x)
{
    uint64_t b, _;
    tie(z->d[0], b) = Sub64(z->d[0], x->d[0], 0);
    tie(z->d[1], b) = Sub64(z->d[1], x->d[1], b);
    tie(z->d[2], b) = Sub64(z->d[2], x->d[2], b);
    tie(z->d[3], b) = Sub64(z->d[3], x->d[3], b);
    tie(z->d[4], b) = Sub64(z->d[4], x->d[4], b);
    tie(z->d[5], _) = Sub64(z->d[5], x->d[5], b);
}
#endif

#ifdef __x86_64__
void __neg(fp* z, const fp* x)
{
    // x86_64 calling convention (https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI):
    // z => %rdi
    // x => %rsi
    // callee needs to restore registers r15, r14, r13, r12, rbx before returning
    asm("mov $0xb9feffffffffaaab,%r8;");
    asm("mov $0x1eabfffeb153ffff,%r9;");
    asm("mov $0x6730d2a0f6b0f624,%r10;");
    asm("mov $0x64774b84f38512bf,%r11;");
    asm("mov $0x4b1ba7b6434bacd7,%rcx;");
    asm("mov $0x1a0111ea397fe69a,%rax;");
    asm("sub     (%rsi),%r8;");
    asm("sbb 0x08(%rsi),%r9;");
    asm("sbb 0x10(%rsi),%r10;");
    asm("sbb 0x18(%rsi),%r11;");
    asm("sbb 0x20(%rsi),%rcx;");
    asm("sbb 0x28(%rsi),%rax;");
    asm("mov %r8,     (%rdi);");
    asm("mov %r9, 0x08(%rdi);");
    asm("mov %r10,0x10(%rdi);");
    asm("mov %r11,0x18(%rdi);");
    asm("mov %rcx,0x20(%rdi);");
    asm("mov %rax,0x28(%rdi);");
}
void _neg(fp* z, const fp* x)
{
    if(x->isZero())
    {
        *z = *x;
        return;
    }
    __neg(z, x);
}
#else
void _neg(fp* z, const fp* x)
{
    if(x->isZero())
    {
        *z = *x;
        return;
    }
    uint64_t borrow, _;
    tie(z->d[0], borrow) = Sub64(fp::MODULUS.d[0], x->d[0], 0);
    tie(z->d[1], borrow) = Sub64(fp::MODULUS.d[1], x->d[1], borrow);
    tie(z->d[2], borrow) = Sub64(fp::MODULUS.d[2], x->d[2], borrow);
    tie(z->d[3], borrow) = Sub64(fp::MODULUS.d[3], x->d[3], borrow);
    tie(z->d[4], borrow) = Sub64(fp::MODULUS.d[4], x->d[4], borrow);
    tie(z->d[5], _)      = Sub64(fp::MODULUS.d[5], x->d[5], borrow);
}
#endif

void _mul(fp* z, const fp* x, const fp* y)
{
    array<uint64_t, 6> t;
    array<uint64_t, 3> c;
    {
        // round 0
        uint64_t v = x->d[0];
        tie(c[1], c[0]) = Mul64(v, y->d[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[1], c[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[2], c[1]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[3], c[1]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[4], c[1]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd1(v, y->d[5], c[1]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 1
        uint64_t v = x->d[1];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 2
        uint64_t v = x->d[2];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 3
        uint64_t v = x->d[3];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 4
        uint64_t v = x->d[4];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], t[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], t[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], t[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], t[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0]) = madd2(v, y->d[5], c[1], t[5]);
        tie(t[5], t[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }
    {
        // round 5
        uint64_t v = x->d[5];
        tie(c[1], c[0]) = madd1(v, y->d[0], t[0]);
        uint64_t m = c[0] * fp::INP;
        c[2] = madd0(m, fp::MODULUS.d[0], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[1], c[1], t[1]);
        tie(c[2], z->d[0]) = madd2(m, fp::MODULUS.d[1], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[2], c[1], t[2]);
        tie(c[2], z->d[1]) = madd2(m, fp::MODULUS.d[2], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[3], c[1], t[3]);
        tie(c[2], z->d[2]) = madd2(m, fp::MODULUS.d[3], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[4], c[1], t[4]);
        tie(c[2], z->d[3]) = madd2(m, fp::MODULUS.d[4], c[2], c[0]);
        tie(c[1], c[0])    = madd2(v, y->d[5], c[1], t[5]);
        tie(z->d[5], z->d[4]) = madd3(m, fp::MODULUS.d[5], c[0], c[2], c[1]);
    }

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b, _;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}

void _square(fp* z, const fp* x)
{
    array<uint64_t, 6> p;
    uint64_t u, v;
    {
        // round 0
        tie(u, p[0]) = Mul64(x->d[0], x->d[0]);
        uint64_t m = p[0] * fp::INP;
        uint64_t C = madd0(m, fp::MODULUS.d[0], p[0]);
        uint64_t t, _;
        tie(t, u, v) = madd1sb(x->d[0], x->d[1], u);
        tie(C, p[0]) = madd2(m, fp::MODULUS.d[1], v, C);
        tie(t, u, v) = madd1s(x->d[0], x->d[2], t, u);
        tie(C, p[1]) = madd2(m, fp::MODULUS.d[2], v, C);
        tie(t, u, v) = madd1s(x->d[0], x->d[3], t, u);
        tie(C, p[2]) = madd2(m, fp::MODULUS.d[3], v, C);
        tie(t, u, v) = madd1s(x->d[0], x->d[4], t, u);
        tie(C, p[3]) = madd2(m, fp::MODULUS.d[4], v, C);
        tie(_, u, v) = madd1s(x->d[0], x->d[5], t, u);
        tie(p[5], p[4]) = madd3(m, fp::MODULUS.d[5], v, C, u);
    }
    {
        // round 1
        uint64_t m = p[0] * fp::INP;
        uint64_t C = madd0(m, fp::MODULUS.d[0], p[0]);
        tie(u, v) = madd1(x->d[1], x->d[1], p[1]);
        tie(C, p[0]) = madd2(m, fp::MODULUS.d[1], v, C);
        uint64_t t, _;
        tie(t, u, v) = madd2sb(x->d[1], x->d[2], p[2], u);
        tie(C, p[1]) = madd2(m, fp::MODULUS.d[2], v, C);
        tie(t, u, v) = madd2s(x->d[1], x->d[3], p[3], t, u);
        tie(C, p[2]) = madd2(m, fp::MODULUS.d[3], v, C);
        tie(t, u, v) = madd2s(x->d[1], x->d[4], p[4], t, u);
        tie(C, p[3]) = madd2(m, fp::MODULUS.d[4], v, C);
        tie(_, u, v) = madd2s(x->d[1], x->d[5], p[5], t, u);
        tie(p[5], p[4]) = madd3(m, fp::MODULUS.d[5], v, C, u);
    }
    {
        // round 2
        uint64_t m = p[0] * fp::INP;
        uint64_t C = madd0(m, fp::MODULUS.d[0], p[0]);
        tie(C, p[0]) = madd2(m, fp::MODULUS.d[1], p[1], C);
        tie(u, v) = madd1(x->d[2], x->d[2], p[2]);
        tie(C, p[1]) = madd2(m, fp::MODULUS.d[2], v, C);
        uint64_t t, _;
        tie(t, u, v) = madd2sb(x->d[2], x->d[3], p[3], u);
        tie(C, p[2]) = madd2(m, fp::MODULUS.d[3], v, C);
        tie(t, u, v) = madd2s(x->d[2], x->d[4], p[4], t, u);
        tie(C, p[3]) = madd2(m, fp::MODULUS.d[4], v, C);
        tie(_, u, v) = madd2s(x->d[2], x->d[5], p[5], t, u);
        tie(p[5], p[4]) = madd3(m, fp::MODULUS.d[5], v, C, u);
    }
    {
        // round 3
        uint64_t m = p[0] * fp::INP;
        uint64_t C = madd0(m, fp::MODULUS.d[0], p[0]);
        tie(C, p[0]) = madd2(m, fp::MODULUS.d[1], p[1], C);
        tie(C, p[1]) = madd2(m, fp::MODULUS.d[2], p[2], C);
        tie(u, v) = madd1(x->d[3], x->d[3], p[3]);
        tie(C, p[2]) = madd2(m, fp::MODULUS.d[3], v, C);
        uint64_t t, _;
        tie(t, u, v) = madd2sb(x->d[3], x->d[4], p[4], u);
        tie(C, p[3]) = madd2(m, fp::MODULUS.d[4], v, C);
        tie(_, u, v) = madd2s(x->d[3], x->d[5], p[5], t, u);
        tie(p[5], p[4]) = madd3(m, fp::MODULUS.d[5], v, C, u);
    }
    {
        // round 4
        uint64_t m = p[0] * fp::INP;
        uint64_t C = madd0(m, fp::MODULUS.d[0], p[0]);
        tie(C, p[0]) = madd2(m, fp::MODULUS.d[1], p[1], C);
        tie(C, p[1]) = madd2(m, fp::MODULUS.d[2], p[2], C);
        tie(C, p[2]) = madd2(m, fp::MODULUS.d[3], p[3], C);
        tie(u, v) = madd1(x->d[4], x->d[4], p[4]);
        tie(C, p[3]) = madd2(m, fp::MODULUS.d[4], v, C);
        uint64_t _;
        tie(_, u, v) = madd2sb(x->d[4], x->d[5], p[5], u);
        tie(p[5], p[4]) = madd3(m, fp::MODULUS.d[5], v, C, u);
    }
    {
        // round 5
        uint64_t m = p[0] * fp::INP;
        uint64_t C = madd0(m, fp::MODULUS.d[0], p[0]);
        tie(C, z->d[0]) = madd2(m, fp::MODULUS.d[1], p[1], C);
        tie(C, z->d[1]) = madd2(m, fp::MODULUS.d[2], p[2], C);
        tie(C, z->d[2]) = madd2(m, fp::MODULUS.d[3], p[3], C);
        tie(C, z->d[3]) = madd2(m, fp::MODULUS.d[4], p[4], C);
        tie(u, v) = madd1(x->d[5], x->d[5], p[5]);
        tie(z->d[5], z->d[4]) = madd3(m, fp::MODULUS.d[5], v, C, u);
    }

    // if z > q --> z -= q
    // note: this is NOT constant time
    if(!(z->d[5] < fp::MODULUS.d[5] || (z->d[5] == fp::MODULUS.d[5] && (z->d[4] < fp::MODULUS.d[4] || (z->d[4] == fp::MODULUS.d[4] && (z->d[3] < fp::MODULUS.d[3] || (z->d[3] == fp::MODULUS.d[3] && (z->d[2] < fp::MODULUS.d[2] || (z->d[2] == fp::MODULUS.d[2] && (z->d[1] < fp::MODULUS.d[1] || (z->d[1] == fp::MODULUS.d[1] && (z->d[0] < fp::MODULUS.d[0]))))))))))))
    {
        uint64_t b, _;
        tie(z->d[0], b) = Sub64(z->d[0], fp::MODULUS.d[0], 0);
        tie(z->d[1], b) = Sub64(z->d[1], fp::MODULUS.d[1], b);
        tie(z->d[2], b) = Sub64(z->d[2], fp::MODULUS.d[2], b);
        tie(z->d[3], b) = Sub64(z->d[3], fp::MODULUS.d[3], b);
        tie(z->d[4], b) = Sub64(z->d[4], fp::MODULUS.d[4], b);
        tie(z->d[5], _) = Sub64(z->d[5], fp::MODULUS.d[5], b);
    }
}

// Add64 returns the sum with carry of x, y and carry: sum = x + y + carry.
// The carry input must be 0 or 1; otherwise the behavior is undefined.
// The carryOut output is guaranteed to be 0 or 1.
//
// This function's execution time does not depend on the inputs.
tuple<uint64_t, uint64_t> Add64(
    const uint64_t& x,
    const uint64_t& y,
    const uint64_t& carry
)
{
    uint64_t sum = x + y + carry;
    // The sum will overflow if both top bits are set (x & y) or if one of them
    // is (x | y), and a carry from the lower place happened. If such a carry
    // happens, the top bit will be 1 + 0 + 1 = 0 (&^ sum).
    uint64_t carryOut = ((x & y) | ((x | y) & ~sum)) >> 63;
    return {sum, carryOut};
}

// Sub64 returns the difference of x, y and borrow: diff = x - y - borrow.
// The borrow input must be 0 or 1; otherwise the behavior is undefined.
// The borrowOut output is guaranteed to be 0 or 1.
//
// This function's execution time does not depend on the inputs.
tuple<uint64_t, uint64_t> Sub64(
    const uint64_t& x,
    const uint64_t& y,
    const uint64_t& borrow
)
{
    uint64_t diff, borrowOut;
    diff = x - y - borrow;
    // See Sub32 for the bit logic.
    borrowOut = ((~x & y) | (~(x ^ y) & diff)) >> 63;
    return {diff, borrowOut};
}

// Mul64 returns the 128-bit product of x and y: (hi, lo) = x * y
// with the product bits' upper half returned in hi and the lower
// half returned in lo.
//
// This function's execution time does not depend on the inputs.
tuple<uint64_t, uint64_t> Mul64(
    const uint64_t& x,
    const uint64_t& y
)
{
    uint128_t result = static_cast<uint128_t>(x) * static_cast<uint128_t>(y);
    uint64_t lo = result;
    uint64_t hi = result >> 64;
    return {hi, lo};
}

tuple<uint64_t, uint64_t, uint64_t> madd(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& t,
    const uint64_t& u,
    const uint64_t& v
)
{
    uint64_t hi, lo, carry, _v, _u, _t, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_v, carry) = Add64(lo, v, 0);
    tie(_u, carry) = Add64(hi, u, carry);
    tie(_t, _) = Add64(t, 0, carry);
    return {_t, _u, _v};
}

// madd0 hi = a*b + c (discards lo bits)
uint64_t madd0(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c
)
{
    uint64_t carry, hi, lo, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_, carry) = Add64(lo, c, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return hi;
}

// madd1 hi, lo = a*b + c
tuple<uint64_t, uint64_t> madd1(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c
)
{
    uint64_t carry, hi, lo, _;
    tie(hi, lo) = Mul64(a, b);
    tie(lo, carry) = Add64(lo, c, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return {hi, lo};
}

// madd2 hi, lo = a*b + c + d
tuple<uint64_t, uint64_t> madd2(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c,
    const uint64_t& d
)
{
    uint64_t carry, hi, lo, _c, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_c, carry) = Add64(c, d, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(lo, carry) = Add64(lo, _c, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return {hi, lo};
}

// madd2s superhi, hi, lo = 2*a*b + c + d + e
tuple<uint64_t, uint64_t, uint64_t> madd2s(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c,
    const uint64_t& d,
    const uint64_t& e
)
{
    uint64_t carry, sum, superhi, hi, lo, _;

    tie(hi, lo) = Mul64(a, b);
    tie(lo, carry) = Add64(lo, lo, 0);
    tie(hi, superhi) = Add64(hi, hi, carry);

    tie(sum, carry) = Add64(c, e, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(lo, carry) = Add64(lo, sum, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(hi, _) = Add64(hi, 0, d);
    return {superhi, hi, lo};
}

tuple<uint64_t, uint64_t, uint64_t> madd1s(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& d,
    const uint64_t& e
)
{
    uint64_t carry, superhi, hi, lo, _;

    tie(hi, lo) = Mul64(a, b);
    tie(lo, carry) = Add64(lo, lo, 0);
    tie(hi, superhi) = Add64(hi, hi, carry);
    tie(lo, carry) = Add64(lo, e, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(hi, _) = Add64(hi, 0, d);
    return {superhi, hi, lo};
}

tuple<uint64_t, uint64_t, uint64_t> madd2sb(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c,
    const uint64_t& e
)
{
    uint64_t carry, sum, superhi, hi, lo, _;

    tie(hi, lo) = Mul64(a, b);
    tie(lo, carry) = Add64(lo, lo, 0);
    tie(hi, superhi) = Add64(hi, hi, carry);

    tie(sum, carry) = Add64(c, e, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(lo, carry) = Add64(lo, sum, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return {superhi, hi, lo};
}

tuple<uint64_t, uint64_t, uint64_t> madd1sb(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& e
)
{
    uint64_t carry, superhi, hi, lo, _;

    tie(hi, lo) = Mul64(a, b);
    tie(lo, carry) = Add64(lo, lo, 0);
    tie(hi, superhi) = Add64(hi, hi, carry);
    tie(lo, carry) = Add64(lo, e, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    return {superhi, hi, lo};
}

tuple<uint64_t, uint64_t> madd3(
    const uint64_t& a,
    const uint64_t& b,
    const uint64_t& c,
    const uint64_t& d,
    const uint64_t& e
)
{
    uint64_t carry, hi, lo, _c, _;
    tie(hi, lo) = Mul64(a, b);
    tie(_c, carry) = Add64(c, d, 0);
    tie(hi, _) = Add64(hi, 0, carry);
    tie(lo, carry) = Add64(lo, _c, 0);
    tie(hi, _) = Add64(hi, e, carry);
    return {hi, lo};
}

} // namespace bls12_381