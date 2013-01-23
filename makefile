# Makefile to build Ironman HTTP Server
# We will use the gcc compiler infrastructure
# for building

# --- targets
Ironman : 
		gcc -o Ironman src/Ironman.c > report 2>&1

# --- remove binary and executable files
clean:
		rm -f Ironman
		rm -f report
