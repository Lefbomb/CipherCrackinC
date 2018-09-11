/*
 * freqAnalysis.h
 *
 *  Created on: Nov 17, 2016
 *      Author: Jerry
 */

#ifndef FREQANALYSIS_H_
#define FREQANALYSIS_H_

#include "text.h"
#include "dict.h"

//Perform single letter frequency analysis
char* singleLetterAnalysis(text* t, char* cipherKey);

//Score the fitness of the text to written English
float scoreQuadgram(text* t, dict* d);

//Substitute the text using the inputed key
void substitute(text* t, char* key);

//Sort numbers in descending order and return the indexes
void indexSort(float* data, int size, int* sortedIndexes);

#endif /* FREQANALYSIS_H_ */
