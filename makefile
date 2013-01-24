# Makefile to build Ironman HTTP Server
# We will use the gcc compiler infrastructure
# for building

# --- targets
Ironman: src/Ironman.o src/lib/initialization.o src/lib/networking.o src/lib/control.o
		ld src/Ironman.o src/lib/initialization.o \
		src/lib/networking.o src/lib/control.o -o Ironman

/src/Ironman.o: 
		gcc src/Ironman.c -o src/Ironman.o

/src/lib/initialization.o:
		gcc -c src/lib/initialization.c -o src/lib/initialization.o

/src/lib/networking.o:
		gcc -c src/lib/networking.c -o src/lib/networking.o

/src/lib/control.o: 
		gcc -c src/lib/control.c -o src/lib/control.o

# --- remove binary and executable files
clean:
		rm -f Ironman
		rm -f report
