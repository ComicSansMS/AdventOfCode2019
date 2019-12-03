#include <wires_crossing.hpp>

#include <catch.hpp>

#include <vector>
#include <sstream>
#include <string>

TEST_CASE("Wires Crossing")
{
    char const sample_input[] = "R8,U5,L5,D3\nU7,R6,D4,L4";

    char const sample_input2[] = "R75,D30,R83,U83,L12,D49,R71,U7,L72\nU62,R66,U55,R34,D71,R55,D58,R83";

    char const sample_input3[] = "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51\nU98,R91,D20,R16,D67,R40,U7,R15,U6,R7";

    SECTION("Path Segment Equality")
    {
        CHECK(PathSegment{ Direction::Up, 1 } == PathSegment{ Direction::Up, 1 });
        CHECK_FALSE(PathSegment{ Direction::Up, 1 } == PathSegment{ Direction::Down, 1 });
        CHECK_FALSE(PathSegment{ Direction::Up, 1 } == PathSegment{ Direction::Left, 1 });
        CHECK_FALSE(PathSegment{ Direction::Up, 1 } == PathSegment{ Direction::Up, 2 });
        CHECK_FALSE(PathSegment{ Direction::Up, 1 } == PathSegment{ Direction::Up, 3 });
        CHECK_FALSE(PathSegment{ Direction::Up, 1 } == PathSegment{ Direction::Left, 3 });
    }

    SECTION("Parse Input")
    {
        Field const field = parseInput(sample_input);

        CHECK(field.wires[0].path == std::vector<PathSegment>{ {Direction::Right, 8}, { Direction::Up, 5 },
                                                               { Direction::Left, 5 }, {Direction::Down, 3} });
        CHECK(field.wires[1].path == std::vector<PathSegment>{ {Direction::Up, 7}, { Direction::Right, 6 },
                                                               { Direction::Down, 4 }, {Direction::Left, 4} });
    }

    SECTION("Layout Wires")
    {
        Field field = parseInput(sample_input);
        layoutWires(field);
        REQUIRE(field.wires[0].lines.size() == 4);
        CHECK(field.wires[0].lines[0] == Line{ {0, 0}, {8, 0} });
        CHECK(field.wires[0].lines[1] == Line{ {8, 0}, {8, 5} });
        CHECK(field.wires[0].lines[2] == Line{ {8, 5}, {3, 5} });
        CHECK(field.wires[0].lines[3] == Line{ {3, 5}, {3, 2} });

        REQUIRE(field.wires[1].lines.size() == 4);
        CHECK(field.wires[1].lines[0] == Line{ {0, 0}, {0, 7} });
        CHECK(field.wires[1].lines[1] == Line{ {0, 7}, {6, 7} });
        CHECK(field.wires[1].lines[2] == Line{ {6, 7}, {6, 3} });
        CHECK(field.wires[1].lines[3] == Line{ {6, 3}, {2, 3} });
    }

    SECTION("Line Is Vertical")
    {
        CHECK(isVertical(Line{ {1, 2}, {1, 100} }));
        CHECK(isVertical(Line{ {1, 2}, {1, -100} }));
        CHECK_FALSE(isVertical(Line{ {1, 2}, {2, 2} }));
    }

    SECTION("Line length")
    {
        CHECK(length(Line{ {1, 2}, {1, 100} }) == 98);
        CHECK(length(Line{ {1, 2}, {1, -100} }) == 102);
        CHECK(length(Line{ {1, 2}, {2, 2} }) == 1);
    }

    SECTION("Line Intersection")
    {
        CHECK_FALSE(intersect(Line{ {0, 0}, {8, 0} }, Line{ {0, 7}, {6, 7} }));

        REQUIRE(intersect(Line{ {8, 5}, {3, 5} }, Line{ {6, 7}, {6, 3} }));
        CHECK(*intersect(Line{ {8, 5}, {3, 5} }, Line{ {6, 7}, {6, 3} }) == Coordinates{ 6, 5 });

        REQUIRE(intersect(Line{ {0, 0}, {8, 0} }, Line{ {0, 0}, {0, 7} }));
        CHECK(*intersect(Line{ {0, 0}, {8, 0} }, Line{ {0, 0}, {0, 7} }) == Coordinates{ 0, 0 });

        REQUIRE(intersect(Line{ {6, 3}, {2, 3} }, Line{ {3, 5}, {3, 2} }));
        CHECK(*intersect(Line{ {6, 3}, {2, 3} }, Line{ {3, 5}, {3, 2} }) == Coordinates{3, 3});
    }

    SECTION("Line Point Intersection")
    {
        CHECK(intersect(Line{ {6, 7}, {6, 3} }, Coordinates{6, 5}));
        CHECK(intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 6, 7 }));
        CHECK(intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 6, 3 }));
        CHECK(intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 6, 4 }));
        CHECK(intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 6, 6 }));
        CHECK(!intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 7, 3 }));
        CHECK(!intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 8, 3 }));
        CHECK(!intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 6, 2 }));
        CHECK(!intersect(Line{ {6, 7}, {6, 3} }, Coordinates{ 6, 8 }));
    }

    SECTION("Closest Intersection")
    {
        Field field = parseInput(sample_input);
        layoutWires(field);
        auto [coords, dist] = closestIntersection(field.wires[0], field.wires[1]);
        CHECK(coords == Coordinates{ 3, 3 });
        CHECK(dist == 6);

        Field field2 = parseInput(sample_input2);
        layoutWires(field2);
        auto [coords2, dist2] = closestIntersection(field2.wires[0], field2.wires[1]);
        CHECK(coords2 == Coordinates{ 155, 4 });
        CHECK(dist2 == 159);

        Field field3 = parseInput(sample_input3);
        layoutWires(field3);
        auto [coords3, dist3] = closestIntersection(field3.wires[0], field3.wires[1]);
        CHECK(coords3 == Coordinates{ 124, 11 });
        CHECK(dist3 == 135);
    }

    SECTION("Walk Intersection Points")
    {
        Field field = parseInput(sample_input);
        layoutWires(field);
        auto const steps = walkIntersectionPoints(field);
        CHECK(steps == std::array<int, 2>{15, 15});
        CHECK(steps[0] + steps[1] == 30);

        Field field2 = parseInput(sample_input2);
        layoutWires(field2);
        auto const steps2 = walkIntersectionPoints(field2);
        CHECK(steps2 == std::array<int, 2>{206, 404});
        CHECK(steps2[0] + steps2[1] == 610);

        Field field3 = parseInput(sample_input3);
        layoutWires(field3);
        auto const steps3 = walkIntersectionPoints(field3);
        CHECK(steps3 == std::array<int, 2>{154, 256});
        CHECK(steps3[0] + steps3[1] == 410);
    }
}
