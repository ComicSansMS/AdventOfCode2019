#include <intcode.hpp>

#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/operations.hpp>

#include <cassert>
#include <limits>
#include <string>

IntcodeProgram parseInput(std::string_view input)
{
    std::unordered_map<Address, Word> memory = ranges::to<std::unordered_map<Address, Word>>(
        ranges::views::zip(
            ranges::views::iota(0),
            input | ranges::views::split(',') |
            ranges::views::transform([](auto sv) -> Word {
                    return std::stoll(ranges::to<std::string>(sv));
                })));
    return IntcodeProgram{ std::move(memory), 0, 0, {}, {}, 0 };
}

std::tuple<Opcode, Mode, Mode, Mode> decode(Word instruction)
{
    auto const opcode = static_cast<Opcode>(instruction % 100);
    int const arity = [opcode]() {
        switch (opcode)
        {
        case Opcode::Add: return 3;
        case Opcode::Multiply: return 3;
        case Opcode::Input: return 1;
        case Opcode::Output: return 1;
        case Opcode::JumpIfTrue: return 2;
        case Opcode::JumpIfFalse: return 2;
        case Opcode::LessThan: return 3;
        case Opcode::Equals: return 3;
        case Opcode::AdjustRelativeBase: return 1;
        case Opcode::Halt: return 0;
        default: return -1;
        }
    }();
    auto const mode_1 = (arity >= 1) ? static_cast<Mode>((instruction / 100) % 10) : Mode::NoArgument;
    auto const mode_2 = (arity >= 2) ? static_cast<Mode>((instruction / 1000) % 10) : Mode::NoArgument;
    auto const mode_3 = (arity >= 3) ? static_cast<Mode>((instruction / 10000) % 10) : Mode::NoArgument;
    assert((mode_1 == Mode::Position) || (mode_1 == Mode::Immediate) || (mode_1 == Mode::Relative) || (mode_1 == Mode::NoArgument));
    assert((mode_2 == Mode::Position) || (mode_2 == Mode::Immediate) || (mode_2 == Mode::Relative) || (mode_2 == Mode::NoArgument));
    assert((mode_3 == Mode::Position) || (mode_3 == Mode::Immediate) || (mode_3 == Mode::Relative) || (mode_3 == Mode::NoArgument));
    return std::make_tuple(opcode, mode_1, mode_2, mode_3);
}

void executeOpcode(IntcodeProgram& p)
{
    auto fetch_arg = [&p](Address position, Mode mode) -> Word& {
        if (mode == Mode::Position) {
            assert(p.memory[p.pc + position] >= 0);
            return (p.memory[p.memory[p.pc + position]]);
        } else if (mode == Mode::Immediate) {
            return (p.memory[p.pc + position]);
        } else {
            assert(mode == Mode::Relative);
            assert(p.memory[p.pc + position] + p.base >= 0);
            return (p.memory[p.memory[p.pc + position] + p.base]);
        }
    };
    auto const [opcode, mode_1, mode_2, mode_3] = decode(p.memory[p.pc]);
    switch (opcode) {
    case Opcode::Add:
        fetch_arg(3, mode_3) = fetch_arg(1, mode_1) + fetch_arg(2, mode_2);
        p.pc += 4;
        break;
    case Opcode::Multiply:
        fetch_arg(3, mode_3) = fetch_arg(1, mode_1) * fetch_arg(2, mode_2);
        p.pc += 4;
        break;
    case Opcode::Input:
        if (p.input.empty()) { p.resume_point = p.pc; p.pc = ResultCode::MissingInput; break; }
        fetch_arg(1, mode_1) = p.input.front();
        p.input.erase(p.input.begin());
        p.pc += 2;
        break;
    case Opcode::Output:
        p.output.push_back(fetch_arg(1, mode_1));
        p.pc += 2;
        break;
    case Opcode::JumpIfTrue:
        if (fetch_arg(1, mode_1) != 0) {
            p.pc = fetch_arg(2, mode_2);
        } else {
            p.pc += 3;
        }
        break;
    case Opcode::JumpIfFalse:
        if (fetch_arg(1, mode_1) == 0) {
            p.pc = fetch_arg(2, mode_2);
        } else {
            p.pc += 3;
        }
        break;
    case Opcode::LessThan:
        fetch_arg(3, mode_3) = (fetch_arg(1, mode_1) < fetch_arg(2, mode_2)) ? 1 : 0;
        p.pc += 4;
        break;
    case Opcode::Equals:
        fetch_arg(3, mode_3) = (fetch_arg(1, mode_1) == fetch_arg(2, mode_2)) ? 1 : 0;
        p.pc += 4;
        break;
    case Opcode::AdjustRelativeBase:
        p.base += fetch_arg(1, mode_1);
        p.pc += 2;
        break;
    case Opcode::Halt:
        p.pc = ResultCode::Halted;
        break;
    default:
        p.pc = ResultCode::InvalidOpcode;
        break;
    }
}

void executeProgram(IntcodeProgram& p)
{
    while (p.pc >= 0) {
        executeOpcode(p);
    }
}
