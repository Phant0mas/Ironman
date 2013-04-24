# Makefile to build Ironman HTTP Server
# We will use the gcc compiler infrastructure
# for building

# --- targets
Ironman: src/Ironman.o src/lib/initialization.o src/lib/networking.o src/lib/control.o
		ld src/Ironman.o src/lib/initialization.o \
		src/lib/networking.o src/lib/control.o -o Ironman

/src/Ironman.o: 
		gcc src/Ironman.c /src/lib/networking.o /src/lib/initialization.o \
		-o src/Ironman.o

/src/lib/initialization.o:
		gcc -c src/lib/initialization.c src/lib/control.o \
		-o src/lib/initialization.o

/src/lib/networking.o:
		gcc -c src/lib/networking.c src/lib/control.o \
		-o src/lib/networking.o

/src/lib/control.o: 
		gcc -c src/lib/control.c -o src/lib/control.o

# --- remove binary and executable files

#this way it compiles,at least until we fix the first one
HardIronman: 
		gcc src/Ironman.c src/lib/control.c src/lib/control.h src/lib/networking.c src/lib/networking.h src/lib/initialization.c src/lib/initialization.h -o Ironman


clean:
		rm -f Ironman
		rm -rf ./src/*.o
		rm -rf ./src/lib/*.o

