#include <image_decoding.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>

Image parseInput(std::string_view input, int width, int height)
{
    Image ret;
    ret.width = width;
    ret.height = height;
    std::vector<int> current_layer;
    assert(input.back() == '\n');
    input = input.substr(0, input.size() - 1);
    for (auto const& c : input) {
        if (current_layer.empty()) { current_layer.reserve(width * height); }
        assert((c >= '0') && (c <= '9'));
        current_layer.push_back(c - '0');
        if (current_layer.size() == width * height) {
            ret.layers.emplace_back(std::move(current_layer));
            current_layer.clear();
        }
    }
    assert(current_layer.empty());
    return ret;
}

int layerWithFewestZeros(Image const& img)
{
    std::vector<int> counted_zeros;
    counted_zeros.reserve(img.layers.size());
    std::transform(begin(img.layers), end(img.layers), std::back_inserter(counted_zeros),
        [](std::vector<int> const& layer) -> int {
            return static_cast<int>(std::count(begin(layer), end(layer), 0));
        });
    return static_cast<int>(std::distance(begin(counted_zeros),
                                          std::min_element(begin(counted_zeros), end(counted_zeros))));
}

int imageChecksum(Image const& img)
{
    int const index = layerWithFewestZeros(img);
    int n_ones = 0;
    int n_twos = 0;
    for (auto const& i : img.layers[index]) {
        if (i == 1) {
            ++n_ones;
        } else if (i == 2) {
            ++n_twos;
        }
    }
    return n_ones * n_twos;
}

Image collapseLayers(Image const& img)
{
    Image ret{ img.width, img.height, { std::vector<int>(img.width* img.height) } };
    for (int idx = 0; idx < img.width * img.height; ++idx) {
        ret.layers[0][idx] = std::accumulate(img.layers.rbegin(), img.layers.rend(), (int)Color::Transparent,
            [idx](int src_color, std::vector<int> blend_layer) -> int {
                int const dst_color = blend_layer[idx];
                return (dst_color == Color::Transparent) ? src_color : dst_color;
            });
    }
    return ret;
}

std::ostream& operator<<(std::ostream& os, Image const& img)
{
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            int const color = img.layers[0][y * img.width + x];
            if(color == Color::Black) {
                os << '.';
            } else if (color == Color::White) {
                os << '#';
            } else {
                os << ' ';
            }
        }
        os << '\n';
    }
    return os;
}
