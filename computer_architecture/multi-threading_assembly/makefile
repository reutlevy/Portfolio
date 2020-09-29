all: exec

libs: ASS3 

ASS3: ass3.s  scheduler.s printer.s target.s drone.s
	nasm -f elf -o ass3.o ass3.s
	nasm -f elf -o scheduler.o scheduler.s
	nasm -f elf -o printer.o printer.s
	nasm -f elf -o target.o target.s
	nasm -f elf -o drones.o drone.s

exec: libs
	gcc -g -m32 -Wall ass3.o scheduler.o printer.o drones.o target.o -o ass3
	rm ass3.o scheduler.o printer.o drones.o target.o

.PHONY: clean
clean:
	rm -rf ./*.o ass3
