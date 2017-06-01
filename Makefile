all:# test1 test2
	g++ main.cpp -o main.o -g -c -Wall -Wshadow -O2
	nasm -f elf64 test1.asm -o test1.o -g
	nasm -f elf64 test2.asm -o test2.o -g
	g++ main.o test1.o test2.o -o test -g -O2

clean:
	rm test1.o
	rm main.o
	rm test
	rm test2.o
