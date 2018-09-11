/*
 * decrypt.c
 *
 *  Created on: Nov 16, 2016
 *      Author: Jerry
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text.h"
#include <ctype.h>
#include "freqAnalysis.h"
#include "dict.h"
#include <math.h>

int main(int argc, char* argv[]) {
	//Load filenames;
	char* dictFilename = argv[1];
	char* sourceFilename = argv[2];

	//Load Dictionary
	dict* quadgrams = createDictionary(200000);
	FILE* fileptr = fopen(dictFilename, "r");
	if (fileptr == NULL) {
		printf("Null File Pointer\n");
		exit(1);
	}

	//Go through each line in the dictionary text file and load it
	//into the dictionary and get the total character count
	char buf[20];
	float totalCount = 0;
	int quadGramCount = 0;
	while (fgets(buf, 20, fileptr) != NULL) {
		char* name = strtok(buf, " ");
		float defn;
		sscanf(strtok(NULL, " "), "%f", &defn);
		totalCount += defn;
		install(quadgrams, name, defn);
		quadGramCount++;
	}
	fclose(fileptr);
	printf("Dictionary loaded with %d unique quadgrams.\nTotal Count: %.0f.\n",
			quadGramCount, totalCount);

	//Convert all the counts to log probabilities
	for (int i = 0; i < quadgrams->tableSize; i++) {
		entry* e;
		for (e = quadgrams->entries[i]; e != NULL; e = e->next) {
			e->defn = log(e->defn / totalCount);
		}
	}

	//Load text and show successful load
	text* input = createText(sourceFilename, 1000);
	printf("Initial Text:\n");
	printText(input);

	//Perform initial analysis
	char curKey[26]; //= "mjgfzvohldyariuesqxkbwptnc";
	singleLetterAnalysis(input, curKey);

	//Print keys in human readable format
	//To decrypt manually, take the cipher text character and
	//substitute with the character below.
	float order[26];
	for (int i = 0; i < 26; i++) {
		order[i] = (float) curKey[i] - 97;
	}
	int indexes[26];
	indexSort(order, 26, indexes);
	printf("\nCurrent Key:\nCipher Text:\t");
	for (int i = 0; i < 26; i++) {
		printf("%c", (char) i + 97);
	}
	printf("\nPlain Text:\t");
	for (int i = 25; i >= 0; i--) {
		printf("%c", (char) indexes[i] + 97);
	}
	printf("\n\n");

	//Score original text
	float curScore = scoreQuadgram(input, quadgrams);
	printf("Initial Score: %.3f\n\n", curScore);

	//Score initial decryption attempt
	text* copy = copyText(input);
	substitute(copy, curKey);
	curScore = scoreQuadgram(copy, quadgrams);
	deleteText(copy);
	printf("Initial Attempt Score: %.3f\n\n", curScore);

	//Crack the subsitution cipher
	printf("Cracking\n");

	//Slowly goes through all the combinations of 2 letter swaps
	//It starts with swapping the first letter with all the other letters,
	//then the second letter with all letters but the first and so on
	//If the score of the text is improved by the swap. The test key is
	//swapped to the current key and all search indices are reset to zero
	//The code stops when no more swaps to improve the score of the text
	//can be found
	int swaps = 0;
	char testKey[26];
	copy = copyText(input);
	float newScore;
	int i = 0;	//Index of first letter to swap
	while (i < 26) {
		int j = i + 1;	//Index of second letter to swap
		while (j < 26) {
			//Copy key
			for (int k = 0; k < 26; k++) {
				testKey[k] = curKey[k];
			}

			//Swap letters
			char temp = testKey[i];
			testKey[i] = testKey[j];
			testKey[j] = temp;

			//Test the fitness of the key
			substitute(copy, testKey);
			newScore = scoreQuadgram(copy, quadgrams);

			//If the score is better, copy the new key over and reset swap parameters
			if (newScore > curScore) {
				for (int j = 0; j < 26; j++) {
					curKey[j] = testKey[j];
				}
				curScore = newScore;
				printf("Swapped: %c<->%c New Max Score: %.3f\n", testKey[i],
						testKey[j], curScore);
				j = 0;
				i = 0;
				swaps++;
			} else {
				j++;
			}
			deleteText(copy);
			copy = copyText(input);
		}
		i++;
	}
	printf("\n");
	printf("Total Swaps: %d\n", swaps);	//Total number of swaps that improved key
	printf("Final Score: %.3f\n", curScore);	//Final key score
	printf("Final Key:\n");

	//Print key in human usable form
	for (int i = 0; i < 26; i++) {
		order[i] = (float) curKey[i] - 97;
	}
	indexSort(order, 26, indexes);
	printf("\nCurrent Key:\nCipher Text:\t");
	for (int i = 0; i < 26; i++) {
		printf("%c", (char) i + 97);
	}
	printf("\nPlain Text:\t");
	for (int i = 25; i >= 0; i--) {
		printf("%c", (char) indexes[i] + 97);
	}
	printf("\n\n");

	//Print out decoded text
	text* final = copyText(input);
	substitute(final, curKey);
	printText(final);

	//Write to file
	int fileNameLength = strlen(sourceFilename);
	char outputName[fileNameLength + 9];
	for (int i = 0; i < fileNameLength-4; i++) {
		outputName[i] = sourceFilename[i];
	}
	outputName[fileNameLength-4] = '\0';
	char* decodeEnd = "_decoded.txt";
	strcat(outputName, decodeEnd);
	FILE* outFile = fopen(outputName, "w");
	for (int i = 0; i < final->curLine; i++) {
		fprintf(outFile, final->data[i]);
	}
	fclose(outFile);

	//Clear up memory
	deleteText(copy);
	deleteText(input);
	deleteText(final);
	deleteDictionary(quadgrams);

	return (0);
}

