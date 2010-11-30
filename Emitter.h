#ifndef _EMITTER_H_
#define _EMITTER_H_

/*
 * Abstract base class for machine code emitters
 */ 
class Emitter
{
public:
    virtual ~Emitter() { };
    
    virtual void Initialize(char* code_base, char* memory_base) = 0; 
    virtual void Prologue() = 0;
    virtual void Epilogue() = 0;

    virtual void IncrementPointer() = 0;    // >
    virtual void DecrementPointer() = 0;    // <

    virtual void Increment() = 0;           // +
    virtual void Decrement() = 0;           // -

    virtual void Output() = 0;              // .
    virtual void Input() = 0;               // ,

    virtual void EnterLoop() = 0;           // [
    virtual void EndLoop() = 0;             // ]
};

#endif
