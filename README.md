## AES-128-Implementation
### ABOUT
This is my bare bones impmementation of the AES-128 (aka Rijndael) encryption algorithm. Insert your usual _**"don't use in production environment"**_ warning here, 
as this project always was, from the beginning, made from a recreational and educational point of view.

### REQUIREMENTS
This Project was built using gcc version 10.1.0 on my Linux system. Although any C compiler with C11 support may be able to compile this project

### HOW TO BUILD AND RUN (ASSUMES MAKE UTILITIES ARE PRE-INSTALLED ON THE SYSTEM)
`make clean`

`make run`

### IF MAKE UTILITIES AREN'T INSTALLED (ASSUMES LATEST GCC VERSION IS INSTALLED)
`gcc cipher.c -c cipher.o`

`gcc main.c -c main.o`

`gcc cipher.o main.o -o aes.exe`

`./aes.exe` (on Unix based machines)

OR `aes.exe`(On Windows machines)

### CONTRIBUTION
This is a very bare bones implementation of the AES-128. I say that because there are many things that are missing from this utility:

1. Variable size password (not key; key will always be 16 bytes long)
2. Variable size data (currently supports the standard single 16 byte block data)
3. Seperate Encipher and Decipher modes (currently just enciphers, outputs, deciphers and outputs)
4. bit optimizations are heavily missing from the basic operations of AES

1 may be achieved by using a hashing functions that creates a 128 bit digest (like MD5 or something)

2 may be achieved by calculating the number of blocks of data, encrypting individual blocks and appending them to a buffer, and then outputting the buffer in the end

3 may be achieved by making a seperate decipher function inside cipehr.c and putting all the decipher functions inside it

4 is not really clear to me, but I feel that there may be many places where I have slacked very much, and optimizations may be possible :)

### RESOURCES USED
1. https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf (official AES-128 specification from FIPS)
2. https://web.stanford.edu/class/ee392d/Chap7.pdf (to read about finite fields)
3. https://crypto.stackexchange.com/questions/12873/aes-mixcolumn-stage?noredirect=1&lq=1 (in order to get some insigts about MixColumns()) 
4. https://crypto.stackexchange.com/questions/63158/regular-and-efficient-doubling-in-gf2n?noredirect=1&lq=1 (some more insigts on FF multiplication)

