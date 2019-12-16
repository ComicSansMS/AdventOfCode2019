#ifndef ADVENT_OF_CODE_16_FREQUENCY_TRANSMISSION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_16_FREQUENCY_TRANSMISSION_HPP_INCLUDE_GUARD

#include <array>
#include <functional>
#include <string_view>
#include <vector>

using Signal = std::vector<int>;

Signal parseInput(std::string_view input);

using Pattern = std::vector<int>;

Pattern generatePattern(int n_iteration);

Signal multiplyPattern(Signal const& s, Pattern const& p);

Signal applyPhase(Signal const& s);

Signal calculateTransmission(Signal const& s, int n);

Signal calculateTransmission_10k(Signal const& s, int n);

#endif
