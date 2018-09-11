/*
 * encrypt.c
 *
 *  Created on: Nov 27, 2016
 *      Author: Jared
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
void shuffle(char *array, size_t n);

int main(int argc, char* argv[]) {
	srand(time(NULL));
	FILE *fptr1, *fptr2;
	fptr1 = fopen(argv[1], "r");
	if (fptr1 == NULL) {
		printf("Null File Pointer\n");
		exit(1);
	}
	fptr2 = fopen(argv[2], "w+");
	char c;
	c = tolower(fgetc(fptr1));
	char alphabet[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
			'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
			'y', 'z' };
	char *a;
	a = alphabet;
	shuffle(a, 26);

	for (int i = 0; i < sizeof(alphabet); i++) {//print cipher
		printf("%c|",alphabet[i]);
	}
	printf("\n");
	int newline = 0;
	while (c != EOF) {
		if (isalpha(c)) {
			c = alphabet[c % 97];
			fputc(c, fptr2);
		}
		else if(c == 10){
			newline=0;
		}
		else {
			fputc(c, fptr2);
		}
		newline++;
		if (newline % 100 == 0) {
			fputc(10, fptr2);//ascii value of 10 is newline
		}
		c = tolower(fgetc(fptr1));
	}

	fclose(fptr1);
	fclose(fptr2);
	return 0;
}

void shuffle(char *array, size_t n) {
	if (n > 1) {
		size_t i;
		for (i = 0; i < n - 1; i++) {
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			char t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

