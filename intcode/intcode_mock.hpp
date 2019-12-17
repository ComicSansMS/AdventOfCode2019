#ifndef ADVENT_OF_CODE_INTCODE_MOCK_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_INTCODE_MOCK_HPP_INCLUDE_GUARD

#include <intcode.hpp>

struct IntcodeProgramMock_Base {
    Address pc;
    Address base;
    std::vector<Word> input;
    std::vector<Word> output;
    Address resume_point;
};

#endif
