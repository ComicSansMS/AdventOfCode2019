#include <path_finder.hpp>

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

    IntegerProgram p = parseInput(*input);
    executeProgram(p);
    Scanner s(p);

    MazeMap const map = floodFill(s);
    auto const it_target = find_target(map);

    std::cout << "First result is " << it_target->second.path.size() << std::endl;
    std::cout << "Second result is " << floodFill2(map) << std::endl;

    return 0;
}
