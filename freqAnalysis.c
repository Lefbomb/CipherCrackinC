/*
 * freqAnalysis.c
 *
 *  Created on: Nov 17, 2016
 *      Author: Jerry
 */

#include <string.h>
#include "text.h"
#include "dict.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

//Return the indexes required to sort an array in descending order
void indexSort(float* data, int size, int* sortedIndexes) {
	//Comparator function for qsort
	int cmp(const void *a, const void *b) {
		int ia = *(int *) a;
		int ib = *(int *) b;
		return data[ia] < data[ib] ? 1 : -1;
	}

	//Array of indexes
	for (int i = 0; i < size; i++) {
		sortedIndexes[i] = i;
	}

	//Sort the array based on the values, but move the indexes
	qsort(sortedIndexes, size, sizeof(*sortedIndexes), cmp);
}

//Perform single letter frequency analysis
char* singleLetterAnalysis(text* t, char* cipherKey) {
	//Load text file with frequencies
	FILE* fileptr = fopen("monograms.txt", "r");
	if (fileptr == NULL) {
		printf("No Single Letter Frequency File\n");
		exit(1);
	}

	//Read each line of the file and get the order in which of often
	//each letter appears in the English language
	char buf[20];
	int idx = 0;
	char letterOrder[26];
	while (fgets(buf, 20, fileptr) != NULL) {
		letterOrder[idx] = tolower(buf[0]);
		idx++;
	}
	fclose(fileptr);

	//Setup to count the number of characters in the file
	float letterCount[26];
	float letterFreq[26];
//	float letterCount[26] = {0};
//	float letterFreq[26] = {0};

	for (int i = 0; i < 26; i++) {
		letterCount[i] = 0;
		letterFreq[i] = 0;
	}

	//Read through each line in the text file and count the occurance of each character
	int maxLine = t->maxLine;
	char curLine[maxLine];
	memset(curLine, '\0', maxLine);
	for (int i = 0; i < t->curLine; i++) {
		if (t->data[i] != NULL) {
			strcpy(curLine, t->data[i]);
			for (int j = 0; j < strlen(curLine); j++) {
				if (isalpha(curLine[j])) {
					letterCount[(int) tolower(curLine[j]) - 97]++;
				}
			}
			memset(curLine, '\0', maxLine);
		}
	}

	//Calculate the total number of characters
	float totalChar = 0;
	for (int i = 0; i < 26; i++) {
		totalChar = totalChar + letterCount[i];
	}
	printf("\nTotal Char = %.0f\n", totalChar);

	//Calculate the frequency distribution of each character in the cipher text
	for (int i = 0; i < 26; i++) {
		letterFreq[i] = letterCount[i] / totalChar;
	}

	//Sort the letters in order of descending frequency
	int sortedIndexes[26];
	indexSort(letterFreq, 26, sortedIndexes);

	//Assign the most frequently appearing letter to the letter that appeared
	//most frequently appearing letter in the cipher text and repeat for each letter
	for (int i = 0; i < 26; i++) {
		cipherKey[sortedIndexes[i]] = letterOrder[i];
	}

	return cipherKey;
}

//Score the fitness of the text to written English
float scoreQuadgram(text* t, dict* d) {
	float score = 0;
	for (int i = 0; i < t->curLine; i++) {
		//Get current line
		char* line = strdup(t->data[i]);

		//convert it to lower case line without spaces or punctuation
		int lineLen = strlen(line);
		char dest[lineLen];
		int k = 0;
		for (int j = 0; j < strlen(line); j++) {
			if (isalpha(line[j])) {
				dest[k++] = tolower(line[j]);
			}
		}
		dest[k] = '\0';

		//Calculate score for line
		int procLen = strlen(dest);
		if (procLen >= 4) {

			//Get current quadgram
			char chunk[5];
			for (int j = 0; j < procLen - 3; j++) {
				for (int k = 0; k < 4; k++) {
					chunk[k] = dest[j + k];
				}
				chunk[4] = '\0';

				//Check for quadgram
				entry* e = lookup(d, chunk);
				if (e != NULL) {
					score += e->defn;
				} else {
					score += -40;	//floored value to prevent adding negative infinity
				}
			}
		}
		free(line);
	}
	return score;
}

//Substitute the text using the inputed key
void substitute(text* t, char* key) {
	for (int i = 0; i < t->curLine; i++) {
		for (int j = 0; j < strlen(t->data[i]); j++) {
			if (isupper(t->data[i][j])) {
				t->data[i][j] = toupper(key[tolower(t->data[i][j]) - 97]);
			} else if (islower(t->data[i][j])) {
				t->data[i][j] = key[t->data[i][j] - 97];
			}
		}
	}
}
