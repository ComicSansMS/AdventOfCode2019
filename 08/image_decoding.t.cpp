#include <image_decoding.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Image Decoding")
{
    char const sample_input[] = "123456789012\n";
    SECTION("Parse Input")
    {
        Image const img = parseInput(sample_input, 3, 2);
        CHECK(img.width == 3);
        CHECK(img.height == 2);
        REQUIRE(img.layers.size() == 2);
        CHECK(img.layers[0] == std::vector<int>{1, 2, 3, 4, 5, 6});
        CHECK(img.layers[1] == std::vector<int>{7, 8, 9, 0, 1, 2});
    }

    SECTION("Determine Layer With Fewest 0s")
    {
        Image const img = parseInput(sample_input, 3, 2);
        CHECK(layerWithFewestZeros(img) == 0);
        CHECK(layerWithFewestZeros(parseInput("1020022200\n", 1, 2)) == 3);
    }

    SECTION("Image Checksum")
    {
        Image const img = parseInput(sample_input, 3, 2);
        CHECK(imageChecksum(img) == 1);
        CHECK(imageChecksum(parseInput("010012221022\n", 2, 2)) == 3);
    }

    SECTION("Collapse Image Layers")
    {
        Image const img = parseInput("0222112222120000\n", 2, 2);
        Image const collapsed = collapseLayers(img);
        CHECK(collapsed.width == img.width);
        CHECK(collapsed.height == img.height);
        REQUIRE(collapsed.layers.size() == 1);
        CHECK(collapsed.layers[0] == std::vector<int>{0, 1, 1, 0});
    }

    SECTION("Image Output")
    {
        std::stringstream sstr;
        sstr << Image{ 2,2, {std::vector<int>{0,1,2,1}} };
        CHECK(sstr.str() == ".#\n #\n");
    }
}
