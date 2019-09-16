#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"


char *strip_punct(char *word){
	// must use b as new word start.
	char *b = word;
	while (ispunct(*b)){
		b++;
	}
	// Strip puncation at end of word
	//  pointer to last char in word
	char *e = word + strlen(word) - 1;
	//  keep going backwards setting pointer to 0 as punct is found
	//. make sure you don't go past the new beggining of word.
	while (ispunct(*e) && b < e) {
		*e = 0;
		e--;
	}
	return b;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]){
	int num_misspelled = 0;
	int counter = 0;
	int c;
	char w[LENGTH + 1];
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
					misspelled[num_misspelled] = (char *) malloc((counter + 1) * sizeof(char));
					strcpy(misspelled[num_misspelled],word);
					num_misspelled++;
					return num_misspelled;
				}
			}
			word[counter] = '\0';
			misspelled[num_misspelled] = (char *) malloc((counter + 1) * sizeof(char));
			strcpy(misspelled[num_misspelled],word);
			num_misspelled++;
			word[0] = '\0';
			counter = 0;
		} else {
			/* check if c is \r \n \t or whitespace which means end of word */
			if (c == '\r' || c == '\n' || c == '\t' || c == ' '){
				/* check word */
				word[counter] = '\0';
				char *b = strip_punct(word);
				if (!check_word(b, hashtable)){
					// add to misspelled array
					// need to malloc memory
					misspelled[num_misspelled] = (char *) malloc((LENGTH + 1) * sizeof(char));
					strcpy(misspelled[num_misspelled],b);
					num_misspelled++;
				}
				word[0] = '\0';
				counter = 0;
			} else {
				/* add to word */
				word[counter] = c;
				counter++;
			}
		}
	}
	// printf("%d\n", num_misspelled);
	return num_misspelled;
}

bool check_word(const char* word, hashmap_t hashtable[]){
	// printf("Word converted from: %s to %s\n", word, nb);
	int bucket;
	bucket = hash_function(word);
	hashmap_t cursor = hashtable[bucket];
	while(cursor){
		// check if word is at cursor
		// printf("checking if word matches cursor: %s\n", cursor->word);
		if (strcmp(cursor->word, word) == 0){
			// printf("\tMatched original\n");
			return true;
		}
		cursor = cursor->next;
	}
	char tword[LENGTH];
	char *tw = tword;
	for(int i = 0; i < strlen(word); i++){
		tw[i] = tolower(word[i]);
	}
	tw[strlen(word)] = '\0';
	bucket = hash_function(tw);
	cursor = hashtable[bucket];
	while(cursor){
		// check if word is at cursor
		// printf("checking if word matches cursor: %s\n", cursor->word);
		if (strcmp(cursor->word, tw) == 0){
			// printf("\tMatched lowercase\n");
			return true;
		}
		cursor = cursor->next;
	}
	return false;
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
				hashmap_t new_node = (hashmap_t) malloc(sizeof(node));
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
