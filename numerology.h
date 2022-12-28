#ifndef __NUMEROLOGY_H__
#define __NUMEROLOGY_H__

typedef enum {
    FALSE = 0,
    TRUE
} boolean;

typedef struct {
    int day_of_month;
    int month;
    int year;
} dob_t;

typedef struct {
    char * first_char_ptr;
    int char_count;
} word_t;

typedef struct {
    word_t * words_ptr;
    int word_count;
} name_t;

typedef struct {
    dob_t date_of_birth;
    name_t full_name;
} personal_info_t;

typedef struct {
    int first;
    int second;
    int third;
    int fourth;
} challenge_numbers_t;

typedef struct {
    int life_path_number;
    int birth_number;
    challenge_numbers_t challenge_numbers;
} birth_date_numbers_t;

typedef struct {
    int major;
    int minor;
} name_number_t;

typedef struct {
    name_number_t expression_number;
    name_number_t desire_number;
    name_number_t personality_number;
} name_numbers_t;

typedef struct {
    birth_date_numbers_t birth_date_numbers;
    name_numbers_t name_numbers;
} personal_numbers_t;

int parse_single_number(int number, boolean to_keep_master_number);
int parse_single_word(word_t * word_ptr);

birth_date_numbers_t parse_birth_date(dob_t * date_of_birth_ptr);
name_numbers_t parse_name(char * full_name_ptr, int length);

personal_numbers_t analyze(personal_info_t * person_info_ptr);

#endif // __NUMEROLOGY_H__
