#include <integer_program.hpp>

#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/drop_while.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/operations.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

std::optional<std::string> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        std::cerr << "Unable to open input file '" << filename << "' for reading." << std::endl;
        return std::nullopt;
    }

    std::stringstream sstr;
    sstr << fin.rdbuf();
    if(!fin) {
        std::cerr << "Unable to read input from file '" << filename << "'." << std::endl;
        return std::nullopt;
    }
    return sstr.str();
}

int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    auto const input = readInput(input_filename);

    if(!input) {
        return 1;
    }

    auto const p = parseInput(*input);

    auto p1 = p;
    p1.memory[1] = 12;
    p1.memory[2] = 2;
    executeProgram(p1);

    std::cout << "First result is " << p1.memory[0] << std::endl;

    auto ri = ranges::views::iota(0, 100);
    auto [_, noun, verb] = ranges::front(
        ranges::views::cartesian_product(ri, ri) |
        ranges::views::transform([&p](auto tp) {
            auto const [noun, verb] = tp;
            auto p2 = p;
            p2.memory[1] = noun;
            p2.memory[2] = verb;
            executeProgram(p2);
            return std::make_tuple(p2.memory[0], noun, verb);
        }) |
        ranges::views::drop_while([](auto t) { return std::get<0>(t) != 19690720; }));

    std::cout << "Second result is " << (100 * noun + verb) << std::endl;

    return 0;
}
