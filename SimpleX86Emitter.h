#ifndef _SIMPLEX86EMITTER_H_
#define _SIMPLEX86EMITTER_H_

#include <stack>

#include "Emitter.h"

class SimpleX86Emitter : public Emitter
{
public:

    SimpleX86Emitter();
    ~SimpleX86Emitter();

    // Emitter
    void Initialize(char* code_base, char* memory_base);
    void Prologue();
    void Epilogue();
    void IncrementPointer();
    void DecrementPointer();
    void Increment();
    void Decrement();
    void Output();
    void Input();
    void EnterLoop();
    void EndLoop();
    
private:
    char* _ip;
    char* _mp;
    
    std::stack<char*> _frames;
    
    static int CalculateRelativeOffset(char* absoluteDest, char* ip, int len);
};

#endif
