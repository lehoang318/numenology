#include "numerology.hpp"
#include "person.hpp"
#include "util.hpp"

#include <array>
#include <vector>
#include <string>
#include <sstream>

#define _abs_diff(a, b) ((a > b) ? (a - b) : (b - a))
#define is_valid_abc(c) ((('A' <= c) && ('Z' >= c)) || (('a' <= c) && ('z' >= c)))
#define to_lowercase(c) ((('A' <= c) && ('Z' >= c)) ? (c + ('a' - 'A')) : c)
#define char2number(c)  (((to_lowercase(c) - 'a') % 9) + 1)

/**************************************************************************************************
 * LOCAL FUNCTIONS
**************************************************************************************************/
static int parse_single_number(int number, bool to_keep_master_number) {
    if (10 > number) {
        return number;
    } else if (to_keep_master_number && ((11 ==  number) || (22 == number))) {
        return number;
    }

    int derived = 0;
    while (0 < number) {
        derived += number % 10;
        number /= 10;
    }

    return parse_single_number(derived, to_keep_master_number);
}

static int parse_single_word(const std::string word) {
    int word_number = 0;
    for (char c : word) {
        int char_number = char2number(to_lowercase(c));
        word_number += char_number;
    }

    return parse_single_number(word_number, true);
}

static std::vector<std::string> split(const std::string input, const char delim=' ') {
    std::vector<std::string> tokens;
    std::string tmp;

    for (int pos = 0; pos < input.length(); pos++) {
        if (delim == input[pos]) {
            if (!tmp.empty()) {
                tokens.push_back(tmp);
                tmp.clear();
            }
        } else {
            tmp += input[pos];
        }
    }

    if (!tmp.empty()) {
        tokens.push_back(tmp);
    }

    return tokens;
}

static bool is_vowel(char c, bool is_first_char=false) {
    if (!is_valid_abc(c)) {
        return false;    
    }

    c = to_lowercase(c);

    switch (c) {
        case 'y':
            if (is_first_char) {
                break;
            }

        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            logd("`%c` is vowel!\n", c);
            return true;

        default:
            break;
    }

    logd("`%c` is not vowel!\n", c);

    return false;
}

/**************************************************************************************************
 * CLASS FUNCTIONS
**************************************************************************************************/
numerology::PersonalNumbers numerology::PersonalNumbers::Analyze(Date dob, int target_day, int target_month, int target_year) {
    numerology::PersonalNumbers personalNumbers;

    std::array<int, 3> buf;
    buf[0] = parse_single_number(dob.mDay, false);
    buf[1] = parse_single_number(dob.mMonth, false);
    buf[2] = parse_single_number(target_year, false);

    personalNumbers.mYear = parse_single_number(buf[0] + buf[1] + buf[2], false);
    if (INVALID_NUMBER == personalNumbers.mYear) {
        loge("Could not calculate Personal Year!!!\n");
        return personalNumbers;
    }

    buf[1] = parse_single_number(target_month, false);

    personalNumbers.mMonth = parse_single_number(personalNumbers.mYear + buf[1], false);
    if (INVALID_NUMBER == personalNumbers.mMonth) {
        loge("Could not calculate Personal Month!!!\n");
        return personalNumbers;
    }

    buf[0] = parse_single_number(target_day, false);

    personalNumbers.mDay = parse_single_number(personalNumbers.mMonth + buf[0], false);

    return personalNumbers;
}

std::string numerology::PersonalNumbers::toString() {
    std::ostringstream ss;
    ss << "Personal numbers:" << std::endl;
    ss << " * Year: " << this->mYear << std::endl;
    ss << " * Month: " << this->mMonth << std::endl;
    ss << " * Day: " << this->mDay << std::endl;

    return ss.str();
}

bool numerology::Numbers::isFullyInitialized() {
    return mDoBInitialized && mNameInitialized;
}

void numerology::Numbers::synchronize() {}

void numerology::Numbers::updateDoB(Date dob) {
    std::array<int, 3> buf;
    buf[0] = parse_single_number(dob.mDay, true);
    buf[1] = parse_single_number(dob.mMonth, true);
    buf[2] = parse_single_number(dob.mYear, true);

    mPerson.mDob = dob;
    mDoBInitialized = true;

    /************************************************/
    this->mLifePath = parse_single_number(buf[0] + buf[1] + buf[2], true);

    /************************************************/
    this->mBirthDay = dob.mDay;

    /************************************************
     * Challenge numbers
     */
    buf[0] = parse_single_number(dob.mDay, false);
    buf[1] = parse_single_number(dob.mMonth, false);
    buf[2] = parse_single_number(dob.mYear, false);
    this->mChallenges[0] = _abs_diff(buf[0], buf[1]);

    this->mChallenges[1] = _abs_diff(buf[0], buf[2]);

    this->mChallenges[2] = _abs_diff(
        this->mChallenges[0], this->mChallenges[1]
    );

    this->mChallenges[3] = _abs_diff(buf[1], buf[2]);

    /************************************************
     * Cycle numbers
     */
    this->mPeriodCycles[0] = parse_single_number(dob.mMonth, true);
    this->mPeriodCycles[1] = parse_single_number(dob.mDay, true);
    this->mPeriodCycles[2] = parse_single_number(dob.mYear, true);

    this->mPinnacleCycles[0] = parse_single_number(this->mPeriodCycles[0] + this->mPeriodCycles[1], true);  // Month & Day
    this->mPinnacleCycles[1] = parse_single_number(this->mPeriodCycles[1] + this->mPeriodCycles[2], true);  // Day & Year
    this->mPinnacleCycles[2] = this->mPinnacleCycles[0] + this->mPinnacleCycles[1];
    this->mPinnacleCycles[3] = parse_single_number(this->mPeriodCycles[0] + this->mPeriodCycles[2], true);  // Month & Year

    // Update composition numbers
    if (mNameInitialized) {
        synchronize();
    }
}

void numerology::Numbers::updateNames(std::string fullname, std::string shortname) {
    // TODO
    mPerson.mFullName = fullname;
    mPerson.mShortName = shortname;
    mNameInitialized = true;

    /************************************************
     * Major expression number
     */
    {
        std::vector<std::string> tokens = split(fullname);
        int tmp = 0;
        for (auto t : tokens) {
            tmp += parse_single_word(t);
        }
        this->mMajorExpression = parse_single_number(tmp, true);
    }

    /************************************************
     * Minor expression number
     */
    if (!shortname.empty()) {
        std::vector<std::string> tokens = split(shortname);
        int tmp = 0;
        for (auto t : tokens) {
            tmp += parse_single_word(t);
        }
        this->mMinorExpression = parse_single_number(tmp, true);
    } else {
        this->mMinorExpression = INVALID_NUMBER;
    }
    
    /************************************************
     * Major desire & personality numbers
     */
    this->mMajorDesire = 0;
    this->mMajorPersonality = 0;

    {
        int t0, t1;
        std::vector<std::string> tokens = split(fullname);
        for (auto token : tokens) {
            t0 = 0;
            t1 = 0;
            for (int i = 0; i < token.length(); i++) {
                if (is_valid_abc(token[i])) {
                    bool is_first_char = false;
                    if (0 < i) {
                        is_first_char = (' ' == token[i - 1]);
                    }

                    if (is_vowel(token[i], is_first_char)) {
                        t0 += char2number(token[i]);
                    } else {
                        t1 += char2number(token[i]);
                    }
                }
            }
            this->mMajorDesire += parse_single_number(t0, true);
            this->mMajorPersonality += parse_single_number(t1, true);
        }

        if (0 != this->mMajorDesire) {
            this->mMajorDesire = parse_single_number(this->mMajorDesire, true);
        } else {
            this->mMajorDesire = INVALID_NUMBER;
        }

        if (0 != this->mMajorPersonality) {
            this->mMajorPersonality = parse_single_number(this->mMajorPersonality, true);
        } else {
            this->mMajorPersonality = INVALID_NUMBER;
        }
    }

    /************************************************
     * Minor desire & personality numbers
     */
    if (shortname.empty()) {
        this->mMinorDesire = INVALID_NUMBER;
        this->mMinorPersonality = INVALID_NUMBER;
    } else {
        this->mMinorDesire = 0;
        this->mMinorPersonality = 0;
        int t0, t1;
        std::vector<std::string> tokens = split(shortname);
        for (auto token : tokens) {
            t0 = 0;
            t1 = 0;
            for (int i = 0; i < token.length(); i++) {
                if (is_valid_abc(token[i])) {
                    bool is_first_char = false;
                    if (0 < i) {
                        is_first_char = (' ' == token[i - 1]);
                    }

                    if (is_vowel(token[i], is_first_char)) {
                        t0 += char2number(token[i]);
                    } else {
                        t1 += char2number(token[i]);
                    }
                }
            }
            this->mMinorDesire += parse_single_number(t0, true);
            this->mMinorPersonality += parse_single_number(t1, true);
        }

        if (0 != this->mMinorDesire) {
            this->mMinorDesire = parse_single_number(this->mMinorDesire, true);
        } else {
            this->mMinorDesire = INVALID_NUMBER;
        }

        if (0 != this->mMinorPersonality) {
            this->mMinorPersonality = parse_single_number(this->mMinorPersonality, true);
        } else {
            this->mMinorPersonality = INVALID_NUMBER;
        }
    }

    /************************************************
     * Hidden passion, Habit challenge & Balance numbers
     */
    {
        std::array<int, 9> chrCnts;
        chrCnts.fill(0);

        this->mHiddenPassion = 0;
        this->mHabitChallenge = 0;
        this->mBalance = 0;

        for (int i = 0; i < fullname.size(); i++) {
            char c = fullname[i];
            if (is_valid_abc(c)) {
                chrCnts[char2number(c) - 1]++;

                if ((0 == i) || (' ' == fullname[i - 1])) {
                    this->mBalance += char2number(c);
                }
            }
        }

        int maxCnt = 0;

        for (int i = 0; i< chrCnts.size(); i++) {
            if (maxCnt < chrCnts[i]) {
                maxCnt = chrCnts[i];
                this->mHiddenPassion = i + 1;
            }

            if (0 != chrCnts[i]) {
                this->mHabitChallenge++;
            }
        }

        this->mBalance = parse_single_number(this->mBalance, false);
    }

    // Update composition numbers
    if (mDoBInitialized) {
        synchronize();
    }
}

numerology::Numbers numerology::Numbers::Analyze(const Person& personalInformation) {
    Numbers numbers;
    numbers.updateDoB(personalInformation.mDob);
    numbers.updateNames(personalInformation.mFullName, personalInformation.mShortName);

    return numbers;
}

std::string numerology::Numbers::toString() {
    std::ostringstream ss;

    if (this->mDoBInitialized) {
        ss << "Birth Date related numbers:" << std::endl;
        ss << " * Life Path: " << this->mLifePath << std::endl;
        ss << " * Birth Day: " << this->mBirthDay << std::endl;
        ss << " * Challenges: [";
        {
            for (int v : this->mChallenges) {
                ss << v << " ";
            }
            ss << "]" << std::endl;
        }

        ss << " * Period Cycles: [";
        {
            for (int v : this->mPeriodCycles) {
                ss << v << " ";
            }
            ss << "]" << std::endl;
        }

        ss << " * Pinnacle Cycles: [";
        {
            for (int v : this->mPinnacleCycles) {
                ss << v << " ";
            }
            ss << "]" << std::endl;
        }
    }

    if (this->mNameInitialized) {
        ss << "Name related numbers:" << std::endl;
        ss << " * Expression: " << this->mMajorExpression << "/" << this->mMinorExpression << std::endl;
        ss << " * Desire: " << this->mMajorDesire << "/" << this->mMinorDesire << std::endl;
        ss << " * Personality: " << this->mMajorPersonality << "/" << this->mMinorPersonality << std::endl;
        ss << " * Hidden Passion: " << this->mHiddenPassion << std::endl;
        ss << " * Habit Challenge: " << this->mHabitChallenge << std::endl;
        ss << " * Balance: " << this->mBalance << std::endl;
    }

    return ss.str();
}
