#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void main(){
	char inputChar;
	int letter = 0, digit = 0;
	
	FILE* fi = fopen("input.txt","rt");	

	if (fi == NULL){
    printf("ERROR!! on loading input file");   
    exit(1);             
  }
    
  while (1) {
    if (feof(fi)) break;
    fscanf(fi,"%c", &inputChar);

    if (isdigit(inputChar)) 
			digit++;
		if(isalpha(inputChar)) 
			letter++;
	}

	printf("Number of digits: %d\n", digit);
	printf("Number of letters: %d", letter);

	fclose(fi);
}
