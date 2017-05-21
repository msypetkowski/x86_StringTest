all:
	g++ main.cpp -o main.o -g -c -Wall -Wshadow -O2
	nasm -f elf64 test1.asm -o test1.o -g
	g++ main.o test1.o -o qwe -g -O2
