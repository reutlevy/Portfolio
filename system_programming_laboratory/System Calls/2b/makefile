#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: 2b

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
2b: flame.o util.o start.o
	ld -m elf_i386 flame.o util.o start.o -o 2b

start.o: start.s
	nasm -f elf32 start.s -o start.o

# Depends on the source and header files
flame.o: 2b.c 
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector 2b.c -o flame.o 
 
util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	



#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o 2b
