#include <password_cracker.hpp>

#include <range/v3/algorithm/adjacent_find.hpp>
#include <range/v3/range/access.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/filter.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <string>

InputRange parseInput(std::string_view input)
{
    auto separator = input.find('-');
    InputRange ret;
    ret.lower = std::stoi(std::string(input.substr(0, separator)));
    ret.upper = std::stoi(std::string(input.substr(separator + 1)));
    return ret;
}

Digits getDigits(int n)
{
    Digits ret;
    int divider = 1;
    for (int i = 0; i < 6; ++i) {
        ret[5 - i] = (n / divider) % 10;
        divider *= 10;
    }
    assert((n / divider) == 0);
    return ret;
}

bool hasAdjacentDigits(Digits const& digits)
{
    return ranges::adjacent_find(digits) != ranges::end(digits);
}

bool isIncreasing(Digits const& digits)
{
    return ranges::adjacent_find(digits, std::greater{}) == ranges::end(digits);
}

bool hasDouble(Digits const& digits)
{
    auto it = begin(digits);
    while (it != end(digits)) {
        it = std::adjacent_find(it, end(digits));
        if (it != end(digits)) {
            auto const successor = std::next(it, 2);
            if (successor != end(digits) && *successor == *it) {
                // group of 3 or more; skip them all
                it = successor;
                while ((it != end(digits)) && (*it == *successor)) { ++it; }
            } else {
                // found a group of exactly 2
                return true;
            }
        }
    }
    return false;
}

bool isValidPassword(int password)
{
    Digits const digits = getDigits(password);
    return isIncreasing(digits) && hasAdjacentDigits(digits);
}

std::vector<int> generateValidPasswords(InputRange r)
{
    return ranges::to<std::vector<int>>(
        ranges::views::iota(r.lower, r.upper + 1) |
        ranges::views::filter(isValidPassword));
}

bool isValidPassword2(int password)
{
    Digits const digits = getDigits(password);
    return isIncreasing(digits) && hasDouble(digits);
}

std::vector<int> generateValidPasswords2(InputRange r)
{
    return ranges::to<std::vector<int>>(
        ranges::views::iota(r.lower, r.upper + 1) |
        ranges::views::filter(isValidPassword2));
}

