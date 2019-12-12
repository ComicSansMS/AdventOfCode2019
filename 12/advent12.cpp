#include <moon_simulator.hpp>

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

    PlanetarySystem const p = parseInput(*input);

    PlanetarySystem p1 = p;
    for (int i = 0; i<1000; ++i) { p1 = simulate(p1); }

    std::cout << "First result is " << totalEnergy(p1) << std::endl;

    std::cout << "Second result is " << findRepeatingState_clever(p) << std::endl;

    return 0;
}
