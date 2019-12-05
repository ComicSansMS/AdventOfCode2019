#include <integer_program_mk2.hpp>

#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/operations.hpp>

#include <cassert>
#include <string>

IntegerProgram parseInput(std::string_view input)
{
    return IntegerProgram{ ranges::to<std::vector<int>>(input | ranges::views::split(',') |
        ranges::views::transform([](auto sv) {
            return std::stoi(ranges::to<std::string>(sv));
        })), 0 };
}

std::tuple<Opcode, Mode, Mode, Mode> decode(int instruction)
{
    auto const opcode = static_cast<Opcode>(instruction % 100);
    int const arity = [opcode]() {
        switch (opcode)
        {
        case Opcode::Add: return 3;
        case Opcode::Multiply: return 3;
        case Opcode::Input: return 1;
        case Opcode::Output: return 1;
        case Opcode::Halt: return 0;
        default: return -1;
        }
    }();
    auto const mode_1 = (arity >= 1) ? static_cast<Mode>((instruction / 100) % 10) : Mode::NoArgument;
    auto const mode_2 = (arity >= 3) ? static_cast<Mode>((instruction / 1000) % 10) : Mode::NoArgument;
    auto const mode_3 = (arity >= 3) ? static_cast<Mode>((instruction / 10000) % 10) : Mode::NoArgument;
    return std::make_tuple(opcode, mode_1, mode_2, mode_3);
}

void executeOpcode(IntegerProgram& p)
{
    auto const range_check = [&p](int i) { return (i >= 0) && (i < static_cast<int>(p.memory.size())); };
    auto checkParameters1 = [&p, range_check](Mode mode_1, bool do_resize = false) -> bool {
        if (!range_check(p.pc + 1)) {
            return false;
        } else if (mode_1 == Mode::Position && !range_check(p.memory[p.pc + 1])) {
            if (!do_resize) { return false; }
            p.memory.resize(p.memory[p.pc + 1] + 1);
        }
        return true;
    };
    auto checkParameters3 = [&p, range_check](Mode mode_1, Mode mode_2, Mode mode_3) -> bool {
        return range_check(p.pc + 1) && range_check(p.pc + 2) && range_check(p.pc + 3) &&
            (mode_1 != Mode::Position || range_check(p.memory[p.pc + 1])) &&
            (mode_2 != Mode::Position || range_check(p.memory[p.pc + 2])) &&
            (mode_3 != Mode::Position || range_check(p.memory[p.pc + 3]));
    };
    auto fetch_arg = [&p](int position, Mode mode) -> int& {
        return (mode == Mode::Position) ? (p.memory[p.memory[p.pc + position]]) : (p.memory[p.pc + position]);
    };
    auto const [opcode, mode_1, mode_2, mode_3] = decode(p.memory[p.pc]);
    switch (opcode) {
    case Opcode::Add:
        if (!checkParameters3(mode_1, mode_2, mode_3)) { p.pc = -3; break; }
        fetch_arg(3, mode_3) = fetch_arg(1, mode_1) + fetch_arg(2, mode_2);
        p.pc += 4;
        break;
    case Opcode::Multiply:
        if (!checkParameters3(mode_1, mode_2, mode_3)) { p.pc = -3; break; }
        fetch_arg(3, mode_3) = fetch_arg(1, mode_1) * fetch_arg(2, mode_2);
        p.pc += 4;
        break;
    case Opcode::Input:
        if (p.input.empty()) { p.pc = -4; break; }
        if (!checkParameters1(mode_1, true)) { p.pc = -3; break; }
        fetch_arg(1, mode_1) = p.input.front();
        p.input.erase(p.input.begin());
        p.pc += 2;
        break;
    case Opcode::Output:
        if (!checkParameters1(mode_1)) { p.pc = -3; break; }
        p.output.push_back(fetch_arg(1, mode_1));
        p.pc += 2;
        break;
    case Opcode::Halt:
        p.pc = -1;
        break;
    default:
        p.pc = -2;
        break;
    }
}

void executeProgram(IntegerProgram& p)
{
    while (p.pc >= 0) {
        executeOpcode(p);
    }
}

int executeWithParameters(IntegerProgram const& p, int noun, int verb)
{
    auto p1 = p;
    p1.memory[1] = noun;
    p1.memory[2] = verb;
    executeProgram(p1);
    return p1.memory[0];
}

SearchResult searchForOutput(IntegerProgram const& p, int desired_result)
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
