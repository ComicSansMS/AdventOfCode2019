#include <rocket_equation.hpp>

#include <catch.hpp>

#include <vector>

TEST_CASE("Rocket Equation")
{
    SECTION("Parse Input")
    {
        CHECK(parseInput("1\n2\n42") == std::vector<int>{1, 2, 42});
    }

    SECTION("Fuel Formula")
    {
        CHECK(rocket_equation(2) == -2);
        CHECK(rocket_equation(12) == 2);
        CHECK(rocket_equation(14) == 2);
        CHECK(rocket_equation(1969) == 654);
        CHECK(rocket_equation(100756) == 33583);
    }

    SECTION("Sum of Fuels")
    {
        std::vector<int> masses{12, 14, 1969, 100756};
        CHECK(total_fuel(masses) == 2 + 2 + 654 + 33583);
    }

    SECTION("Fuel Formula Considering Fuel Mass")
    {
        CHECK(rocket_equation_with_fuel(12) == 2);
        CHECK(rocket_equation_with_fuel(14) == 2);
        CHECK(rocket_equation_with_fuel(1969) == 966);
        CHECK(rocket_equation_with_fuel(100756) == 50346);
    }

    SECTION("Sum of Fuels Considering Fuel Mass")
    {
        std::vector<int> masses{ 12, 14, 1969, 100756 };
        CHECK(total_fuel_with_fuel(masses) == 2 + 2 + 966 + 50346);
    }
}
