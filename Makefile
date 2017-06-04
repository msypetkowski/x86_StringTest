all:
	g++ main.cpp -o main.o -g -c -Wall -Wshadow -O2
	nasm -f elf64 test1.asm -o test1.o -g
	nasm -f elf64 test2.asm -o test2.o -g
	nasm -f elf64 test3.asm -o test3.o -g
	nasm -f elf64 test4.asm -o test4.o -g
	g++ main.o test1.o test2.o test3.o test4.o -o test -g -O2

clean:
	rm test1.o
	rm test2.o
	rm test3.o
	rm test4.o
	rm main.o
	rm test
