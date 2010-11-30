INCLUDE=.

OUTPUT=jib
SRC=Jitter.cpp SimpleX86Emitter.cpp OptimizedX86Emitter.cpp main.cpp

CC=g++
CFLAGS=-m32 -DJIT_MEMORY_PROTECTION
LD=ld

# ##################
# Targets
# ##################

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT)
