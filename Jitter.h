#ifndef _JITTER_H_
#define _JITTER_H_

#include "Emitter.h"

typedef void (*jit_func)();

class BFJitter
{
public:
    BFJitter(Emitter* emitter);
    ~BFJitter();

    // Compile code from an input string
    // Returns when a non-BF character is encountered (or NULL);
    jit_func Compile(const char* code);

private:

    Emitter* _emitter;
    char* _code_base;
    char* _memory_base;
    
    void Initialize();
    void Finalize();
    bool CompileNextInstruction(char instruction);
};

#endif
