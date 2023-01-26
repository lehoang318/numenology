#ifndef _PERSON_HPP_
#define _PERSON_HPP_

#include <string>

namespace numerology {
struct Date {
    Date() {}
    Date(int day_of_month, int month, int year) {
        this->mDay = day_of_month;
        this->mMonth = month;
        this->mYear = year;
    }
    ~Date() {}

    int mDay;   // Day of month [1;31]
    int mMonth; // Month of year [1:12]
    int mYear;
};  // struct Date

struct Person {
    Person() {}
    Person(Date dob, std::string fullname, std::string shortname) {
        this->mDob = dob;
        this->mFullName = fullname;
        this->mShortName = shortname;
    }

    ~Person() {}

    static Person Create(int day_of_month, int month, int year, std::string fullname, std::string shortname) {
        Date dob(day_of_month, month, year);
        return Person(dob, fullname, shortname);
    }

    Date mDob;  // Date of birth
    std::string mFullName;
    std::string mShortName;
};  // struct Person

}   // namespace numenology

#endif  // _PERSON_HPP_
