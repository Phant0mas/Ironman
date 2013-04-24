# Makefile to build Ironman HTTP Server
# We will use the gcc compiler infrastructure
# for building

# --- targets
Ironman: src/Ironman.c src/lib/networking.o src/lib/initialization.o 
		gcc src/Ironman.c src/lib/networking.o src/lib/initialization.o \
		src/lib/control.o -o src/Ironman

src/lib/initialization.o: src/lib/initialization.c src/lib/control.o
		gcc -c src/lib/initialization.c src/lib/control.o -o src/lib/initialization.o

src/lib/networking.o: src/lib/networking.c src/lib/control.o
		gcc -c src/lib/networking.c src/lib/control.o -o src/lib/networking.o

src/lib/control.o: src/lib/control.c
		gcc -c src/lib/control.c -o src/lib/control.o

# -- remove object, binary and the rest of the build data.
clean:
		rm -f Ironman
		rm -rf ./src/*.o
		rm -rf ./src/lib/*.o

