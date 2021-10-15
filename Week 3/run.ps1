gcc -c charcode.c
gcc -c token.c
gcc -c reader.c
gcc -c error.c
gcc -c scanner.c
gcc -o 1 scanner.o charcode.o token.o reader.o error.o
./1 ..\test\example1.kpl