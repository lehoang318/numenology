#include <stdio.h>
#include <string.h>

#include "numerology.h"

boolean parse_single_number_ut() {
    // Day of month & Month
    for (int d = 1; 31 >= d; d++) {
        printf("parse_single_number(%d, FALSE) => %d\n", d, parse_single_number(d, FALSE));
        printf("parse_single_number(%d, TRUE) => %d\n", d, parse_single_number(d, TRUE));
    }

    // Year
    for (int y = 1980; 2000 > y; y++) {
        printf("parse_single_number(%d, FALSE) => %d\n", y, parse_single_number(y, FALSE));
        printf("parse_single_number(%d, TRUE) => %d\n", y, parse_single_number(y, TRUE));
    }

    return TRUE;
}

int main(void) {
    dob_t dob = {31, 8, 1988};
    parse_birth_date(&dob);

    char * full_name = "Le Huy Hoang";
    parse_name(full_name, strlen(full_name));

    return 0;
}
