gcc -c charcode.c 
gcc -c reader.c 
gcc -c error.c 
gcc -c token.c 
gcc -c parser.c
gcc -c main.c  
gcc -o 1 main.o charcode.o reader.o error.o token.o 
./1