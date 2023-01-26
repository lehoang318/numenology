#ifndef _NUMEROLOGY_HPP_
#define _NUMEROLOGY_HPP_

#include "person.hpp"

#include <array>
#include <vector>
#include <string>

namespace numerology {

constexpr int INVALID_NUMBER = -1;

struct PersonalNumbers {
public:
    static PersonalNumbers Analyze(Date dob, int target_day, int target_month, int target_year);

    PersonalNumbers() : mYear(INVALID_NUMBER), mMonth(INVALID_NUMBER), mDay(INVALID_NUMBER) {}
    ~PersonalNumbers() {}

    std::string toString();

    int mDay;
    int mMonth;
    int mYear;
};  // struct PersonalNumbers

class Numbers {
public:
    static Numbers Analyze(const Person& personalInformation);

    Numbers() : mDoBInitialized(false), mNameInitialized(false) {}
    ~Numbers() {}

    bool isFullyInitialized();
    void updateDoB(Date dob);
    void updateNames(std::string fullname, std::string shortname="");

    std::string toString();

    // Based-on your birth date
    int mLifePath;
    int mBirthDay;
    std::array<int, 4> mChallenges;

    std::array<int, 3> mPeriodCycles;
    std::array<int, 4> mPinnacleCycles;

    // Based-on your names
    int mMajorExpression;
    int mMinorExpression;

    int mMajorDesire;
    int mMinorDesire;

    int mMajorPersonality;
    int mMinorPersonality;

    int mHiddenPassion;
    int mHabitChallenge;
    int mBalance;
    // int mExpressionMethod;

    // Based-on DoB & Name
    // int mBridge;
    // int mMaturity;
    // int mRationalThought;

    std::vector<int> mKarmicDebts;

private:
    void synchronize();

    bool mDoBInitialized;
    bool mNameInitialized;
    Person mPerson;

};  // class Numbers

}   // namespace numerology

#endif  // _NUMEROLOGY_HPP_
