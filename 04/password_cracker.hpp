#ifndef ADVENT_OF_CODE_04_PASSWORD_CRACKING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_04_PASSWORD_CRACKING_HPP_INCLUDE_GUARD

#include <array>
#include <string_view>
#include <vector>

struct InputRange {
    int lower;
    int upper;
};

using Digits = std::array<int, 6>;

InputRange parseInput(std::string_view input);

Digits getDigits(int n);

bool hasAdjacentDigits(Digits const& digits);

bool isIncreasing(Digits const& digits);

bool hasDouble(Digits const& digits);

bool isValidPassword(int password);

std::vector<int> generateValidPasswords(InputRange r);

bool isValidPassword2(int password);

std::vector<int> generateValidPasswords2(InputRange r);

#endif
