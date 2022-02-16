gcc -c charcode.c 
gcc -c error.c 
gcc -c reader.c 
gcc -c scanner.c 
gcc -c token.c 
gcc -o 1 charcode.o error.o reader.o SCANNER.o token.o
./1 ../test/example1.kpl