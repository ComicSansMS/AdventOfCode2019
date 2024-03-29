#include <integer_program_mk2.hpp>

#include <catch.hpp>

#include <vector>

TEST_CASE("Integer Program")
{
    char const program1[] = "1,9,10,3,2,3,11,0,99,30,40,50";
    SECTION("Parse Input")
    {
        auto const p0 = parseInput("1,2,3,4,5");
        CHECK(p0.memory == std::vector<int>{1, 2, 3, 4, 5});
        CHECK(p0.pc == 0);
        CHECK(p0.input.empty());
        CHECK(p0.output.empty());

        auto const p1 = parseInput(program1);
        CHECK(p1.memory == std::vector<int>{1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50});
        CHECK(p1.pc == 0);
        CHECK(p1.input.empty());
        CHECK(p1.output.empty());
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
        auto p = parseInput("42,0,0,0,99");
        executeOpcode(p);
        CHECK(p.memory == std::vector<int>{42, 0, 0, 0, 99});
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

    SECTION("Execute with parameters")
    {
        auto p = parseInput("1,0,0,0,99");
        CHECK(executeWithParameters(p, 4, 1) == 103);
        CHECK(executeWithParameters(p, 3, 1) == 3);
        CHECK(executeWithParameters(p, 0, 1) == 1);
        CHECK(executeWithParameters(p, 1, 1) == 2);
        CHECK(executeWithParameters(p, 1, 2) == 3);
    }

    SECTION("Search for output")
    {
        auto const [noun, verb] = searchForOutput(parseInput("1,1,1,4,99,5,6,0,99"), 1);
        CHECK(noun == 0);
        CHECK(verb == 2);
    }

    SECTION("Input")
    {
        auto p = parseInput("3,3,99");
        CHECK(p.memory.size() == 3);
        p.input.push_back(42);
        executeProgram(p);
        CHECK(p.pc == -1);
        CHECK(p.input.empty());
        REQUIRE(p.memory.size() == 4);
        CHECK(p.memory == std::vector<int>{ 3, 3, 99, 42 });

        auto p_no_input = parseInput("3,0,99");
        REQUIRE(p_no_input.input.empty());
        executeProgram(p_no_input);
        CHECK(p_no_input.pc == -4);

        auto p_out_of_range = parseInput("3");
        p_out_of_range.input.push_back(2);
        executeProgram(p_out_of_range);
        CHECK(p_out_of_range.pc == -3);
    }

    SECTION("Output")
    {
        auto p = parseInput("4,3,99,42");
        CHECK(p.output.empty());
        executeProgram(p);
        CHECK(p.pc == -1);
        CHECK(p.output == std::vector<int>{ 42 });

        auto p_out_of_range1 = parseInput("4,3,99");
        executeProgram(p_out_of_range1);
        CHECK(p_out_of_range1.pc == -3);

        auto p_out_of_range2 = parseInput("4");
        executeProgram(p_out_of_range2);
        CHECK(p_out_of_range2.pc == -3);
    }

    SECTION("In Out")
    {
        auto p = parseInput("3,0,4,0,99");
        p.input.push_back(42);
        executeProgram(p);
        CHECK(p.pc == -1);
        CHECK(p.input.empty());
        CHECK(p.output == std::vector<int>{ 42 });
    }

    SECTION("Decode Opcode")
    {
        CHECK(decode(1) == std::make_tuple(Opcode::Add, Mode::Position, Mode::Position, Mode::Position));
        CHECK(decode(2) == std::make_tuple(Opcode::Multiply, Mode::Position, Mode::Position, Mode::Position));
        CHECK(decode(3) == std::make_tuple(Opcode::Input, Mode::Position, Mode::NoArgument, Mode::NoArgument));
        CHECK(decode(4) == std::make_tuple(Opcode::Output, Mode::Position, Mode::NoArgument, Mode::NoArgument));
        CHECK(decode(99) == std::make_tuple(Opcode::Halt, Mode::NoArgument, Mode::NoArgument, Mode::NoArgument));

        CHECK(decode(101) == std::make_tuple(Opcode::Add, Mode::Immediate, Mode::Position, Mode::Position));
        CHECK(decode(1101) == std::make_tuple(Opcode::Add, Mode::Immediate, Mode::Immediate, Mode::Position));
        CHECK(decode(1001) == std::make_tuple(Opcode::Add, Mode::Position, Mode::Immediate, Mode::Position));
        CHECK(decode(11001) == std::make_tuple(Opcode::Add, Mode::Position, Mode::Immediate, Mode::Immediate));
        CHECK(decode(10001) == std::make_tuple(Opcode::Add, Mode::Position, Mode::Position, Mode::Immediate));
        CHECK(decode(10101) == std::make_tuple(Opcode::Add, Mode::Immediate, Mode::Position, Mode::Immediate));
        CHECK(decode(11101) == std::make_tuple(Opcode::Add, Mode::Immediate, Mode::Immediate, Mode::Immediate));
    }

    SECTION("Immediate Arguments")
    {
        auto p1 = parseInput("1101,5,23,0,99,42");
        executeProgram(p1);
        CHECK(p1.memory[0] == 28);

        auto p2 = parseInput("1001,5,23,0,99,42");
        executeProgram(p2);
        CHECK(p2.memory[0] == 65);
    }

    SECTION("Opcode Jump If True")
    {
        {
            auto p = parseInput("5,9,7,104,42,99,0,10,0,1,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 23 });
        }
        {
            auto p = parseInput("5,9,7,104,42,99,0,10,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 42 });
        }
        {
            auto p = parseInput("1005,9,10,104,42,99,0,0,0,1,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 23 });
        }
        {
            auto p = parseInput("1005,9,10,104,42,99,0,0,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 42 });
        }
        {
            auto p = parseInput("1105,1,10,104,42,99,0,0,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 23 });
        }
        {
            auto p = parseInput("1105,0,10,104,42,99,0,0,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 42 });
        }
    }

    SECTION("Opcode Jump If False")
    {
        {
            auto p = parseInput("6,9,7,104,42,99,0,10,0,1,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 42 });
        }
        {
            auto p = parseInput("6,9,7,104,42,99,0,10,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 23 });
        }
        {
            auto p = parseInput("1006,9,10,104,42,99,0,0,0,1,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 42 });
        }
        {
            auto p = parseInput("1006,9,10,104,42,99,0,0,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 23 });
        }
        {
            auto p = parseInput("1106,1,10,104,42,99,0,0,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 42 });
        }
        {
            auto p = parseInput("1106,0,10,104,42,99,0,0,0,0,104,23,99");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.output == std::vector<int>{ 23 });
        }
    }

    SECTION("Opcode Less Than")
    {
        {
            auto p = parseInput("7,5,6,0,99,100,1");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 0);
        }
        {
            auto p = parseInput("7,5,6,0,99,100,101");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 1);
        }
        {
            auto p = parseInput("1107,5,6,0,99,100,1");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 1);
        }
        {
            auto p = parseInput("1007,5,6,0,99,100,1");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 0);
        }
    }

    SECTION("Opcode Equals")
    {
        {
            auto p = parseInput("8,5,6,0,99,2,1");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 0);
        }
        {
            auto p = parseInput("8,5,6,0,99,2,2");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 1);
        }
        {
            auto p = parseInput("1108,5,6,0,99,5,5");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 0);
        }
        {
            auto p = parseInput("108,5,6,0,99,15,5");
            executeProgram(p);
            CHECK(p.pc == -1);
            CHECK(p.memory[0] == 1);
        }
    }

    SECTION("Sample Programs Equal")
    {
        std::string const programs_equal[] = {
            "3,9,8,9,10,9,4,9,99,-1,8",
            "3,3,1108,-1,8,3,4,3,99"
        };
        for (auto const& str : programs_equal) {
            {
                auto p = parseInput(str);
                p.input.push_back(7);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 0 });
            }
            {
                auto p = parseInput(str);
                p.input.push_back(8);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 1 });
            }
            {
                auto p = parseInput(str);
                p.input.push_back(9);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 0 });
            }
        }
    }

    SECTION("Sample Programs Less Than")
    {
        std::string const programs_less_than[] = {
            "3,9,7,9,10,9,4,9,99,-1,8",
            "3,3,1107,-1,8,3,4,3,99"
        };
        for (auto const& str : programs_less_than) {
            {
                auto p = parseInput(str);
                p.input.push_back(7);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 1 });
            }
            {
                auto p = parseInput(str);
                p.input.push_back(8);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 0 });
            }
            {
                auto p = parseInput(str);
                p.input.push_back(9);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 0 });
            }
        }
    }

    SECTION("Sample Programs Jump Tests")
    {
        std::string const programs_jump[] = {
            "3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9",
            "3,3,1105,-1,9,1101,0,0,12,4,12,99,1"
        };
        for (auto const& str : programs_jump) {
            {
                auto p = parseInput(str);
                p.input.push_back(7);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 1 });
            }
            {
                auto p = parseInput(str);
                p.input.push_back(0);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 0 });
            }
            {
                auto p = parseInput(str);
                p.input.push_back(-1);
                executeProgram(p);
                CHECK(p.pc == -1);
                CHECK(p.input.empty());
                CHECK(p.output == std::vector<int>{ 1 });
            }
        }
    }

    SECTION("Larger Example")
    {
        char const sample_input[] = "3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,"
                                    "1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,"
                                    "999,1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99";
        {
            auto p = parseInput(sample_input);
            p.input.push_back(6);
            executeProgram(p);
            CHECK(p.output == std::vector<int>{ 999 });
        }
        {
            auto p = parseInput(sample_input);
            p.input.push_back(7);
            executeProgram(p);
            CHECK(p.output == std::vector<int>{ 999 });
        }
        {
            auto p = parseInput(sample_input);
            p.input.push_back(8);
            executeProgram(p);
            CHECK(p.output == std::vector<int>{ 1000 });
        }
        {
            auto p = parseInput(sample_input);
            p.input.push_back(9);
            executeProgram(p);
            CHECK(p.output == std::vector<int>{ 1001 });
        }
        {
            auto p = parseInput(sample_input);
            p.input.push_back(10);
            executeProgram(p);
            CHECK(p.output == std::vector<int>{ 1001 });
        }
    }
}
