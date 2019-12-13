#include <intcode_arcade.hpp>

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

    IntegerProgram const p = parseInput(*input);

    IntegerProgram p1 = p;
    executeProgram(p1);
    std::cout << "First result is " << countTiles(p1.output, Tile::Block) << std::endl;

    IntegerProgram p2 = p;
    //std::ofstream fout("game_out.txt");
    std::cout << "Second result is " << playTheGame(p2, nullptr) << std::endl;

    return 0;
}
