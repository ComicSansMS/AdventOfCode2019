#include <integer_program_mk2.hpp>

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
    p1.input.push_back(1);
    executeProgram(p1);

    std::cout << "First result is " << p1.output.back() << std::endl;
    //
    //auto const [noun, verb] = searchForOutput(p, 19690720);
    //
    //std::cout << "Second result is " << (100 * noun + verb) << std::endl;

    return 0;
}
