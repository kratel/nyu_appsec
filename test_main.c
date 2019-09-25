#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "wordlists/wordlist.txt"
#define TESTDICT "wordlists/test_wordlist.txt"
#define TESTDICTLONG "wordlists/test_wordlist2.txt"
#define TESTDICTOF "wordlists/test_wordlist3.txt"
#define TESTINPUT "inputs/test1.txt"
#define TESTINPUT2 "inputs/test2.txt"
#define TESTINPUT3 "inputs/test3.txt"
#define TESTINPUT4 "inputs/test4.txt"
#define TESTINPUT5 "inputs/test5.txt"
#define TESTINPUT6 "inputs/test6.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    char* expected[3];
    expected[0] = "first";
    expected[1] = "second";
    expected[2] = "third";
    for (int i = 0; i < 3; ++i)
    {
        ck_assert(strcmp(expected[i],hashtable[hash_function(expected[i])]->word) == 0);
    }
}
END_TEST

START_TEST(test_dictionary_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICTOF, hashtable));
    char* expected[4];
    expected[0] = "bad";
    expected[1] = "apple";
    expected[2] = "peach";
    expected[3] = "duck";
    for (int i = 0; i < 4; ++i)
    {
        ck_assert(strcmp(expected[i],hashtable[hash_function(expected[i])]->word) == 0);
    }
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    // Test here: What if a word begins and ends with "? 
    // ^ Invalid request check_word does not modify the character and quotes would have been stripped out before.
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen(TESTINPUT, "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
    fclose(fp);
    // Test how a pure punctuation word acts, should be ignored
    fp = fopen(TESTINPUT3, "r");
    num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    test = strlen(misspelled[0]) == strlen(expected[0]);
    len1 = strlen(misspelled[0]);
    len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
    fclose(fp);
}
END_TEST

START_TEST(test_check_words_irregular_spacing)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen(TESTINPUT2, "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
}
END_TEST

START_TEST(test_check_words_overflow)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[4];
    expected[0] = "pneumonoultramasdfdsicroscosadawedapicsilisaw";
    expected[1] = "sogn";
    expected[2] = "skyn";
    expected[3] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen(TESTINPUT4, "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 4);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
    ck_assert_msg(strcmp(misspelled[3], expected[3]) == 0);
    fclose(fp);
    // Test if word is at end of file
    fp = fopen(TESTINPUT5, "r");
    for (int i=0; i < 4; i++){
        misspelled[i] = NULL;
    }
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    expected[3] = "pneumonoultramasdfdsicroscosadawedapicsilisaw";
    num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 4);
    test = strlen(misspelled[0]) == strlen(expected[0]);
    len1 = strlen(misspelled[0]);
    len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0);
    ck_assert_msg(strcmp(misspelled[3], expected[3]) == 0);
    fclose(fp);
    // Test overflow of misspelled words
    fp = fopen(TESTINPUT6, "r");
    num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 1000);
    fclose(fp);
}
END_TEST

START_TEST(test_check_word_numbers)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_number = "22";
    const char* correct_number_dec = "3.14934";
    const char* correct_number_sep1 = "1,200,468";
    const char* correct_number_sep2 = "1.200.468";
    const char* correct_number_phone = "1-200-867-5309";
    const char* correct_number_phone_area = "1-(200)-867-5309";
    const char* correct_number_eq = "5^3=125";
    const char* correct_number_eq2 = "5+(3-2)=6";
    const char* correct_number_eq3 = "5*(4/2)=10";
    const char* correct_number_eq4 = "3!=6";
    const char* no_leet = "3l3ph4nt";
    ck_assert(check_word(correct_number, hashtable));
    ck_assert(check_word(correct_number_dec, hashtable));
    ck_assert(check_word(correct_number_sep1, hashtable));
    ck_assert(check_word(correct_number_sep2, hashtable));
    ck_assert(check_word(correct_number_phone, hashtable));
    ck_assert(check_word(correct_number_phone_area, hashtable));
    ck_assert(check_word(correct_number_eq, hashtable));
    ck_assert(check_word(correct_number_eq2, hashtable));
    ck_assert(check_word(correct_number_eq3, hashtable));
    ck_assert(check_word(correct_number_eq4, hashtable));
    ck_assert(!check_word(no_leet, hashtable));
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("check_word");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_dictionary_overflow);
    tcase_add_test(check_word_case, test_check_words_irregular_spacing);
    tcase_add_test(check_word_case, test_check_words_overflow);
    tcase_add_test(check_word_case, test_check_word_numbers);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

