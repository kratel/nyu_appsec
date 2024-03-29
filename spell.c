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

char *wordtolower(const char *word){
	char tword[LENGTH + 1];
	char *tw = tword;
	for(int i = 0; i < strlen(word); i++){
		tw[i] = tolower(word[i]);
	}
	tw[strlen(word)] = '\0';
	return tw;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]){
	// clear misspelled array since we always start count from 0
	for (int i=0; i < MAX_MISSPELLED; i++){
        misspelled[i] = NULL;
        free(misspelled[i]);
    }
	int num_misspelled = 0;
	int counter = 0;
	int c;
	char w[LENGTH + 1];
	char* word;
	word = w;
	word[LENGTH] = '\0';
	char tr[7];
	char * trunc_suffix;
	trunc_suffix = tr;
	trunc_suffix = "_TRUNC";
	// int malloc_counter = 0;
	while ((c = fgetc(fp)) != EOF || counter != 0){
		if (num_misspelled >= MAX_MISSPELLED){
			// Reached max limit
			break;
		}
		// Check if word is too long
		if (counter > LENGTH){
			// Keep moving pointer to end of the word in file. This word is truncated and counted as misspelled.
			strcpy(&word[LENGTH - strlen(trunc_suffix)],trunc_suffix);
			misspelled[num_misspelled] = (char *) malloc((LENGTH + 1) * sizeof(char));
			// malloc_counter++;
			strcpy(misspelled[num_misspelled],word);
			num_misspelled++;
			while (c != EOF && c != '\r' && c != '\n' && c != '\t' && c != ' '){
				c = fgetc(fp);
				if (c == EOF){
					return num_misspelled;
				}
			}
			word[0] = '\0';
			counter = 0;
		}
		/* check if c is \r \n \t or whitespace which means end of word
		   and that counter is not 0 meaning we have a word.
		   If reached EOF do final check if we currently have a word.*/
		if (counter != 0 && (c == '\r' || c == '\n' || c == '\t' || c == ' ' || c == EOF)){
			/* check word */
			word[counter] = '\0';
			char *b = strip_punct(word);
			if (strlen(b) == 0){
				// Word was actually pure punctation, ignore, not misspelled.
				;
			} else if (!check_word(b, hashtable)){
				// add to misspelled array
				// need to malloc memory
				misspelled[num_misspelled] = (char *) malloc((LENGTH + 1) * sizeof(char));
				// malloc_counter++;
				strcpy(misspelled[num_misspelled],b);
				num_misspelled++;
			}
			word[0] = '\0';
			counter = 0;
		} else {
			if (counter == 0 && (c == '\r' || c == '\n' || c == '\t' || c == ' ')){
				/* Don't start building a word until we get an appropriate char */
				continue;
			} else {
				/* add to word */
				word[counter] = c;
				counter++;
			}
		}
	}
	// printf("%d\n", num_misspelled);
	// printf("There were %d malloc calls in this run of check_words\n", malloc_counter);
	return num_misspelled;
}

bool is_valid_num_string(const char* num_string){
	int l = strlen(num_string) - 1;
	char c;
	for (int i = 0; i <= l; ++i){
		if (isdigit(num_string[i])){
			continue;
		}
		if (ispunct(num_string[i]) && i != 0){
			c = num_string[i];
			// Check if valid punct
			if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == ',' || c == '.' || c == '(' || c == ')' || c == '=' || c == '!'){
				continue;
			} else {
				return false;
			}
		}
		if (isalpha(num_string[i]) && i != l){
			c = num_string[i];
			/* Check if valid letter: 
				'e' and 'E' for scientific notation shorthand
				'i' for imaginary*/
			if (c == 'e' || c == 'E' || c == 'i'){
				continue;
			} else {
				return false;
			}
		}
		if (isalpha(num_string[i]) && i == l){
			c = num_string[i];
			// If a letter is the last char then it can only be 'i'
			if (c == 'i'){
				continue;
			} else {
				return false;
			}
		}
	}
	return true;
}

bool check_word(const char* word, hashmap_t hashtable[]){
	// printf("Word converted from: %s to %s\n", word, nb);
	if (isdigit(word[0])){
		/* if first char is a digit treat this word as a number */
		if (is_valid_num_string(word)){
			return true;
		}
		// if not then don't return false, just in case a leet-speak dictionary was used.
	}
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
	char tword[LENGTH + 1];
	char *tw = tword;
	tw = wordtolower(word);
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
	// Set all elements in hashtable to NULL
	for (int i = 0; i < HASH_SIZE; i++){
		hashtable[i]= NULL;
	}
	// Try to open dictionary
	FILE *fp;
	fp = fopen(dictionary_file, "r");
	// If dictionary could not be opened return with false
	if (!fp){
		return false;
	}
	int counter = 0;
	int c;
	int bucket;
	char w[LENGTH + 1];
	char* word;
	word = w;
	// int malloc_counter = 0;
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
		}
		/* check if c is \r \n \t or whitespace which means end of word
		   and that counter is not 0 meaning we have a word. */
		if (counter != 0 && (c == '\r' || c == '\n' || c == '\t' || c == ' ')){
			/* add word in */
			hashmap_t new_node = (hashmap_t) malloc(sizeof(node));
			// malloc_counter++;
			new_node->next = NULL;
			word[counter] = '\0';
			strcpy(new_node->word,word);
			bucket = hash_function(word);
			if (bucket < HASH_SIZE) {
				if (hashtable[bucket] != NULL){
					new_node->next = hashtable[bucket];
				}
				hashtable[bucket] = new_node;
			}
			word[0] = '\0';
			counter = 0;
		} else {
			if (counter == 0 && (c == '\r' || c == '\n' || c == '\t' || c == ' ')){
				/* Don't start building a word until we get an appropriate char */
				continue;
			} else {
				/* add to word */
				word[counter] = c;
				counter++;
			}
		}
	}
	fclose(fp);
	// printf("There were %d malloc calls during this run of load_dictionary\n", malloc_counter);
	return true;
}
