#include "dictionary.h"
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(argv[1], hashtable);
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen(argv[2], "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    printf("There were %d misspelled words in your input file '%s' found using dictionary '%s'\n", num_misspelled, argv[2], argv[1]);
    
    return 0;
}
