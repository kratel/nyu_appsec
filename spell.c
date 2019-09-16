#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]){
	int num_misspelled = 0;
	int counter = 0;
	int c;
	char w[LENGTH];
	char* word;
	word = w;
	while ((c = fgetc(fp)) != EOF){
		// Check if word is too long
		if (counter > LENGTH){
			// Keep moving pointer to end of the word in file. This word is counted as misspelled.
			while (c != EOF && c != '\r' && c != '\n' && c != '\t' && c != ' '){
				word[counter] = c;
				counter++;
				c = fgetc(fp);
				if (c == EOF){
					word[counter] = '\0';
					misspelled[num_misspelled] = (char *) malloc(counter * sizeof(char));
					strcpy(misspelled[num_misspelled],word);
					num_misspelled++;
					return num_misspelled;
				}
			}
			word[counter] = '\0';
			misspelled[num_misspelled] = (char *) malloc(counter * sizeof(char));
			strcpy(misspelled[num_misspelled],word);
			num_misspelled++;
			word[0] = '\0';
			counter = 0;
		} else {
			/* check if c is \r \n \t or whitespace which means end of word */
			if (c == '\r' || c == '\n' || c == '\t' || c == ' '){
				/* check word */
				word[counter] = '\0';
				if (!check_word(word, hashtable)){
					// add to misspelled array
					// need to malloc memory
					misspelled[num_misspelled] = (char *) malloc(LENGTH * sizeof(char));
					strcpy(misspelled[num_misspelled],word);
					num_misspelled++;
				}
			} else {
				/* add to word */
				word[counter] = c;
				counter++;
			}
		}
	}
	printf("test\n");
	return num_misspelled;
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
	for (int i = 0; i < HASH_SIZE; i++){
		hashtable[i]= NULL;
	}
	// Try to open dictionary
	FILE *fp;
	fp = fopen(dictionary_file, "r");
	// If dictionary could not be opened return with false
	if (!fp){
		loaded = false;
		return loaded;
	}
	int counter = 0;
	int c;
	int bucket;
	char w[LENGTH];
	char* word;
	word = w;
	while ((c = fgetc(fp)) != EOF){
		// Check if word is too long
		if (counter > LENGTH){
			/* might not be neccessary if dictionaries are expected to have words smaller than 45 chars*/
			// Keep moving pointer to end of the word in file. This word won't be added to hashtable.
			while (c != EOF && c != '\r' && c != '\n' && c != '\t' && c != ' '){
				c = fgetc(fp);
				if (c == EOF){
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
				hashmap_t new_node = (hashmap_t) malloc(LENGTH * sizeof(char));
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
