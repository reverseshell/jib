#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef JIT_MEMORY_PROTECTION
    #include <sys/mman.h>
    #include <mach/i386/vm_param.h>
    #include <errno.h>
#endif

#include "Jitter.h"

#define BF_JIT_CODE_SIZE    2048
#define BF_JIT_MEMORY_SIZE  2048

BFJitter::BFJitter(Emitter* emitter)
{
    _emitter = emitter;
}

bool BFJitter::CompileNextInstruction(char instruction)
{
    bool terminate = false;
    bool loop = false;
    
    switch (instruction)
    {
        case '>':
            _emitter->IncrementPointer();
            break;

        case '<':
            _emitter->DecrementPointer();
            break;

        case '+':
            _emitter->Increment();
            break;

        case '-':
            _emitter->Decrement();
            break;

        case '.':
            _emitter->Output();
            break;

        case ',':
            _emitter->Input();
            break;

        case '[':
            _emitter->EnterLoop();
            break;

        case ']':
            _emitter->EndLoop();
            break;

        default:
            terminate = true;
            break;
    }

    return terminate;
}

jit_func BFJitter::Compile(const char* bf)
{   
    // TODO: Error handling for bad bf code
    
    bool done = false;
    const char* instruction = bf;
    
    this->Initialize();

    _emitter->Prologue();
    
    while (!done)
    {
        done = this->CompileNextInstruction(*instruction++);
    }
    
    _emitter->Epilogue();
    
    this->Finalize();
    
    return (jit_func)_code_base;
}

void BFJitter::Initialize()
{   
    // Allocate the BF memory
    _memory_base = (char*)malloc(BF_JIT_MEMORY_SIZE);
    
    // Allocate space for the jitted code
#ifdef JIT_MEMORY_PROTECTION
    int result = posix_memalign((void**)&_code_base, PAGE_SIZE, BF_JIT_CODE_SIZE);
#else    
    _code_base = (char*)malloc(BF_JIT_CODE_SIZE);
#endif

    // Initialize the memory
    memset(_memory_base, 0x00, BF_JIT_MEMORY_SIZE);
    memset(_code_base, 0xcc, BF_JIT_CODE_SIZE);
    
    // Initialize the machine-code emitter
    _emitter->Initialize(_code_base, _memory_base);
}

void BFJitter::Finalize()
{
#ifdef JIT_MEMORY_PROTECTION
    int result = mprotect(_code_base, BF_JIT_CODE_SIZE, PROT_READ | PROT_EXEC);   
#endif
}
