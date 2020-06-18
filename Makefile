CC = gcc

aes.exe: cipher.o main.o
	$(CC) cipher.o main.o -o aes.exe

clean: 
	rm -f *.o
	rm aes.exe

run: aes.exe
	clear; ./aes.exe
