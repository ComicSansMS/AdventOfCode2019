#include <intcode_arcade.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Intcode Arcade")
{
    SECTION("Count Tiles")
    {
        CHECK(countTiles(std::vector<Word>{1, 2, 3, 6, 5, 4}, Tile::Block) == 0);
        CHECK(countTiles(std::vector<Word>{1, 2, 3, 6, 5, 4}, Tile::Ball) == 1);
        CHECK(countTiles(std::vector<Word>{1, 2, 3, 6, 5, 4}, Tile::HorizontalPaddle) == 1);
    }

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
}
