#include <vacuum_robot.hpp>

#include <cassert>
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

void printOutput(Intcode& p, std::ostream& os = std::cout)
{
    for (auto const& c : p.output()) {
        os << static_cast<char>(c);
    }
    p.output().clear();
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

    bool const do_output = false;

    auto iprog = parseInput(*input);
    Intcode p(iprog);
    p.execute();
    Map m = parseMap(p.output());
    markIntersections(m);
    if constexpr (do_output) { std::cout << m; }

    std::cout << "First result is " << alignment1(m) << std::endl;

    /* Character Limit:  |------------------| */
    std::string main  = "A,C,A,C,B,C,A,B,C,B\n";
    std::string moveA = "L,12,L,8,R,10,R,10\n";
    std::string moveB = "R,10,L,8,L,4,R,10\n";
    std::string moveC = "L,6,L,4,L,12\n";
    /* Character Limit:  |------------------| */
    std::string wantVideo = "n\n";
    assert(main.size() <= 20);
    assert(moveA.size() <= 20);
    assert(moveB.size() <= 20);
    assert(moveC.size() <= 20);
    iprog.memory[0] = 2;

    Intcode p2(iprog);
    p2.execute();
    if constexpr (do_output) { printOutput(p2); }
    if (p2.pc() != ResultCode::MissingInput) { return 1; }

    p2.input().insert(p2.input().end(), main.begin(), main.end());
    p2.resumeExecution();
    if constexpr (do_output) { printOutput(p2); }
    if (p2.pc() != ResultCode::MissingInput) { return 1; }

    p2.input().insert(p2.input().end(), moveA.begin(), moveA.end());
    p2.resumeExecution();
    if constexpr (do_output) { printOutput(p2); }
    if (p2.pc() != ResultCode::MissingInput) { return 1; }

    p2.input().insert(p2.input().end(), moveB.begin(), moveB.end());
    p2.resumeExecution();
    if constexpr (do_output) { printOutput(p2); }
    if (p2.pc() != ResultCode::MissingInput) { return 1; }

    p2.input().insert(p2.input().end(), moveC.begin(), moveC.end());
    p2.resumeExecution();
    if constexpr (do_output) { printOutput(p2); }
    if (p2.pc() != ResultCode::MissingInput) { return 1; }

    p2.input().insert(p2.input().end(), wantVideo.begin(), wantVideo.end());
    p2.resumeExecution();

    assert(p2.pc() == ResultCode::Halted);
    Word const result = p2.output().back();
    p2.output().pop_back();
    if constexpr (do_output) {
        std::ofstream fout("out.txt");
        printOutput(p2, fout);
    }

    std::cout << "Second result is " << result << std::endl;

    return 0;
}
