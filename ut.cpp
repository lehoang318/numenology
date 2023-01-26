#include "numerology.hpp"
#include "util.hpp"

int main() {
    logi("Unit Testing\n");

    numerology::Person personalInfo = numerology::Person::Create(31, 8, 1988, "Le Huy Hoang", "Le Hoang");
    // numerology::Person personalInfo = numerology::Person::Create(25, 12, 1964, "Thomas John Hancock", "Tom Hancock");
    // numerology::Person personalInfo = numerology::Person::Create(15, 5, 1949, "Thomas John Hancock", "Tom Hancock");
    numerology::Numbers numbers = numerology::Numbers::Analyze(personalInfo);
    numerology::PersonalNumbers personalNumbers = numerology::PersonalNumbers::Analyze(personalInfo.mDob, 27, 1, 2023);

    logi(numbers.toString().c_str());
    logi(personalNumbers.toString().c_str());

    return 0;
}
