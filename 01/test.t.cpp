#include <test.hpp>

#include <catch.hpp>

TEST_CASE("Test")
{
    SECTION("Test1")
    {
        CHECK(test1("1") == 1);
    }

    SECTION("Test2")
    {
        CHECK(test1("2") == 2);
    }
}
