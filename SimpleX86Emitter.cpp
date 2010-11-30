#include <iostream>
#include <stack>
#include <stdlib.h>

#include "SimpleX86Emitter.h"

#define EMIT_BYTE(d, b)     *((d))++ = (b)

#define EMIT_WORD(d, w)     {                           \
                                *(int*)(d) = (int)(w);  \
                                (d) += 4;               \
                            }

SimpleX86Emitter::SimpleX86Emitter()
{
    
}

SimpleX86Emitter::~SimpleX86Emitter()
{
    
}

void SimpleX86Emitter::Initialize(char* code_base, char* memory_base)
{
    _ip = code_base;   
    _mp = memory_base;
}

void SimpleX86Emitter::Prologue()
{
    EMIT_BYTE(_ip, 0x55); // push %ebp
    EMIT_BYTE(_ip, 0x89); // mov %esp, %ebp
    EMIT_BYTE(_ip, 0xe5); 
    
    EMIT_BYTE(_ip, 0x81); // sub $0x8, %esp
    EMIT_BYTE(_ip, 0xec);
    EMIT_WORD(_ip, 8);
    
    EMIT_BYTE(_ip, 0x89); // mov %esi,0x4(%esp)
    EMIT_BYTE(_ip, 0x74);
    EMIT_BYTE(_ip, 0x24);
    EMIT_BYTE(_ip, 0x04);
        
    EMIT_BYTE(_ip, 0xbe); // mov _mp, %esi
    EMIT_WORD(_ip, _mp);
    
    EMIT_BYTE(_ip, 0x31); // xor %eax, %eax
    EMIT_BYTE(_ip, 0xc0);
}

void SimpleX86Emitter::Epilogue()
{
    EMIT_BYTE(_ip, 0x8b); // mov 0x4(%esp),%esi
    EMIT_BYTE(_ip, 0x74);
    EMIT_BYTE(_ip, 0x24);
    EMIT_BYTE(_ip, 0x04);
    
    EMIT_BYTE(_ip, 0xc9); // leave
    EMIT_BYTE(_ip, 0xc3); // ret
}

void SimpleX86Emitter::IncrementPointer()
{
    EMIT_BYTE(_ip, 0x46); // inc %esi
}

void SimpleX86Emitter::DecrementPointer()
{
    EMIT_BYTE(_ip, 0x4e); // dec %esi
}

void SimpleX86Emitter::Increment()
{
    EMIT_BYTE(_ip, 0x8a); // movb (%esi), %al
    EMIT_BYTE(_ip, 0x06);
    EMIT_BYTE(_ip, 0xfe); // inc %al
    EMIT_BYTE(_ip, 0xc0); 
    EMIT_BYTE(_ip, 0x88); // movb %al, (%esi)
    EMIT_BYTE(_ip, 0x06);
}

void SimpleX86Emitter::Decrement()
{
    EMIT_BYTE(_ip, 0x8a); // movb (%esi), %al
    EMIT_BYTE(_ip, 0x06);
    EMIT_BYTE(_ip, 0xfe); // dec %al
    EMIT_BYTE(_ip, 0xc8); 
    EMIT_BYTE(_ip, 0x88); // movb %al, (%esi)
    EMIT_BYTE(_ip, 0x06);
}

void SimpleX86Emitter::Output()
{
    EMIT_BYTE(_ip, 0x8a); // movb (%esi), %al
    EMIT_BYTE(_ip, 0x06);

    EMIT_BYTE(_ip, 0x89); // mov %eax, (%esp)
    EMIT_BYTE(_ip, 0x04);
    EMIT_BYTE(_ip, 0x24);
    
    int offset = SimpleX86Emitter::CalculateRelativeOffset((char*)putchar, _ip, 5);
    EMIT_BYTE(_ip, 0xe8); // call output_character
    EMIT_WORD(_ip, offset);
}

void SimpleX86Emitter::Input()
{
    int offset = SimpleX86Emitter::CalculateRelativeOffset((char*)getchar, _ip, 5);
    EMIT_BYTE(_ip, 0xe8); // call output_character
    EMIT_WORD(_ip, offset);
    
    EMIT_BYTE(_ip, 0x88); // movb %al, (%esi)
    EMIT_BYTE(_ip, 0x06);
}

void SimpleX86Emitter::EnterLoop()
{
    _frames.push(_ip);
    
    EMIT_BYTE(_ip, 0x8a); // movb (%esi), %al
    EMIT_BYTE(_ip, 0x06);
    
    EMIT_BYTE(_ip, 0x3c); // cmp %al, 0x00
    EMIT_BYTE(_ip, 0x00);
    
    EMIT_BYTE(_ip, 0x0f); // jz <DUMMY>
    EMIT_BYTE(_ip, 0x84);
    EMIT_WORD(_ip, 0x0);
}

void SimpleX86Emitter::EndLoop()
{
    char *loop = _frames.top();
    _frames.pop();
    
    int offset = SimpleX86Emitter::CalculateRelativeOffset(loop, _ip, 5);
    EMIT_BYTE(_ip, 0xe9); // jmp <loop>
    EMIT_WORD(_ip, offset);
    
    // Re-write the jz at the beginning of the loop to point here
    char *jz_operand = loop + 6;
    offset = _ip - loop - 10;
    *(int*)jz_operand = offset;
}

int SimpleX86Emitter::CalculateRelativeOffset(char* absoluteDest, char* ip, int len)
{
    return absoluteDest - (ip + len);   
}
