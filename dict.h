/*
 * dict.h
 *
 *  Created on: Nov 17, 2016
 *      Author: Jerry
 */

#ifndef DICT_H_
#define DICT_H_

typedef struct entry { /* table entry: */
    struct entry *next; /* next entry in chain */
    char *name; /* defined name (quadgram in our case)*/
    float defn; /* replacement text or probability in our use*/
} entry;

typedef struct dict {
	int tableSize;
	entry** entries;
} dict;

//Create a dictionary object (hashmap)
dict* createDictionary(int tableSize);

//Form hash value for string s
unsigned hash(dict* d, char *s);

//Find a string in the dictionary
entry* lookup(dict* d, char *s);

//Add an entry to the dictionary
void install(dict* d, char *name, float defn);

//Delete the dictionary
void deleteDictionary(dict* d);

#endif /* DICT_H_ */
