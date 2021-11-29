gcc -c charcode.c 
gcc -c reader.c 
gcc -c error.c 
gcc -c token.c 
gcc -c parser.c
gcc -c scanner.c
gcc -c main.c  
gcc -o 1 parser.o main.o charcode.o reader.o error.o token.o scanner.o
./1 ..\test\example4.kpl