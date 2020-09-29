#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task2

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
task2: main2.o task2.o
	gcc -m32 -g -Wall -o task2 task2.o task2.o

# Depends on the source and header files
main2.o: main_task2Assignment1.c 
	gcc -g -Wall -m32 -ansi -c -o main2.o main_task2Assignment1.c 
 
task2.o: asm_task2Assignment1.s
	nasm -g -f elf -w+all -o task2.o asm_task2Assignment1.s


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task2
