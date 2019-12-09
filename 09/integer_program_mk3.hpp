#ifndef ADVENT_OF_CODE_09_INTEGER_PROGRAM_MK3_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_09_INTEGER_PROGRAM_MK3_HPP_INCLUDE_GUARD

#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

using Word = int64_t;
using Address = int64_t;

struct IntegerProgram {
    std::unordered_map<Address, Word> memory;
    Address pc;
    std::vector<Word> input;
    std::vector<Word> output;
    Address resume_point;
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

struct ResultCode {
    enum ResultCodes {
        Halted = -1,
        InvalidOpcode = -2,
        InvalidAddress = -3,
        MissingInput = -4
    };
};

IntegerProgram parseInput(std::string_view input);

std::tuple<Opcode, Mode, Mode, Mode> decode(Word instruction);

void executeOpcode(IntegerProgram& p);

void executeProgram(IntegerProgram& p);

Word executeWithParameters(IntegerProgram const& p, Word noun, Word verb);

struct SearchResult {
    Word noun;
    Word verb;
};

SearchResult searchForOutput(IntegerProgram const& p, Word desired_result);

#endif
