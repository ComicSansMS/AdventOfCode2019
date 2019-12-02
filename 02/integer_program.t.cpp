#include <integer_program.hpp>

#include <catch.hpp>

#include <vector>

TEST_CASE("Integer Program")
{
    char const program1[] = "1,9,10,3,2,3,11,0,99,30,40,50";
    SECTION("Parse Input")
    {
        CHECK(parseInput("1,2,3,4,5").memory == std::vector<int>{1, 2, 3, 4, 5});
        CHECK(parseInput("1,2,3,4,5").pc == 0);

        CHECK(parseInput(program1).memory == std::vector<int>{1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50});
        CHECK(parseInput(program1).pc == 0);
    }

    SECTION("Opcode Execution")
    {
        auto p = parseInput(program1);
        executeOpcode(p);
        CHECK(p.memory == std::vector<int>{1, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50});
        CHECK(p.pc == 4);

        executeOpcode(p);
        CHECK(p.memory == std::vector<int>{3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50});
        CHECK(p.pc == 8);

        executeOpcode(p);
        CHECK(p.memory == std::vector<int>{3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50});
        CHECK(p.pc == -1);
    }

    SECTION("Invalid opcode")
    {
        auto p = parseInput("3,0,0,0,99");
        executeOpcode(p);
        CHECK(p.memory == std::vector<int>{3, 0, 0, 0, 99});
        CHECK(p.pc == -2);
    }

    SECTION("Parameters out of range")
    {
        auto p = parseInput("1,5,0,0,99");
        executeOpcode(p);
        CHECK(p.memory == std::vector<int>{1, 5, 0, 0, 99});
        CHECK(p.pc == -3);
    }

    SECTION("Program execution")
    {
        auto p = parseInput(program1);
        executeProgram(p);
        CHECK(p.memory == std::vector<int>{3500, 9, 10, 70, 2, 3, 11, 0, 99, 30, 40, 50});
        CHECK(p.pc == -1);

        p = parseInput("1,0,0,0,99");
        executeProgram(p);
        CHECK(p.memory == std::vector<int>{2, 0, 0, 0, 99});
        CHECK(p.pc == -1);

        p = parseInput("2,3,0,3,99");
        executeProgram(p);
        CHECK(p.memory == std::vector<int>{2, 3, 0, 6, 99});
        CHECK(p.pc == -1);

        p = parseInput("2,4,4,5,99,0");
        executeProgram(p);
        CHECK(p.memory == std::vector<int>{2, 4, 4, 5, 99, 9801});
        CHECK(p.pc == -1);

        p = parseInput("1,1,1,4,99,5,6,0,99");
        executeProgram(p);
        CHECK(p.memory == std::vector<int>{30, 1, 1, 4, 2, 5, 6, 0, 99});
        CHECK(p.pc == -1);
    }
}
