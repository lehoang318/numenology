#include <stdio.h>
#include <stdlib.h>

#include "numerology.h"

#define _abs_diff(a, b) ((a > b) ? (a - b) : (b - a))
#define is_valid_abc(c) ((('A' <= c) && ('Z' >= c)) || (('a' <= c) && ('z' >= c)))
#define to_lowercase(c) ((('A' <= c) && ('Z' >= c)) ? (c + ('a' - 'A')) : c)
#define char2number(c)  (((to_lowercase(c) - 'a') % 9) + 1)

int parse_single_number(int number, boolean to_keep_master_number) {
    if (10 > number) {
        return number;
    } else if ((to_keep_master_number) && ((11 ==  number) || (22 == number))) {
        return number;
    }

    int derived = 0;
    while (0 < number) {
        derived += number % 10;
        number /= 10;
    }

    return parse_single_number(derived, to_keep_master_number);
}

int parse_single_word(word_t * word_ptr) {
    int word_number = 0;
    for (int i = 0; word_ptr->char_count > i; i++) {
        int char_number = char2number(to_lowercase(word_ptr->first_char_ptr[i]));
        word_number += char_number;
    }

    return parse_single_number(word_number, TRUE);
}

birth_date_numbers_t parse_birth_date(dob_t * date_of_birth_ptr) {
    birth_date_numbers_t birth_date_numbers;

    int buf[3];

    buf[0] = parse_single_number(date_of_birth_ptr->day_of_month, TRUE);
    printf("Day of month: %d => %d\n", date_of_birth_ptr->day_of_month, buf[0]);
    buf[1] = parse_single_number(date_of_birth_ptr->month, TRUE);
    printf("Month: %d => %d\n", date_of_birth_ptr->month, buf[1]);
    buf[2] = parse_single_number(date_of_birth_ptr->year, TRUE);
    printf("Year: %d => %d\n", date_of_birth_ptr->year, buf[2]);

    birth_date_numbers.life_path_number = parse_single_number(buf[0] + buf[1] + buf[2], TRUE);
    printf("=> Life path number: %d\n", birth_date_numbers.life_path_number);

    birth_date_numbers.birth_number = date_of_birth_ptr->day_of_month;
    printf("=> Birth number: %d\n", birth_date_numbers.birth_number);

    // Challenge numbers
    buf[0] = parse_single_number(date_of_birth_ptr->day_of_month, FALSE);
    buf[1] = parse_single_number(date_of_birth_ptr->month, FALSE);
    buf[2] = parse_single_number(date_of_birth_ptr->year, FALSE);
    birth_date_numbers.challenge_numbers.first = _abs_diff(buf[0], buf[1]);
    printf("=> 1st Challenge number: %d\n", birth_date_numbers.challenge_numbers.first);

    birth_date_numbers.challenge_numbers.second = _abs_diff(buf[0], buf[2]);
    printf("=> 2nd Challenge number: %d\n", birth_date_numbers.challenge_numbers.second);

    birth_date_numbers.challenge_numbers.third = _abs_diff(
        birth_date_numbers.challenge_numbers.first, birth_date_numbers.challenge_numbers.second
    );
    printf("=> 3rd Challenge number: %d\n", birth_date_numbers.challenge_numbers.third);

    birth_date_numbers.challenge_numbers.fourth = _abs_diff(buf[1], buf[2]);
    printf("=> 4th Challenge number: %d\n", birth_date_numbers.challenge_numbers.fourth);

    return birth_date_numbers;
}

name_numbers_t parse_name(char * full_name_ptr, int length) {
    name_numbers_t name_numbers;
    int derived_number = -1;
    word_t tmp_word;

    name_numbers.expression_number.major = 0;
    name_numbers.expression_number.minor = -1;
    int i = 0;
    while (length > i) {
        if (is_valid_abc(full_name_ptr[i])) {
            printf("`");
            tmp_word.first_char_ptr = full_name_ptr + i;
            tmp_word.char_count = 0;

            while (length > i) {
                if (is_valid_abc(full_name_ptr[i])) {
                    printf("%c", full_name_ptr[i]);
                    tmp_word.char_count++;
                    i++;
                } else {
                    break;
                }
            }

            derived_number = parse_single_word(&tmp_word);
            printf("` => %d\n", derived_number);
            if (0 > name_numbers.expression_number.minor) {
                name_numbers.expression_number.minor = derived_number;
            }
            name_numbers.expression_number.major += derived_number;
        }
        i++;
    }

    name_numbers.expression_number.major = parse_single_number(
        name_numbers.expression_number.major, TRUE
    );

    if (0 < derived_number) {
        name_numbers.expression_number.minor += derived_number;
    }
    name_numbers.expression_number.minor = parse_single_number(
        name_numbers.expression_number.minor, TRUE
    );

    printf("=> Major expression number: %d\n", name_numbers.expression_number.major);
    printf("=> Minor expression number: %d\n", name_numbers.expression_number.minor);

    return name_numbers;
}

// personal_numbers_t analyze(personal_info_t * person_info_ptr) {
//     personal_numbers_t personal_numbers;
//     personal_numbers.birth_date_numbers = parse_birth_date(&person_info_ptr->date_of_birth);
//     personal_numbers.name_numbers = parse_name(&person_info_ptr->full_name);

//     return personal_numbers;
// }
