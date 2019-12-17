#include <vacuum_robot.hpp>

#include <intcode_mock.hpp>

#include <catch.hpp>

#include <sstream>

struct IntcodeMock : public IntcodeProgramMock_Base {
    Vector2 robot_position;
    Direction robot_direction;
    std::string map_string;
    int map_width;
    int map_height;

    IntcodeMock(std::string_view map, int width, int height, Vector2 position, Direction direction)
        :robot_position(position), robot_direction(direction), map_string(map), map_width(width), map_height(height)
    {
        pc = 0;
    }
};

void executeProgram(IntcodeMock& p) {
    if (p.pc < 0) { return; }
    for (int y = 0; y < p.map_height; ++y) {
        for (int x = 0; x < p.map_width; ++x) {
            if (Vector2(x, y) == p.robot_position) {
                if (p.robot_direction == Direction::Up) {
                    p.output.push_back('^');
                } else if (p.robot_direction == Direction::Down) {
                    p.output.push_back('v');
                } else if (p.robot_direction == Direction::Left) {
                    p.output.push_back('<');
                } else {
                    REQUIRE(p.robot_direction == Direction::Right);
                    p.output.push_back('>');
                }
            } else {
                p.output.push_back(p.map_string[y * (p.map_width + 1) + x]);
            }
        }
        REQUIRE(p.map_string[y * (p.map_width + 1) + p.map_width] == '\n');
        p.output.push_back('\n');
    }
    p.pc = -4;
    p.resume_point = 42;
}

TEST_CASE("Vacuum Robot")
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
        std::stringstream sstr;
        sstr << Vector2(5, -12);
        CHECK(sstr.str() == "[5,-12]");
    }

    char const sample_map[] = "..#..........\n"
                              "..#..........\n"
                              "#######...###\n"
                              "#.#...#...#.#\n"
                              "#############\n"
                              "..#...#...#..\n"
                              "..#####...#..\n\n";
    IntcodeMock mock(sample_map, 13, 7, Vector2(10, 6), Direction::Up);
    Intcode p(mock);

    SECTION("Parse Map")
    {
        p.execute();
        Map const m = parseMap(p.output());
        CHECK(m.map_width == 13);
        CHECK(m.map_height == 7);
        CHECK(m.robot_position == Vector2(10, 6));
        CHECK(m.robot_direction == Direction::Up);

        std::stringstream sstr;
        sstr << m;
        CHECK(sstr.str() == "..#..........\n"
                            "..#..........\n"
                            "#######...###\n"
                            "#.#...#...#.#\n"
                            "#############\n"
                            "..#...#...#..\n"
                            "..#####...^..\n");
    }

    SECTION("Detect Intersections")
    {
        p.execute();
        Map m = parseMap(p.output());
        markIntersections(m);

        std::stringstream sstr;
        sstr << m;
        CHECK(sstr.str() == "..#..........\n"
                            "..#..........\n"
                            "##O####...###\n"
                            "#.#...#...#.#\n"
                            "##O###O###O##\n"
                            "..#...#...#..\n"
                            "..#####...^..\n");

        CHECK(alignment1(m) == 76);
    }
}
