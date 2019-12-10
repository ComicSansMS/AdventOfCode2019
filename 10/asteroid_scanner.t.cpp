#include <asteroid_scanner.hpp>

#include <catch.hpp>

#include <vector>

TEST_CASE("Asteroid Scanner")
{
    char const sample_input[] = ".#..#\n"
                                ".....\n"
                                "#####\n"
                                "....#\n"
                                "...##\n";
    SECTION("Parse Input")
    {
        Map const m = parseInput(sample_input);
        CHECK(m.width == 5);
        CHECK(m.height == 5);
        CHECK(m.asteroid_positions.size() == 10);
        CHECK(m.asteroid_positions == std::vector<Vector2>{
            { 1, 0 }, { 4, 0 }, { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 }, { 4, 2 }, { 4, 3 }, { 3, 4 }, { 4, 4 }
        });
    }

    SECTION("Line of Sight")
    {
        Line l1(Vector2(0, 0), Vector2(15, 15));
        CHECK(l1.angle == Vector2(1, 1));
        CHECK(l1.distance == 15);

        Line l2(Vector2(2, 4), Vector2(8, 16));
        CHECK(l2.angle == Vector2(1, 2));
        CHECK(l2.distance == 6);

        Line l3(Vector2(3, 4), Vector2(2, 2));
        CHECK(l3.angle == Vector2(-1, -2));
        CHECK(l3.distance == 1);

        Line l4(Vector2(3, 4), Vector2(1, 0));
        CHECK(l4.angle == Vector2(-1, -2));
        CHECK(l4.distance == 2);
    }

    SECTION("All Lines of Sight")
    {
        Map m = parseInput(sample_input);
        determineLinesOfSight(m);
        REQUIRE(m.lines_of_sight.size() == m.asteroid_positions.size());
        for (std::size_t i = 0; i < m.lines_of_sight.size(); ++i) {
            REQUIRE(m.lines_of_sight[i].size() == m.asteroid_positions.size());
        }

        CHECK(m.lines_of_sight[0][0].angle == Vector2(0, 0));
        CHECK(m.lines_of_sight[0][0].distance == 0);

        CHECK(m.lines_of_sight[0][1].angle == Vector2(1, 0));
        CHECK(m.lines_of_sight[0][1].distance == 3);

        CHECK(m.lines_of_sight[0][2].angle == Vector2(-1, 2));
        CHECK(m.lines_of_sight[0][2].distance == 1);

        CHECK(m.lines_of_sight[0][3].angle == Vector2(0, 1));
        CHECK(m.lines_of_sight[0][3].distance == 2);

        CHECK(m.lines_of_sight[0][4].angle == Vector2(1, 2));
        CHECK(m.lines_of_sight[0][4].distance == 1);

        CHECK(m.lines_of_sight[1][0].angle == Vector2(-1, 0));
        CHECK(m.lines_of_sight[1][0].distance == 3);

        CHECK(m.lines_of_sight[2][0].angle == Vector2(1, -2));
        CHECK(m.lines_of_sight[2][0].distance == 1);
    }

    SECTION("Count Visible Asteroids")
    {
        Map m = parseInput(sample_input);
        determineLinesOfSight(m);

        CHECK(asteroidsVisibleFrom(m, 0) == 7);
        CHECK(asteroidsVisibleFrom(m, 1) == 7);
        CHECK(asteroidsVisibleFrom(m, 2) == 6);
        CHECK(asteroidsVisibleFrom(m, 3) == 7);
        CHECK(asteroidsVisibleFrom(m, 4) == 7);
        CHECK(asteroidsVisibleFrom(m, 5) == 7);
        CHECK(asteroidsVisibleFrom(m, 6) == 5);
        CHECK(asteroidsVisibleFrom(m, 7) == 7);
        CHECK(asteroidsVisibleFrom(m, 8) == 8);
        CHECK(asteroidsVisibleFrom(m, 9) == 7);
    }

    SECTION("Best Vantage Point")
    {
        {
            Map m = parseInput(sample_input);
            determineLinesOfSight(m);
            auto const [visible, index] = bestVantagePoint(m);
            CHECK(visible == 8);
            CHECK(m.asteroid_positions[index] == Vector2(3, 4));
        }
        {
            char const input[] = "......#.#.\n"
                                 "#..#.#....\n"
                                 "..#######.\n"
                                 ".#.#.###..\n"
                                 ".#..#.....\n"
                                 "..#....#.#\n"
                                 "#..#....#.\n"
                                 ".##.#..###\n"
                                 "##...#..#.\n"
                                 ".#....####\n";
            Map m = parseInput(input);
            determineLinesOfSight(m);
            auto const [visible, index] = bestVantagePoint(m);
            CHECK(visible == 33);
            CHECK(m.asteroid_positions[index] == Vector2(5, 8));
        }
        {
            char const input[] = "#.#...#.#.\n"
                                 ".###....#.\n"
                                 ".#....#...\n"
                                 "##.#.#.#.#\n"
                                 "....#.#.#.\n"
                                 ".##..###.#\n"
                                 "..#...##..\n"
                                 "..##....##\n"
                                 "......#...\n"
                                 ".####.###.\n";
            Map m = parseInput(input);
            determineLinesOfSight(m);
            auto const [visible, index] = bestVantagePoint(m);
            CHECK(visible == 35);
            CHECK(m.asteroid_positions[index] == Vector2(1, 2));
        }
        {
            char const input[] = ".#..#..###\n"
                                 "####.###.#\n"
                                 "....###.#.\n"
                                 "..###.##.#\n"
                                 "##.##.#.#.\n"
                                 "....###..#\n"
                                 "..#.#..#.#\n"
                                 "#..#.#.###\n"
                                 ".##...##.#\n"
                                 ".....#.#..\n";
            Map m = parseInput(input);
            determineLinesOfSight(m);
            auto const [visible, index] = bestVantagePoint(m);
            CHECK(visible == 41);
            CHECK(m.asteroid_positions[index] == Vector2(6, 3));
        }
        {
            char const input[] = ".#..##.###...#######\n"
                                 "##.############..##.\n"
                                 ".#.######.########.#\n"
                                 ".###.#######.####.#.\n"
                                 "#####.##.#.##.###.##\n"
                                 "..#####..#.#########\n"
                                 "####################\n"
                                 "#.####....###.#.#.##\n"
                                 "##.#################\n"
                                 "#####.##.###..####..\n"
                                 "..######..##.#######\n"
                                 "####.##.####...##..#\n"
                                 ".#####..#.######.###\n"
                                 "##...#.##########...\n"
                                 "#.##########.#######\n"
                                 ".####.#.###.###.#.##\n"
                                 "....##.##.###..#####\n"
                                 ".#.#.###########.###\n"
                                 "#.#.#.#####.####.###\n"
                                 "###.##.####.##.#..##\n";
            Map m = parseInput(input);
            determineLinesOfSight(m);
            auto const [visible, index] = bestVantagePoint(m);
            CHECK(visible == 210);
            CHECK(m.asteroid_positions[index] == Vector2(11, 13));
        }

    }
}
