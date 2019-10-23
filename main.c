#include "dictionary.h"
#include <stdlib.h>

void clear_malloced_llist_array(hashmap_t hashtable[]){
    // int free_counter = 0;
    hashmap_t cursor = NULL;
    hashmap_t next = NULL;
    for (int i = 0; i < HASH_SIZE; i++){
        cursor = hashtable[i];
        while(cursor){
            // printf("in while\n");
            next = cursor->next;
            free(cursor);
            // free_counter++;
            cursor = next;
        }
        free(cursor);
    }
}

void clear_char_array(int num_misspelled, char * misspelled[]){
    // int free_counter = 0;
    for (int i = 0; i < num_misspelled; i++){
        free(misspelled[i]);
        // free_counter++;
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("This function was expecting 2 arguments:\nSample Call:\n\t./spell_check /path/to/dictionary.txt /path/to/input.txt\n");
        return 0;
    }
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(argv[1], hashtable);
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen(argv[2], "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    printf("There were %d misspelled words in your input text.\n", num_misspelled);
    printf("Misspelled Words:\n");
    for (int i = 0; i < num_misspelled; i++){
        printf("%s\n", misspelled[i]);
    }


    clear_malloced_llist_array(hashtable);
    clear_char_array(num_misspelled, misspelled);
    fclose(fp);
    
    return 0;
}
