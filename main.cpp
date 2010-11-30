#include "Jitter.h"
#include "OptimizedX86Emitter.h"

// usage: ./jib [bf]

int main(int argc, char** argv)
{
    const char* bf = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
    
    if (argc > 1)
    {
        bf = argv[1];    
    }

    BFJitter* jitter = new BFJitter(new OptimizedX86Emitter());
    jit_func code = jitter->Compile(bf);

    code();

    return 0;
}
