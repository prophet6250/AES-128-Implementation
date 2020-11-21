CC = gcc

aes.exe: cipher.o main.o
	$(CC) cipher.o main.o -o aes.exe

clean: 
	rm -f *.o
	rm aes.exe
	rm output

run: aes.exe
	clear
	./aes.exe > output
