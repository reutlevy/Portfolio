#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: task4

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
task4: task4.o
	gcc -g -m32 -Wall -o task4 task4.o

task4.o: task4.c
	gcc -m32 -g -Wall -fno-pie -c -o task4.o task4.c


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task4
