# Makefile to build Ironman HTTP Server
# We will use the gcc compiler infrastructure
# for building

# TO THE COMPILER: I know where you are installed. No funny games, capice?

# The default compiler for now will be gcc.
CC=gcc

# The compiler flags we are using
# -c: compiles the source file, and only produces an object file (No linking done)
CFLAGS=-c 

# --- targets
Ironman: src/Ironman.c src/lib/networking.o src/lib/initialization.o 
		gcc src/Ironman.c src/lib/networking.o src/lib/initialization.o \
		src/lib/control.o -o src/Ironman

src/lib/initialization.o: src/lib/initialization.c src/lib/control.o
		$(CC) $(CFLAGS) src/lib/initialization.c src/lib/control.o -o src/lib/initialization.o

src/lib/networking.o: src/lib/networking.c src/lib/control.o
		$(CC) $(CFLAGS) src/lib/networking.c src/lib/control.o -o src/lib/networking.o

src/lib/control.o: src/lib/control.c
		$(CC) $(CFLAGS) src/lib/control.c -o src/lib/control.o
		
src/lib/logger.o: src/lib/logger.c
		$(CC) $(CFLAGS) src/lib/logger.c -o src/lib/logger.o 

# -- remove object, binary and the rest of the build data.
clean:
		rm -f Ironman
		rm -rf ./src/*.o
		rm -rf ./src/lib/*.o

