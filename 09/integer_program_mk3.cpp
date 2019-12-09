#include <integer_program_mk3.hpp>

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

IntegerProgram parseInput(std::string_view input)
{
    std::unordered_map<Address, Word> memory = ranges::to<std::unordered_map<Address, Word>>(
        ranges::views::zip(
            ranges::views::iota(0),
            input | ranges::views::split(',') |
            ranges::views::transform([](auto sv) {
                return std::stoi(ranges::to<std::string>(sv));
                })));
    return IntegerProgram{ std::move(memory), 0, {}, {}, 0 };
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
        case Opcode::Halt: return 0;
        default: return -1;
        }
    }();
    auto const mode_1 = (arity >= 1) ? static_cast<Mode>((instruction / 100) % 10) : Mode::NoArgument;
    auto const mode_2 = (arity >= 2) ? static_cast<Mode>((instruction / 1000) % 10) : Mode::NoArgument;
    auto const mode_3 = (arity >= 3) ? static_cast<Mode>((instruction / 10000) % 10) : Mode::NoArgument;
    return std::make_tuple(opcode, mode_1, mode_2, mode_3);
}

void executeOpcode(IntegerProgram& p)
{
    auto const range_check = [&p](Word i) { return (i >= 0) && (i < std::numeric_limits<Address>::max()); };
    auto checkParameters1 = [&p, range_check](Mode mode_1) -> bool {
        return range_check(p.pc + 1) &&
            (mode_1 == Mode::Immediate || range_check(p.memory[p.pc + 1]));
    };
    auto checkParameters2 = [&p, range_check](Mode mode_1, Mode mode_2) -> bool {
        return range_check(p.pc + 1) && range_check(p.pc + 2) &&
            (mode_1 == Mode::Immediate || range_check(p.memory[p.pc + 1])) &&
            (mode_2 == Mode::Immediate || range_check(p.memory[p.pc + 2]));
    };
    auto checkParameters3 = [&p, range_check](Mode mode_1, Mode mode_2, Mode mode_3) -> bool {
        return range_check(p.pc + 1) && range_check(p.pc + 2) && range_check(p.pc + 3) &&
            (mode_1 == Mode::Immediate || range_check(p.memory[p.pc + 1])) &&
            (mode_2 == Mode::Immediate || range_check(p.memory[p.pc + 2])) &&
            (mode_3 == Mode::Immediate || range_check(p.memory[p.pc + 3]));
    };
    auto fetch_arg = [&p](Address position, Mode mode) -> Word& {
        return (mode == Mode::Position) ? (p.memory[p.memory[p.pc + position]]) : (p.memory[p.pc + position]);
    };
    auto const [opcode, mode_1, mode_2, mode_3] = decode(p.memory[p.pc]);
    switch (opcode) {
    case Opcode::Add:
        if (!checkParameters3(mode_1, mode_2, mode_3)) { p.pc = ResultCode::InvalidAddress; break; }
        fetch_arg(3, mode_3) = fetch_arg(1, mode_1) + fetch_arg(2, mode_2);
        p.pc += 4;
        break;
    case Opcode::Multiply:
        if (!checkParameters3(mode_1, mode_2, mode_3)) { p.pc = ResultCode::InvalidAddress; break; }
        fetch_arg(3, mode_3) = fetch_arg(1, mode_1) * fetch_arg(2, mode_2);
        p.pc += 4;
        break;
    case Opcode::Input:
        if (p.input.empty()) { p.resume_point = p.pc; p.pc = ResultCode::MissingInput; break; }
        if (!checkParameters1(mode_1)) { p.pc = ResultCode::InvalidAddress; break; }
        fetch_arg(1, mode_1) = p.input.front();
        p.input.erase(p.input.begin());
        p.pc += 2;
        break;
    case Opcode::Output:
        if (!checkParameters1(mode_1)) { p.pc = ResultCode::InvalidAddress; break; }
        p.output.push_back(fetch_arg(1, mode_1));
        p.pc += 2;
        break;
    case Opcode::JumpIfTrue:
        if (!checkParameters2(mode_1, mode_2)) { p.pc = ResultCode::InvalidAddress; break; }
        if (fetch_arg(1, mode_1) != 0) {
            p.pc = fetch_arg(2, mode_2);
        } else {
            p.pc += 3;
        }
        break;
    case Opcode::JumpIfFalse:
        if (!checkParameters2(mode_1, mode_2)) { p.pc = ResultCode::InvalidAddress; break; }
        if (fetch_arg(1, mode_1) == 0) {
            p.pc = fetch_arg(2, mode_2);
        } else {
            p.pc += 3;
        }
        break;
    case Opcode::LessThan:
        if (!checkParameters3(mode_1, mode_2, mode_3)) { p.pc = ResultCode::InvalidAddress; break; }
        fetch_arg(3, mode_3) = (fetch_arg(1, mode_1) < fetch_arg(2, mode_2)) ? 1 : 0;
        p.pc += 4;
        break;
    case Opcode::Equals:
        if (!checkParameters3(mode_1, mode_2, mode_3)) { p.pc = ResultCode::InvalidAddress; break; }
        fetch_arg(3, mode_3) = (fetch_arg(1, mode_1) == fetch_arg(2, mode_2)) ? 1 : 0;
        p.pc += 4;
        break;
    case Opcode::Halt:
        p.pc = ResultCode::Halted;
        break;
    default:
        p.pc = ResultCode::InvalidOpcode;
        break;
    }
}

void executeProgram(IntegerProgram& p)
{
    while (p.pc >= 0) {
        executeOpcode(p);
    }
}

Word executeWithParameters(IntegerProgram const& p, Word noun, Word verb)
{
    auto p1 = p;
    p1.memory[1] = noun;
    p1.memory[2] = verb;
    executeProgram(p1);
    return p1.memory[0];
}

SearchResult searchForOutput(IntegerProgram const& p, Word desired_result)
{
    auto ri = ranges::views::iota(0, 100);
    auto [_, noun, verb] = ranges::front(
        ranges::views::cartesian_product(ri, ri) |
        ranges::views::transform([&p](auto tp) {
            auto const [noun, verb] = tp;
            return std::make_tuple(executeWithParameters(p, noun, verb), noun, verb);
            }) |
        ranges::views::drop_while([desired_result](auto t) { return std::get<0>(t) != desired_result; }));
    return SearchResult{ noun, verb };
}
