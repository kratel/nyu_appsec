#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]){
	//First Test
	int i;
	i = 1;
	return i;
}

bool check_word(const char* word, hashmap_t hashtable[]){
	//First Test
	bool i;
	i = false;
	return i;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
	// Init bool var
	bool loaded;
	// Set all elements in hashtable to NULL
	for (int i = 0; i < HASH_SIZE; i++)
	{
		hashtable[i]= NULL;
	}
	// Try to open dictionary
	FILE *fp;
	fp = fopen(dictionary_file, "r");
	// If dictionary could not be opened return with false
	if (!fp)
	{
		loaded = false;
		return loaded;
	}
	int counter = 0;
	int hashctr = 0;
	int c;
	int bucket;
	char w[LENGTH];
	char* word;
	word = w;
	while ((c = fgetc(fp)) != EOF){
		// Check if word is too long
		if (counter > LENGTH)
		{
			/* might not be neccessary if dictionaries are expected to have words smaller than 45 chars*/
			while (c != EOF && c == '\r' && c == '\n' && c == '\t' && c == ' ') {
				c = fgetc(fp);
				if (c == EOF)
				{
					fclose(fp);
					return true;
				}
			}
			word[0] = '\0';
			counter = 0;
		} else {
			/* check if c is \r \n \t or whitespace which means end of word */
			if (c == '\r' || c == '\n' || c == '\t' || c == ' '){
				/* add word in */
				hashmap_t new_node = (hashmap_t) malloc(45 * sizeof(char));
				new_node->next = NULL;
				word[counter] = '\0';
				strcpy(new_node->word,word);
				bucket = hash_function(word);
				if (hashtable[bucket] != NULL){
					new_node->next = hashtable[bucket];
				}
				hashtable[bucket] = new_node;
				word[0] = '\0';
				counter = 0;
			} else {
				/* add to word */
				word[counter] = c;
				counter++;
			}
		}
	}
	fclose(fp);
	return true;
}
