#ifndef ADVENT_OF_CODE_08_IMAGE_DECODING_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_08_IMAGE_DECODING_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <string_view>
#include <vector>

struct Image {
    int width;
    int height;
    std::vector<std::vector<int>> layers;
};

struct Color {
    enum Color_Values {
        Black = 0,
        White = 1,
        Transparent = 2
    };
};

Image parseInput(std::string_view input, int width, int height);

int layerWithFewestZeros(Image const& img);

int imageChecksum(Image const& img);

Image collapseLayers(Image const& img);

std::ostream& operator<<(std::ostream& os, Image const& img);

#endif
