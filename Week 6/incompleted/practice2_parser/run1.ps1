gcc -c charcode.c 
gcc -c reader.c 
gcc -c error.c 
gcc -c token.c 
gcc -c final.c
gcc -c scanner.c
gcc -c main.c  
gcc -o 1 final.o main.o charcode.o reader.o error.o token.o scanner.o
./1 ..\test\example3.kpl