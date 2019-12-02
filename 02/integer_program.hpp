#ifndef ADVENT_OF_CODE_02_INTEGER_PROGRAM_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_02_INTEGER_PROGRAM_HPP_INCLUDE_GUARD

#include <string_view>
#include <vector>

struct IntegerProgram {
    std::vector<int> memory;
    int pc;
};

enum class Opcode {
    Add = 1,
    Multiply = 2,
    Halt = 99
};

IntegerProgram parseInput(std::string_view input);

void executeOpcode(IntegerProgram& p);

void executeProgram(IntegerProgram& p);

int executeWithParameters(IntegerProgram const& p, int noun, int verb);

struct SearchResult {
    int noun;
    int verb;
};

SearchResult searchForOutput(IntegerProgram const& p, int desired_result);

#endif
