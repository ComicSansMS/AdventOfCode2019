#include <planet_orbits.hpp>

#include <catch.hpp>

TEST_CASE("Planet Orbits")
{
    char const galaxy[] = "COM)B\nB)C\nC)D\nD)E\nE)F\nB)G\nG)H\nD)I\nE)J\nJ)K\nK)L";
    char const galaxy2[] = "COM)B\nB)C\nC)D\nD)E\nE)F\nB)G\nG)H\nD)I\nE)J\nJ)K\nK)L\nK)YOU\nI)SAN";

    SECTION("Parse Input")
    {
        auto const orbits = parseInput("AAA)BBB\nC)D");
        REQUIRE(orbits.size() == 2);
        CHECK(orbits[0].center == "AAA");
        CHECK(orbits[0].orbiter == "BBB");
        CHECK(orbits[1].center == "C");
        CHECK(orbits[1].orbiter == "D");
    }

    SECTION("Build tree")
    {
        /*
        *        G - H       J - K - L
        *        /           /
        * COM - B - C - D - E - F
        *                \
        *                 I
        */
        auto const orbits = parseInput(galaxy);
        auto t = buildPlanetTree(orbits);
        CHECK(t.root->planet == "COM");

        CHECK(countAllOrbits(t) == 42);
    }

    SECTION("Common Element")
    {
        /*
        *                           YOU
        *                          /
        *         G - H       J - K - L
        *        /           /
        * COM - B - C - D - E - F
        *                \
        *                 I - SAN
        */
        auto const orbits = parseInput(galaxy2);
        auto t = buildPlanetTree(orbits);
        CHECK(commonElement(t, "YOU", "SAN") == 4);
    }
}
