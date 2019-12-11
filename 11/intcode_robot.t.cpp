#include <intcode_robot.hpp>

#include <catch.hpp>

#include <sstream>
#include <vector>

TEST_CASE("Intcode Robot")
{
    SECTION("Vector2")
    {
        CHECK(Vector2().x == 0);
        CHECK(Vector2().y == 0);
        CHECK(Vector2(1, 2).x == 1);
        CHECK(Vector2(1, 2).y == 2);
        CHECK(Vector2(1, 2) == Vector2(1, 2));
        CHECK_FALSE(Vector2(1, 2) == Vector2(0, 2));
        CHECK_FALSE(Vector2(1, 2) == Vector2(1, 0));
        CHECK_FALSE(Vector2(1, 2) == Vector2(0, 0));
    }

    SECTION("Robot State Construction")
    {
        RobotState s;
        CHECK(s.position == Vector2(0, 0));
        CHECK(s.direction == Direction::Up);
        CHECK(s.grid.empty());
        CHECK(s.grid_dimensions.min == Vector2(0, 0));
        CHECK(s.grid_dimensions.max == Vector2(0, 0));
    }

    SECTION("Turn Left")
    {
        CHECK(turnLeft(Direction::Up) == Direction::Left);
        CHECK(turnLeft(Direction::Left) == Direction::Down);
        CHECK(turnLeft(Direction::Down) == Direction::Right);
        CHECK(turnLeft(Direction::Right) == Direction::Up);
    }

    SECTION("Turn Right")
    {
        CHECK(turnRight(Direction::Up) == Direction::Right);
        CHECK(turnRight(Direction::Right) == Direction::Down);
        CHECK(turnRight(Direction::Down) == Direction::Left);
        CHECK(turnRight(Direction::Left) == Direction::Up);
    }

    SECTION("Move Forward")
    {
        CHECK(moveForward(Vector2(1, 1), Direction::Up) == Vector2(1, 2));
        CHECK(moveForward(Vector2(1, 1), Direction::Left) == Vector2(0, 1));
        CHECK(moveForward(Vector2(1, 1), Direction::Down) == Vector2(1, 0));
        CHECK(moveForward(Vector2(1, 1), Direction::Right) == Vector2(2, 1));
    }

    SECTION("Robot Input Processing")
    {
        RobotState s;
        // .....
        // .....
        // ..^..
        // .....
        // .....
        CHECK(scanPanel(s) == Color::Black);
        // .....
        // .....
        // .<#..
        // .....
        // .....
        processInput(s, std::vector<Word>{1, 0});
        CHECK(s.position == Vector2(-1, 0));
        CHECK(s.direction == Direction::Left);
        CHECK(s.grid_dimensions.min == Vector2(-1, 0));
        CHECK(s.grid_dimensions.max == Vector2(0, 0));
        CHECK(s.grid.size() == 1);
        REQUIRE(s.grid.find(Vector2(0, 0)) != s.grid.end());
        CHECK(s.grid.find(Vector2(0, 0))->second == Color::White);
        CHECK(scanPanel(s) == Color::Black);
        // .....
        // .....
        // ..#..
        // .v...
        // .....
        processInput(s, std::vector<Word>{0, 0});
        CHECK(s.position == Vector2(-1, -1));
        CHECK(s.direction == Direction::Down);
        CHECK(s.grid_dimensions.min == Vector2(-1, -1));
        CHECK(s.grid_dimensions.max == Vector2(0, 0));
        CHECK(s.grid.size() == 2);
        REQUIRE(s.grid.find(Vector2(0, 0)) != s.grid.end());
        CHECK(s.grid.find(Vector2(0, 0))->second == Color::White);
        REQUIRE(s.grid.find(Vector2(-1, 0)) != s.grid.end());
        CHECK(s.grid.find(Vector2(-1, 0))->second == Color::Black);
        CHECK(scanPanel(s) == Color::Black);
        // .....
        // .....
        // ..^..
        // .##..
        // .....
        processInput(s, std::vector<Word>{1, 0});
        processInput(s, std::vector<Word>{1, 0});
        CHECK(s.position == Vector2(0, 0));
        CHECK(s.direction == Direction::Up);
        CHECK(s.grid_dimensions.min == Vector2(-1, -1));
        CHECK(s.grid_dimensions.max == Vector2(0, 0));
        CHECK(s.grid.size() == 4);
        REQUIRE(s.grid.find(Vector2(0, -1)) != s.grid.end());
        CHECK(s.grid.find(Vector2(0, -1))->second == Color::White);
        CHECK(scanPanel(s) == Color::White);
        // .....
        // ..<#.
        // ...#.
        // .##..
        // .....
        processInput(s, std::vector<Word>{0, 1});
        processInput(s, std::vector<Word>{1, 0});
        processInput(s, std::vector<Word>{1, 0});
        CHECK(s.position == Vector2(0, 1));
        CHECK(s.direction == Direction::Left);
        CHECK(s.grid_dimensions.min == Vector2(-1, -1));
        CHECK(s.grid_dimensions.max == Vector2(1, 1));
        CHECK(s.grid.size() == 6);
        REQUIRE(s.grid.find(Vector2(1, 0)) != s.grid.end());
        CHECK(s.grid.find(Vector2(1, 0))->second == Color::White);
        REQUIRE(s.grid.find(Vector2(1, 1)) != s.grid.end());
        CHECK(s.grid.find(Vector2(1, 1))->second == Color::White);
        CHECK(scanPanel(s) == Color::Black);
    }

    SECTION("Intcode Execution")
    {
        char const* intcode_program = "104,1,104,0,"
                                      "104,0,104,0,"
                                      "104,1,104,0,"
                                      "104,1,104,0,"
                                      "3,0,"
                                      "104,0,104,1,"
                                      "104,1,104,0,"
                                      "104,1,104,0,"
                                      "3,1,99";
        IntcodeRobot r(intcode_program);
        r.run();
        CHECK(r.robot.grid.size() == 6);
        CHECK(r.program.memory[0] == Color::White);
        CHECK(r.program.memory[1] == Color::Black);
    }

    SECTION("Grid Output")
    {
        RobotState s;
        processInput(s, std::vector<Word>{1, 0});
        processInput(s, std::vector<Word>{0, 0});
        processInput(s, std::vector<Word>{1, 0});
        processInput(s, std::vector<Word>{1, 0});
        processInput(s, std::vector<Word>{0, 1});
        processInput(s, std::vector<Word>{1, 0});
        processInput(s, std::vector<Word>{1, 0});
        std::stringstream sstr;
        sstr << s;
        CHECK(sstr.str() == ".....\n"
                            "...#.\n"
                            "...#.\n"
                            ".##..\n"
                            ".....\n");
    }

    SECTION("Grid Output From Intcode")
    {
        char const* intcode_program = "104,1,104,0,"
                                      "104,0,104,0,"
                                      "104,1,104,0,"
                                      "104,1,104,0,"
                                      "3,0,"
                                      "104,0,104,1,"
                                      "104,1,104,0,"
                                      "104,1,104,0,"
                                      "3,1,99";
        IntcodeRobot r(intcode_program);
        r.run();
        std::stringstream sstr;
        sstr << r.robot;
        CHECK(sstr.str() == ".....\n"
                            "...#.\n"
                            "...#.\n"
                            ".##..\n"
                            ".....\n");
    }
}
