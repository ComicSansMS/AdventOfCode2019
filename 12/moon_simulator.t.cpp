#include <moon_simulator.hpp>

#include <catch.hpp>

#include <sstream>
#include <vector>

TEST_CASE("Moon Simulator")
{
    SECTION("Vector3 Construction")
    {
        CHECK(Vector3().x == 0);
        CHECK(Vector3().y == 0);
        CHECK(Vector3().z == 0);

        CHECK(Vector3(1, 2, 3).x == 1);
        CHECK(Vector3(1, 2, 3).y == 2);
        CHECK(Vector3(1, 2, 3).z == 3);

        CHECK(Vector3(1, 2, 3) == Vector3(1, 2, 3));
        CHECK_FALSE(Vector3(1, 2, 3) == Vector3(0, 2, 3));
        CHECK_FALSE(Vector3(1, 2, 3) == Vector3(1, 0, 3));
        CHECK_FALSE(Vector3(1, 2, 3) == Vector3(1, 2, 0));
        CHECK_FALSE(Vector3(1, 2, 3) == Vector3(1, 0, 0));
        CHECK_FALSE(Vector3(1, 2, 3) == Vector3(0, 2, 0));
        CHECK_FALSE(Vector3(1, 2, 3) == Vector3(0, 0, 3));
        CHECK_FALSE(Vector3(1, 2, 3) == Vector3(0, 0, 0));
    }

    SECTION("Vector3 Output")
    {
        std::stringstream sstr;
        sstr << Vector3(1, 2, 3);
        CHECK(sstr.str() == "[1,2,3]");
    }

    SECTION("Vector3 Addition")
    {
        CHECK(Vector3(1, 2, 3) + Vector3(1, 2, 3) == Vector3(2, 4, 6));
        CHECK(Vector3(1, 2, 3) + Vector3(5, 9, 14) == Vector3(6, 11, 17));
        CHECK(Vector3(1, 2, 3) + Vector3(-1, -2, -3) == Vector3());
    }

    char const sample_input[] = "<x=-1, y=0, z=2>\n"
                                "<x=2, y=-10, z=-7>\n"
                                "<x=4, y=-8, z=8>\n"
                                "<x=3, y=5, z=-1>\n";

    SECTION("Parse Input")
    {
        PlanetarySystem p = parseInput(sample_input);
        REQUIRE(p.size() == 4);
        CHECK(p[0].position == Vector3(-1, 0, 2));
        CHECK(p[1].position == Vector3(2, -10, -7));
        CHECK(p[2].position == Vector3(4, -8, 8));
        CHECK(p[3].position == Vector3(3, 5, -1));

        CHECK(p[0].velocity == Vector3());
        CHECK(p[1].velocity == Vector3());
        CHECK(p[2].velocity == Vector3());
        CHECK(p[3].velocity == Vector3());
    }

    SECTION("Gravity Between Two Moons")
    {
        Moon ganymede{ Vector3(3, 12, 6), Vector3() };
        Moon callisto{ Vector3(5, -1, 6), Vector3() };
        applyGravity(ganymede, callisto);
        CHECK(ganymede.velocity == Vector3(1, -1, 0));
        CHECK(callisto.velocity == Vector3(-1, 1, 0));
        // position is unchanged
        CHECK(ganymede.position == Vector3(3, 12, 6));
        CHECK(callisto.position == Vector3(5, -1, 6));

        applyGravity(ganymede, callisto);
        CHECK(ganymede.velocity == Vector3(2, -2, 0));
        CHECK(callisto.velocity == Vector3(-2, 2, 0));
    }

    SECTION("Apply Velocity")
    {
        Moon europa{ Vector3(1, 2, 3), Vector3(-2, 0, 3) };
        applyVelocity(europa);
        CHECK(europa.position == Vector3(-1, 2, 6));
        // velocity is unchanges
        CHECK(europa.velocity == Vector3(-2, 0, 3));
    }

    SECTION("Potential Energy")
    {
        PlanetarySystem p;
        p[0] = Moon{ Vector3( 2,  1, -3), Vector3(-3, -2,  1) };
        p[1] = Moon{ Vector3( 1, -8,  0), Vector3(-1,  1,  3) };
        p[2] = Moon{ Vector3( 3, -6,  1), Vector3( 3,  2, -3) };
        p[3] = Moon{ Vector3( 2,  0,  4), Vector3( 1, -1, -1) };
        CHECK(potentialEnergy(p[0]) ==  6);
        CHECK(potentialEnergy(p[1]) ==  9);
        CHECK(potentialEnergy(p[2]) == 10);
        CHECK(potentialEnergy(p[3]) ==  6);
    }

    SECTION("Kinetic Energy")
    {
        PlanetarySystem p;
        p[0] = Moon{ Vector3( 2,  1, -3), Vector3(-3, -2,  1) };
        p[1] = Moon{ Vector3( 1, -8,  0), Vector3(-1,  1,  3) };
        p[2] = Moon{ Vector3( 3, -6,  1), Vector3( 3,  2, -3) };
        p[3] = Moon{ Vector3( 2,  0,  4), Vector3( 1, -1, -1) };
        CHECK(kineticEnergy(p[0]) == 6);
        CHECK(kineticEnergy(p[1]) == 5);
        CHECK(kineticEnergy(p[2]) == 8);
        CHECK(kineticEnergy(p[3]) == 3);
    }

    SECTION("Total Energy")
    {
        PlanetarySystem p;
        p[0] = Moon{ Vector3( 2,  1, -3), Vector3(-3, -2,  1) };
        p[1] = Moon{ Vector3( 1, -8,  0), Vector3(-1,  1,  3) };
        p[2] = Moon{ Vector3( 3, -6,  1), Vector3( 3,  2, -3) };
        p[3] = Moon{ Vector3( 2,  0,  4), Vector3( 1, -1, -1) };
        CHECK(totalEnergy(p) == 179);
    }

    SECTION("Simulate")
    {
        PlanetarySystem p = parseInput(sample_input);
        // After 0 steps:
        // pos=<x=-1, y=  0, z= 2>, vel=<x= 0, y= 0, z= 0>
        // pos=<x= 2, y=-10, z=-7>, vel=<x= 0, y= 0, z= 0>
        // pos=<x= 4, y= -8, z= 8>, vel=<x= 0, y= 0, z= 0>
        // pos=<x= 3, y=  5, z=-1>, vel=<x= 0, y= 0, z= 0>
        CHECK(p[0].position == Vector3(-1,   0,  2));
        CHECK(p[1].position == Vector3( 2, -10, -7));
        CHECK(p[2].position == Vector3( 4,  -8,  8));
        CHECK(p[3].position == Vector3( 3,   5, -1));

        CHECK(p[0].velocity == Vector3(0, 0, 0));
        CHECK(p[1].velocity == Vector3(0, 0, 0));
        CHECK(p[2].velocity == Vector3(0, 0, 0));
        CHECK(p[3].velocity == Vector3(0, 0, 0));

        // After 1 step:
        // pos=<x= 2, y=-1, z= 1>, vel=<x= 3, y=-1, z=-1>
        // pos=<x= 3, y=-7, z=-4>, vel=<x= 1, y= 3, z= 3>
        // pos=<x= 1, y=-7, z= 5>, vel=<x=-3, y= 1, z=-3>
        // pos=<x= 2, y= 2, z= 0>, vel=<x=-1, y=-3, z= 1>
        p = simulate(p);
        CHECK(p[0].position == Vector3( 2, -1,  1));
        CHECK(p[1].position == Vector3( 3, -7, -4));
        CHECK(p[2].position == Vector3( 1, -7,  5));
        CHECK(p[3].position == Vector3( 2,  2,  0));

        CHECK(p[0].velocity == Vector3( 3, -1, -1));
        CHECK(p[1].velocity == Vector3( 1,  3,  3));
        CHECK(p[2].velocity == Vector3(-3,  1, -3));
        CHECK(p[3].velocity == Vector3(-1, -3,  1));

        // After 2 steps:
        // pos=<x= 5, y=-3, z=-1>, vel=<x= 3, y=-2, z=-2>
        // pos=<x= 1, y=-2, z= 2>, vel=<x=-2, y= 5, z= 6>
        // pos=<x= 1, y=-4, z=-1>, vel=<x= 0, y= 3, z=-6>
        // pos=<x= 1, y=-4, z= 2>, vel=<x=-1, y=-6, z= 2>
        p = simulate(p);
        CHECK(p[0].position == Vector3( 5, -3, -1));
        CHECK(p[1].position == Vector3( 1, -2,  2));
        CHECK(p[2].position == Vector3( 1, -4, -1));
        CHECK(p[3].position == Vector3( 1, -4,  2));

        CHECK(p[0].velocity == Vector3( 3, -2, -2));
        CHECK(p[1].velocity == Vector3(-2,  5,  6));
        CHECK(p[2].velocity == Vector3( 0,  3, -6));
        CHECK(p[3].velocity == Vector3(-1, -6,  2));

        // After 3 steps:
        // pos=<x= 5, y=-6, z=-1>, vel=<x= 0, y=-3, z= 0>
        // pos=<x= 0, y= 0, z= 6>, vel=<x=-1, y= 2, z= 4>
        // pos=<x= 2, y= 1, z=-5>, vel=<x= 1, y= 5, z=-4>
        // pos=<x= 1, y=-8, z= 2>, vel=<x= 0, y=-4, z= 0>
        p = simulate(p);
        CHECK(p[0].position == Vector3( 5, -6, -1));
        CHECK(p[1].position == Vector3( 0,  0,  6));
        CHECK(p[2].position == Vector3( 2,  1, -5));
        CHECK(p[3].position == Vector3( 1, -8,  2));

        CHECK(p[0].velocity == Vector3( 0, -3,  0));
        CHECK(p[1].velocity == Vector3(-1,  2,  4));
        CHECK(p[2].velocity == Vector3( 1,  5, -4));
        CHECK(p[3].velocity == Vector3( 0, -4,  0));

        // After 4 steps:
        // pos=<x= 2, y=-8, z= 0>, vel=<x=-3, y=-2, z= 1>
        // pos=<x= 2, y= 1, z= 7>, vel=<x= 2, y= 1, z= 1>
        // pos=<x= 2, y= 3, z=-6>, vel=<x= 0, y= 2, z=-1>
        // pos=<x= 2, y=-9, z= 1>, vel=<x= 1, y=-1, z=-1>
        p = simulate(p);
        CHECK(p[0].position == Vector3( 2, -8,  0));
        CHECK(p[1].position == Vector3( 2,  1,  7));
        CHECK(p[2].position == Vector3( 2,  3, -6));
        CHECK(p[3].position == Vector3( 2, -9,  1));

        CHECK(p[0].velocity == Vector3(-3, -2,  1));
        CHECK(p[1].velocity == Vector3( 2,  1,  1));
        CHECK(p[2].velocity == Vector3( 0,  2, -1));
        CHECK(p[3].velocity == Vector3( 1, -1, -1));

        // After 5 steps:
        // pos=<x=-1, y=-9, z= 2>, vel=<x=-3, y=-1, z= 2>
        // pos=<x= 4, y= 1, z= 5>, vel=<x= 2, y= 0, z=-2>
        // pos=<x= 2, y= 2, z=-4>, vel=<x= 0, y=-1, z= 2>
        // pos=<x= 3, y=-7, z=-1>, vel=<x= 1, y= 2, z=-2>
        p = simulate(p);
        CHECK(p[0].position == Vector3(-1, -9,  2));
        CHECK(p[1].position == Vector3( 4,  1,  5));
        CHECK(p[2].position == Vector3( 2,  2, -4));
        CHECK(p[3].position == Vector3( 3, -7, -1));

        CHECK(p[0].velocity == Vector3(-3, -1,  2));
        CHECK(p[1].velocity == Vector3( 2,  0, -2));
        CHECK(p[2].velocity == Vector3( 0, -1,  2));
        CHECK(p[3].velocity == Vector3( 1,  2, -2));

        // After 6 steps:
        // pos=<x=-1, y=-7, z= 3>, vel=<x= 0, y= 2, z= 1>
        // pos=<x= 3, y= 0, z= 0>, vel=<x=-1, y=-1, z=-5>
        // pos=<x= 3, y=-2, z= 1>, vel=<x= 1, y=-4, z= 5>
        // pos=<x= 3, y=-4, z=-2>, vel=<x= 0, y= 3, z=-1>
        p = simulate(p);
        CHECK(p[0].position == Vector3(-1, -7,  3));
        CHECK(p[1].position == Vector3( 3,  0,  0));
        CHECK(p[2].position == Vector3( 3, -2,  1));
        CHECK(p[3].position == Vector3( 3, -4, -2));

        CHECK(p[0].velocity == Vector3( 0,  2,  1));
        CHECK(p[1].velocity == Vector3(-1, -1, -5));
        CHECK(p[2].velocity == Vector3( 1, -4,  5));
        CHECK(p[3].velocity == Vector3( 0,  3, -1));

        // After 7 steps:
        // pos=<x= 2, y=-2, z= 1>, vel=<x= 3, y= 5, z=-2>
        // pos=<x= 1, y=-4, z=-4>, vel=<x=-2, y=-4, z=-4>
        // pos=<x= 3, y=-7, z= 5>, vel=<x= 0, y=-5, z= 4>
        // pos=<x= 2, y= 0, z= 0>, vel=<x=-1, y= 4, z= 2>
        // 
        p = simulate(p);
        CHECK(p[0].position == Vector3(2, -2,  1));
        CHECK(p[1].position == Vector3(1, -4, -4));
        CHECK(p[2].position == Vector3(3, -7,  5));
        CHECK(p[3].position == Vector3(2,  0,  0));

        CHECK(p[0].velocity == Vector3( 3,  5, -2));
        CHECK(p[1].velocity == Vector3(-2, -4, -4));
        CHECK(p[2].velocity == Vector3( 0, -5,  4));
        CHECK(p[3].velocity == Vector3(-1,  4,  2));

        // After 8 steps:
        // pos=<x= 5, y= 2, z=-2>, vel=<x= 3, y= 4, z=-3>
        // pos=<x= 2, y=-7, z=-5>, vel=<x= 1, y=-3, z=-1>
        // pos=<x= 0, y=-9, z= 6>, vel=<x=-3, y=-2, z= 1>
        // pos=<x= 1, y= 1, z= 3>, vel=<x=-1, y= 1, z= 3>
        p = simulate(p);
        CHECK(p[0].position == Vector3( 5,  2, -2));
        CHECK(p[1].position == Vector3( 2, -7, -5));
        CHECK(p[2].position == Vector3( 0, -9,  6));
        CHECK(p[3].position == Vector3( 1,  1,  3));

        CHECK(p[0].velocity == Vector3( 3,  4, -3));
        CHECK(p[1].velocity == Vector3( 1, -3, -1));
        CHECK(p[2].velocity == Vector3(-3, -2,  1));
        CHECK(p[3].velocity == Vector3(-1,  1,  3));

        // After 9 steps:
        // pos=<x= 5, y= 3, z=-4>, vel=<x= 0, y= 1, z=-2>
        // pos=<x= 2, y=-9, z=-3>, vel=<x= 0, y=-2, z= 2>
        // pos=<x= 0, y=-8, z= 4>, vel=<x= 0, y= 1, z=-2>
        // pos=<x= 1, y= 1, z= 5>, vel=<x= 0, y= 0, z= 2>
        p = simulate(p);
        CHECK(p[0].position == Vector3( 5,  3, -4));
        CHECK(p[1].position == Vector3( 2, -9, -3));
        CHECK(p[2].position == Vector3( 0, -8,  4));
        CHECK(p[3].position == Vector3( 1,  1,  5));

        CHECK(p[0].velocity == Vector3( 0,  1, -2));
        CHECK(p[1].velocity == Vector3( 0, -2,  2));
        CHECK(p[2].velocity == Vector3( 0,  1, -2));
        CHECK(p[3].velocity == Vector3( 0,  0,  2));

        // After 10 steps:
        // pos=<x= 2, y= 1, z=-3>, vel=<x=-3, y=-2, z= 1>
        // pos=<x= 1, y=-8, z= 0>, vel=<x=-1, y= 1, z= 3>
        // pos=<x= 3, y=-6, z= 1>, vel=<x= 3, y= 2, z=-3>
        // pos=<x= 2, y= 0, z= 4>, vel=<x= 1, y=-1, z=-1>
        p = simulate(p);
        CHECK(p[0].position == Vector3( 2,  1, -3));
        CHECK(p[1].position == Vector3( 1, -8,  0));
        CHECK(p[2].position == Vector3( 3, -6,  1));
        CHECK(p[3].position == Vector3( 2,  0,  4));

        CHECK(p[0].velocity == Vector3(-3, -2,  1));
        CHECK(p[1].velocity == Vector3(-1,  1,  3));
        CHECK(p[2].velocity == Vector3( 3,  2, -3));
        CHECK(p[3].velocity == Vector3( 1, -1, -1));
        CHECK(totalEnergy(p) == 179);
    }

    char const sample_input2[] = "<x=-8, y=-10, z=0>\n"
                                 "<x=5, y=5, z=10>\n"
                                 "<x=2, y=-7, z=3>\n"
                                 "<x=9, y=-8, z=-3>\n";
    SECTION("Simulation #2")
    {
        PlanetarySystem p = parseInput(sample_input2);
        // After 0 steps:
        // pos=<x= -8, y=-10, z=  0>, vel=<x=  0, y=  0, z=  0>
        // pos=<x=  5, y=  5, z= 10>, vel=<x=  0, y=  0, z=  0>
        // pos=<x=  2, y= -7, z=  3>, vel=<x=  0, y=  0, z=  0>
        // pos=<x=  9, y= -8, z= -3>, vel=<x=  0, y=  0, z=  0>
        CHECK(p[0].position == Vector3( -8, -10,   0));
        CHECK(p[1].position == Vector3(  5,   5,  10));
        CHECK(p[2].position == Vector3(  2,  -7,   3));
        CHECK(p[3].position == Vector3(  9,  -8,  -3));

        CHECK(p[0].velocity == Vector3(  0,   0,   0));
        CHECK(p[1].velocity == Vector3(  0,   0,   0));
        CHECK(p[2].velocity == Vector3(  0,   0,   0));
        CHECK(p[3].velocity == Vector3(  0,   0,   0));

        // After 10 steps:
        // pos=<x= -9, y=-10, z=  1>, vel=<x= -2, y= -2, z= -1>
        // pos=<x=  4, y= 10, z=  9>, vel=<x= -3, y=  7, z= -2>
        // pos=<x=  8, y=-10, z= -3>, vel=<x=  5, y= -1, z= -2>
        // pos=<x=  5, y=-10, z=  3>, vel=<x=  0, y= -4, z=  5>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3( -9, -10,   1));
        CHECK(p[1].position == Vector3(  4,  10,   9));
        CHECK(p[2].position == Vector3(  8, -10,  -3));
        CHECK(p[3].position == Vector3(  5, -10,   3));

        CHECK(p[0].velocity == Vector3( -2,  -2,  -1));
        CHECK(p[1].velocity == Vector3( -3,   7,  -2));
        CHECK(p[2].velocity == Vector3(  5,  -1,  -2));
        CHECK(p[3].velocity == Vector3(  0,  -4,   5));

        // After 20 steps:
        // pos=<x=-10, y=  3, z= -4>, vel=<x= -5, y=  2, z=  0>
        // pos=<x=  5, y=-25, z=  6>, vel=<x=  1, y=  1, z= -4>
        // pos=<x= 13, y=  1, z=  1>, vel=<x=  5, y= -2, z=  2>
        // pos=<x=  0, y=  1, z=  7>, vel=<x= -1, y= -1, z=  2>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3(-10,   3,  -4));
        CHECK(p[1].position == Vector3(  5, -25,   6));
        CHECK(p[2].position == Vector3( 13,   1,   1));
        CHECK(p[3].position == Vector3(  0,   1,   7));

        CHECK(p[0].velocity == Vector3( -5,   2,   0));
        CHECK(p[1].velocity == Vector3(  1,   1,  -4));
        CHECK(p[2].velocity == Vector3(  5,  -2,   2));
        CHECK(p[3].velocity == Vector3( -1,  -1,   2));

        // After 30 steps:
        // pos=<x= 15, y= -6, z= -9>, vel=<x= -5, y=  4, z=  0>
        // pos=<x= -4, y=-11, z=  3>, vel=<x= -3, y=-10, z=  0>
        // pos=<x=  0, y= -1, z= 11>, vel=<x=  7, y=  4, z=  3>
        // pos=<x= -3, y= -2, z=  5>, vel=<x=  1, y=  2, z= -3>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3( 15,  -6,  -9));
        CHECK(p[1].position == Vector3( -4, -11,   3));
        CHECK(p[2].position == Vector3(  0,  -1,  11));
        CHECK(p[3].position == Vector3( -3,  -2,   5));

        CHECK(p[0].velocity == Vector3( -5,   4,   0));
        CHECK(p[1].velocity == Vector3( -3, -10,   0));
        CHECK(p[2].velocity == Vector3(  7,   4,   3));
        CHECK(p[3].velocity == Vector3(  1,   2,  -3));

        // After 40 steps:
        // pos=<x= 14, y=-12, z= -4>, vel=<x= 11, y=  3, z=  0>
        // pos=<x= -1, y= 18, z=  8>, vel=<x= -5, y=  2, z=  3>
        // pos=<x= -5, y=-14, z=  8>, vel=<x=  1, y= -2, z=  0>
        // pos=<x=  0, y=-12, z= -2>, vel=<x= -7, y= -3, z= -3>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3( 14, -12,  -4));
        CHECK(p[1].position == Vector3( -1,  18,   8));
        CHECK(p[2].position == Vector3( -5, -14,   8));
        CHECK(p[3].position == Vector3(  0, -12,  -2));

        CHECK(p[0].velocity == Vector3( 11,   3,   0));
        CHECK(p[1].velocity == Vector3( -5,   2,   3));
        CHECK(p[2].velocity == Vector3(  1,  -2,   0));
        CHECK(p[3].velocity == Vector3( -7,  -3,  -3));

        // After 50 steps:
        // pos=<x=-23, y=  4, z=  1>, vel=<x= -7, y= -1, z=  2>
        // pos=<x= 20, y=-31, z= 13>, vel=<x=  5, y=  3, z=  4>
        // pos=<x= -4, y=  6, z=  1>, vel=<x= -1, y=  1, z= -3>
        // pos=<x= 15, y=  1, z= -5>, vel=<x=  3, y= -3, z= -3>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3(-23,   4,   1));
        CHECK(p[1].position == Vector3( 20, -31,  13));
        CHECK(p[2].position == Vector3( -4,   6,   1));
        CHECK(p[3].position == Vector3( 15,   1,  -5));

        CHECK(p[0].velocity == Vector3( -7,  -1,   2));
        CHECK(p[1].velocity == Vector3(  5,   3,   4));
        CHECK(p[2].velocity == Vector3( -1,   1,  -3));
        CHECK(p[3].velocity == Vector3(  3,  -3,  -3));

        // After 60 steps:
        // pos=<x= 36, y=-10, z=  6>, vel=<x=  5, y=  0, z=  3>
        // pos=<x=-18, y= 10, z=  9>, vel=<x= -3, y= -7, z=  5>
        // pos=<x=  8, y=-12, z= -3>, vel=<x= -2, y=  1, z= -7>
        // pos=<x=-18, y= -8, z= -2>, vel=<x=  0, y=  6, z= -1>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3( 36, -10,   6));
        CHECK(p[1].position == Vector3(-18,  10,   9));
        CHECK(p[2].position == Vector3(  8, -12,  -3));
        CHECK(p[3].position == Vector3(-18,  -8,  -2));

        CHECK(p[0].velocity == Vector3(  5,   0,   3));
        CHECK(p[1].velocity == Vector3( -3,  -7,   5));
        CHECK(p[2].velocity == Vector3( -2,   1,  -7));
        CHECK(p[3].velocity == Vector3(  0,   6,  -1));

        // After 70 steps:
        // pos=<x=-33, y= -6, z=  5>, vel=<x= -5, y= -4, z=  7>
        // pos=<x= 13, y= -9, z=  2>, vel=<x= -2, y= 11, z=  3>
        // pos=<x= 11, y= -8, z=  2>, vel=<x=  8, y= -6, z= -7>
        // pos=<x= 17, y=  3, z=  1>, vel=<x= -1, y= -1, z= -3>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3(-33,  -6,   5));
        CHECK(p[1].position == Vector3( 13,  -9,   2));
        CHECK(p[2].position == Vector3( 11,  -8,   2));
        CHECK(p[3].position == Vector3( 17,   3,   1));

        CHECK(p[0].velocity == Vector3( -5,  -4,   7));
        CHECK(p[1].velocity == Vector3( -2,  11,   3));
        CHECK(p[2].velocity == Vector3(  8,  -6,  -7));
        CHECK(p[3].velocity == Vector3( -1,  -1,  -3));

        // After 80 steps:
        // pos=<x= 30, y= -8, z=  3>, vel=<x=  3, y=  3, z=  0>
        // pos=<x= -2, y= -4, z=  0>, vel=<x=  4, y=-13, z=  2>
        // pos=<x=-18, y= -7, z= 15>, vel=<x= -8, y=  2, z= -2>
        // pos=<x= -2, y= -1, z= -8>, vel=<x=  1, y=  8, z=  0>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3( 30,  -8,   3));
        CHECK(p[1].position == Vector3( -2,  -4,   0));
        CHECK(p[2].position == Vector3(-18,  -7,  15));
        CHECK(p[3].position == Vector3( -2,  -1,  -8));

        CHECK(p[0].velocity == Vector3(  3,   3,   0));
        CHECK(p[1].velocity == Vector3(  4, -13,   2));
        CHECK(p[2].velocity == Vector3( -8,   2,  -2));
        CHECK(p[3].velocity == Vector3(  1,   8,   0));

        // After 90 steps:
        // pos=<x=-25, y= -1, z=  4>, vel=<x=  1, y= -3, z=  4>
        // pos=<x=  2, y= -9, z=  0>, vel=<x= -3, y= 13, z= -1>
        // pos=<x= 32, y= -8, z= 14>, vel=<x=  5, y= -4, z=  6>
        // pos=<x= -1, y= -2, z= -8>, vel=<x= -3, y= -6, z= -9>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3(-25,  -1,   4));
        CHECK(p[1].position == Vector3(  2,  -9,   0));
        CHECK(p[2].position == Vector3( 32,  -8,  14));
        CHECK(p[3].position == Vector3( -1,  -2,  -8));

        CHECK(p[0].velocity == Vector3(  1,  -3,   4));
        CHECK(p[1].velocity == Vector3( -3,  13,  -1));
        CHECK(p[2].velocity == Vector3(  5,  -4,   6));
        CHECK(p[3].velocity == Vector3( -3,  -6,  -9));

        // After 100 steps:
        // pos=<x=  8, y=-12, z= -9>, vel=<x= -7, y=  3, z=  0>
        // pos=<x= 13, y= 16, z= -3>, vel=<x=  3, y=-11, z= -5>
        // pos=<x=-29, y=-11, z= -1>, vel=<x= -3, y=  7, z=  4>
        // pos=<x= 16, y=-13, z= 23>, vel=<x=  7, y=  1, z=  1>
        for (int i = 0; i < 10; ++i) { p = simulate(p); }
        CHECK(p[0].position == Vector3(  8, -12,  -9));
        CHECK(p[1].position == Vector3( 13,  16,  -3));
        CHECK(p[2].position == Vector3(-29, -11,  -1));
        CHECK(p[3].position == Vector3( 16, -13,  23));

        CHECK(p[0].velocity == Vector3( -7,   3,   0));
        CHECK(p[1].velocity == Vector3(  3, -11,  -5));
        CHECK(p[2].velocity == Vector3( -3,   7,   4));
        CHECK(p[3].velocity == Vector3(  7,   1,   1));

        // Energy after 100 steps:
        // pot:  8 + 12 +  9 = 29;   kin: 7 +  3 + 0 = 10;   total: 29 * 10 = 290
        // pot: 13 + 16 +  3 = 32;   kin: 3 + 11 + 5 = 19;   total: 32 * 19 = 608
        // pot: 29 + 11 +  1 = 41;   kin: 3 +  7 + 4 = 14;   total: 41 * 14 = 574
        // pot: 16 + 13 + 23 = 52;   kin: 7 +  1 + 1 =  9;   total: 52 *  9 = 468
        // Sum of total energy: 290 + 608 + 574 + 468 = 1940
        CHECK(totalEnergy(p) == 1940);
    }

    SECTION("Brute Force Simulation")
    {
        PlanetarySystem p = parseInput(sample_input);
        CHECK(findRepeatingState_brute_force(p) == 2772);
    }

    SECTION("Clever Simulation")
    {
        PlanetarySystem p = parseInput(sample_input);
        CHECK(findRepeatingState_clever(p) == 2772);
    }

    SECTION("Clever Simulation #2")
    {
        PlanetarySystem p = parseInput(sample_input2);
        CHECK(findRepeatingState_clever(p) == 4686774924);
    }
}
