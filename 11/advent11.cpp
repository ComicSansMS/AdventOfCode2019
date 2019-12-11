#include <intcode_robot.hpp>

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

    IntcodeRobot r(*input);
    r.run();

    std::cout << "First result is " << r.robot.grid.size() << std::endl;

    IntcodeRobot r2(*input);
    r2.robot.grid[Vector2(0, 0)] = Color::White;
    r2.run();

    std::cout << "Second result is\n" << r2.robot << std::endl;

    return 0;
}
