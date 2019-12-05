#ifndef ADVENT_OF_CODE_02_INTEGER_PROGRAM_MK2_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_02_INTEGER_PROGRAM_MK2_HPP_INCLUDE_GUARD

#include <string_view>
#include <tuple>
#include <vector>

struct IntegerProgram {
    std::vector<int> memory;
    int pc;
    std::vector<int> input;
    std::vector<int> output;
};

enum class Opcode {
    Add = 1,
    Multiply = 2,
    Input = 3,
    Output = 4,
    JumpIfTrue = 5,
    JumpIfFalse = 6,
    LessThan = 7,
    Equals = 8,
    Halt = 99
};

enum class Mode {
    Position = 0,
    Immediate = 1,
    NoArgument = -1
};

IntegerProgram parseInput(std::string_view input);

std::tuple<Opcode, Mode, Mode, Mode> decode(int instruction);

void executeOpcode(IntegerProgram& p);

void executeProgram(IntegerProgram& p);

int executeWithParameters(IntegerProgram const& p, int noun, int verb);

struct SearchResult {
    int noun;
    int verb;
};

SearchResult searchForOutput(IntegerProgram const& p, int desired_result);

#endif
