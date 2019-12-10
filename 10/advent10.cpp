#include <asteroid_scanner.hpp>

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

    auto map = parseInput(*input);
    determineLinesOfSight(map);

    auto const [visible, index] = bestVantagePoint(map);

    std::cout << "First result is " << visible << std::endl;

    auto const targets = determineTargets(map, index);
    auto const vaporizing_order = vaporize(targets);
    Vector2 target_200th = vaporizing_order[199];

    std::cout << "Second result is " << target_200th.x << target_200th.y << std::endl;

    return 0;
}
