/*
 * text.h
 *
 *  Created on: Nov 16, 2016
 *      Author: Jerry
 */

#ifndef TEXT_H_
#define TEXT_H_

//Text file definition
typedef struct text {
	int size;
	int curLine;
	int maxLine;
	char** data;
} text;

//Create a text object, if filename is null, then
//an empty text object is generated
text* createText(char* filename, int maxLine);

//append a line to the text object
void addLine(text* t, char* line, int lineLength);

//delete the text object
void deleteText(text* t);

//prints all the lines of the text object to the console
void printText(text* t);

text* copyText(text* t);

#endif /* TEXT_H_ */
