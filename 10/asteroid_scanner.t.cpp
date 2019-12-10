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
    }

    SECTION("Dot Product")
    {
        CHECK(dot(Vector2(1, 1), Vector2(2, 2)) == 4);
        CHECK(dot(Vector2(1, 2), Vector2(-1, 2)) == 3);
        CHECK(dot(Vector2(1, 2), Vector2(2, -1)) == 0);
    }

    char const sample_input2[] = ".#....#####...#..\n"
                                 "##...##.#####..##\n"
                                 "##...#...#.#####.\n"
                                 "..#.....#...###..\n"
                                 "..#.#.....#....##\n";
    SECTION("Vaporize")
    {
        auto m = parseInput(sample_input2);
        determineLinesOfSight(m);
        auto const [visible, index] = bestVantagePoint(m);
        auto const all_lines = m.lines_of_sight[index];
        CHECK(!all_lines.empty());
        auto const t = determineTargets(m, index);
        CHECK(t.size() == m.asteroid_positions.size() - 1);
        auto vaporized = vaporize(t);
        CHECK(vaporized.size() == t.size());
        //   01234567890123456
        // 0 .#....###24...#..
        // 1 ##...##.13#67..9#
        // 2 ##...#...5.8####.
        // 3 ..#.....X...###..
        // 4 ..#.#.....#....##
        CHECK(vaporized[0] == Vector2(8, 1));       // 1
        CHECK(vaporized[1] == Vector2(9, 0));       // 2
        CHECK(vaporized[2] == Vector2(9, 1));       // 3
        CHECK(vaporized[3] == Vector2(10, 0));      // 4
        CHECK(vaporized[4] == Vector2(9, 2));       // 5
        CHECK(vaporized[5] == Vector2(11, 1));      // 6
        CHECK(vaporized[6] == Vector2(12, 1));      // 7
        CHECK(vaporized[7] == Vector2(11, 2));      // 8
        CHECK(vaporized[8] == Vector2(15, 1));      // 9

        //   01234567890123456
        // 0 .#....###.....#..
        // 1 ##...##...#.....#
        // 2 ##...#......1234.
        // 3 ..#.....X...5##..
        // 4 ..#.9.....8....76
        CHECK(vaporized[9] == Vector2(12, 2));      // 1
        CHECK(vaporized[10] == Vector2(13, 2));     // 2
        CHECK(vaporized[11] == Vector2(14, 2));     // 3
        CHECK(vaporized[12] == Vector2(15, 2));     // 4
        CHECK(vaporized[13] == Vector2(12, 3));     // 5
        CHECK(vaporized[14] == Vector2(16, 4));     // 6
        CHECK(vaporized[15] == Vector2(15, 4));     // 7
        CHECK(vaporized[16] == Vector2(10, 4));     // 8
        CHECK(vaporized[17] == Vector2(4, 4));      // 9

        //   01234567890123456
        // 0 .8....###.....#..
        // 1 56...9#...#.....#
        // 2 34...7...........
        // 3 ..2.....X....##..
        // 4 ..1..............
        CHECK(vaporized[18] == Vector2(2, 4));      // 1
        CHECK(vaporized[19] == Vector2(2, 3));      // 2
        CHECK(vaporized[20] == Vector2(0, 2));      // 3
        CHECK(vaporized[21] == Vector2(1, 2));      // 4
        CHECK(vaporized[22] == Vector2(0, 1));      // 5
        CHECK(vaporized[23] == Vector2(1, 1));      // 6
        CHECK(vaporized[24] == Vector2(5, 2));      // 7
        CHECK(vaporized[25] == Vector2(1, 0));      // 8
        CHECK(vaporized[26] == Vector2(5, 1));      // 9

        //   01234567890123456
        // 0 ......234.....6..
        // 1 ......1...5.....7
        // 2 .................
        // 3 ........X....89..
        // 4 .................
        CHECK(vaporized[27] == Vector2(6, 1));      // 1
        CHECK(vaporized[28] == Vector2(6, 0));      // 2
        CHECK(vaporized[29] == Vector2(7, 0));      // 3
        CHECK(vaporized[30] == Vector2(8, 0));      // 4
        CHECK(vaporized[31] == Vector2(10, 1));     // 5
        CHECK(vaporized[32] == Vector2(14, 0));     // 6
        CHECK(vaporized[33] == Vector2(16, 1));     // 7
        CHECK(vaporized[34] == Vector2(13, 3));     // 8
        CHECK(vaporized[35] == Vector2(14, 3));     // 9
        CHECK(vaporized.size() == 36);
    }

    SECTION("Big Input")
    {
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
            auto const t = determineTargets(m, index);
            CHECK(t.size() == m.asteroid_positions.size() - 1);
            auto const vaporized = vaporize(t);
            REQUIRE(vaporized.size() == t.size());

            // The 1st asteroid to be vaporized is at 11, 12.
            CHECK(vaporized[0] == Vector2(11, 12));
            // The 2nd asteroid to be vaporized is at 12, 1.
            CHECK(vaporized[1] == Vector2(12, 1));
            // The 3rd asteroid to be vaporized is at 12, 2.
            CHECK(vaporized[2] == Vector2(12, 2));
            // The 10th asteroid to be vaporized is at 12, 8.
            CHECK(vaporized[9] == Vector2(12, 8));
            // The 20th asteroid to be vaporized is at 16, 0.
            CHECK(vaporized[19] == Vector2(16, 0));
            // The 50th asteroid to be vaporized is at 16, 9.
            CHECK(vaporized[49] == Vector2(16, 9));
            // The 100th asteroid to be vaporized is at 10, 16.
            CHECK(vaporized[99] == Vector2(10, 16));
            // The 199th asteroid to be vaporized is at 9, 6.
            CHECK(vaporized[198] == Vector2(9, 6));
            // The 200th asteroid to be vaporized is at 8, 2.
            CHECK(vaporized[199] == Vector2(8, 2));
            // The 201st asteroid to be vaporized is at 10, 9.
            CHECK(vaporized[200] == Vector2(10, 9));
            // The 299th and final asteroid to be vaporized is at 11, 1.
            CHECK(vaporized[298] == Vector2(11, 1));
            CHECK(vaporized.size() == 299);
        }
    }
}
