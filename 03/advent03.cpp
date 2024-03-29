#include <wires_crossing.hpp>

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

    Field f = parseInput(*input);
    layoutWires(f);
    auto [coords, dist] = closestIntersection(f.wires[0], f.wires[1]);

    std::cout << "First result is " << dist << std::endl;

    auto const [steps_w1, steps_w2] = walkIntersectionPoints(f);
    std::cout << "Second result is " << steps_w1 + steps_w2 << std::endl;

    return 0;
}
