#ifndef ADVENT_OF_CODE_07_AMP_SETTINGS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_07_AMP_SETTINGS_HPP_INCLUDE_GUARD

#include <integer_program_mk2.hpp>

#include <array>

using AmpChain = std::array<IntegerProgram, 5>;

int determineMaxSignal(IntegerProgram const& p);

int determineMaxSignalWithFeedback(IntegerProgram const& p);

#endif
