/*
 * dict.c
 *
 *  Created on: Nov 17, 2016
 *      Author: Jerry
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dict.h"


//Create a dictionary object (hashmap)
dict* createDictionary(int tableSize) {
	dict* d = (dict*) malloc(sizeof(dict));
	d->tableSize = tableSize;
	d->entries = (entry**) malloc(tableSize * sizeof(entry*));

	for (int i = 0; i < tableSize; i++) {
		d->entries[i] = NULL;

	}

	return d;
}

//Form hash value for string s
unsigned hash(dict* d, char *s) {
	unsigned hashval;

	for (int i = 0; i < strlen(s); i++) {
		s[i] = tolower(s[i]);
	}

	//Interesting for loop syntax we found online
	//Code starts with hashval of zero, and as long as the character is not
	//null the character is used to hash and then the memory address is
	//incremented
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % d->tableSize;
}

//Find a string in the dictionary
entry* lookup(dict* d, char *s) {
	entry* np;
	for (np = d->entries[hash(d, s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np; /* found */
	return NULL; /* not found */
}

//Add an entry to the dictionary
void install(dict* d, char *name, float defn) {
	entry* np;
	unsigned hashval;
	if ((np = lookup(d, name)) == NULL) { /* not found */
		np = (entry *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL) {
			printf("Out of Memory\n");
			exit(1);
		}
		np->defn = defn;
		hashval = hash(d, name);
		np->next = d->entries[hashval];
		d->entries[hashval] = np;
	} else {
		/* already there */
		np->defn = defn;
	}
}

//Delete the dictionary
void deleteDictionary(dict* d) {
	for (int i = 0; i < d->tableSize; i++) {
		entry* e = d->entries[i];
		while (e != NULL) {
			d->entries[i] = e->next;
			free(e->name);
			free(e);
			e = d->entries[i];
		}
	}
	free(d->entries);
	free(d);
}
