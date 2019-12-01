#ifndef ADVENT_OF_CODE_01_ROCKET_EQUATION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_01_ROCKET_EQUATION_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

std::vector<int> parseInput(std::string_view input);

int rocket_equation(int mass);

int total_fuel(std::vector<int> const& masses);

int rocket_equation_with_fuel(int mass);

int total_fuel_with_fuel(std::vector<int> const& masses);

#endif
