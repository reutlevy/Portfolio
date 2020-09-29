#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: 1b

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
1b: encoder.o start.o util.o
	ld -m elf_i386 start.o encoder.o util.o -o 1b

start.o: start.s
	nasm -f elf32 start.s -o start.o	

# Depends on the source and header files
encoder.o: 1b.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector 1b.c -o encoder.o 
 
util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o 1b
