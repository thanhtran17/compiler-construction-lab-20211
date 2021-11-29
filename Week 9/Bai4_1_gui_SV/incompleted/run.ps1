gcc -c debug.c
gcc -c main.c
gcc -c symtab.c 
gcc -o 1 debug.o main.o symtab.o 
./1
