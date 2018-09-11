/*
 * text.c
 *
 *  Created on: Nov 16, 2016
 *      Author: Jerry
 */

#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Wrapper class to hold a text file in array format
text* createText(char* filename, int maxLine) {

	//Allocate memory for empty text object and set fields to defaults
	text* t = (text*) malloc(sizeof(text));
	t->size = 10;
	t->curLine = 0;
	t->maxLine = maxLine;
	t->data = (char**) malloc(10 * sizeof(char*));
	for (int i = 0; i < t->size; i++) {
		t->data[i] = NULL;
	}

	//If the is an included filename, open the file and load it into the text object
	if (filename != NULL) {
		FILE* fileptr = fopen(filename, "r");
		if (fileptr == NULL) {
			printf("Null File Pointer\n");
			exit(1);
		}

		// character buffer to store each line
		char buf[t->maxLine];

		// while there are lines to be read, go through each line and connect the nodes
		while (fgets(buf, t->maxLine, fileptr) != NULL) {
			addLine(t, buf, t->maxLine);
		}

		fclose(fileptr);
	}
	return t;
}

//Adds a line to the text object
void addLine(text* t, char* line, int lineLength) {
	//If the test array is too small, reallocate the array of pointers to double the size
	if (t->curLine >= t->size) {
		t->data = (char**) realloc(t->data, sizeof(char*) * t->size * 2);
		if (t->data == NULL) {
			printf("Text Size Increase Failed.\n");
			exit(1);
		}
		for (int i = t->size; i < t->size * 2; i++) {
			t->data[i] = NULL;
		}

		t->size = t->size * 2;
	}

	//Add the current line of data to the text object
	t->data[t->curLine] = strdup(line);
	if (t->data[t->curLine] == NULL) {
		printf("Row Allocation Failed.\n");
		exit(1);
	}
	t->curLine++;
}

//Prints out the text file line by line
void printText(text* t) {
	for (int i = 0; i < t->size; i++) {
		if (t->data[i] != NULL) {
			printf("%s", t->data[i]);
		}
	}
	printf("\n");
}

//Deletes the text object and frees all memory
void deleteText(text* t) {
	for (int i = 0; i < t->size; i++) {
		free(t->data[i]);
	}
	free(t->data);
	free(t);
}

//Copies a text object into a new text object
text* copyText(text* t) {
	text* copy = createText(NULL,t->maxLine);
	for (int i = 0; i < t->curLine; i++) {
		addLine(copy, t->data[i],t->maxLine);
	}

	return copy;
}
