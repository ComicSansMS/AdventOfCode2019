#ifndef ADVENT_OF_CODE_16_FREQUENCY_TRANSMISSION_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_16_FREQUENCY_TRANSMISSION_HPP_INCLUDE_GUARD

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/repeat_n.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip_with.hpp>
#include <range/v3/range/conversion.hpp>

#include <array>
#include <functional>
#include <string_view>
#include <vector>

using Signal = std::vector<int>;

Signal parseInput(std::string_view input);

using Pattern = std::vector<int>;

static constexpr inline std::array<int, 4> basePattern = { 0, 1, 0, -1 };

template<typename BasePattern_T = decltype(basePattern)>
inline auto generatePattern(int n_iteration, BasePattern_T base_pattern = basePattern)
{
    return ranges::views::for_each(base_pattern, [n_iteration](int x) {
        return ranges::yield_from(ranges::views::repeat_n(x, n_iteration));
    });
}

template<typename Pattern_T>
inline auto multiplyPattern(Signal const& s, Pattern_T&& p)
{
    auto repeating_pattern = ranges::views::cycle(p) | ranges::views::drop(1);
    return ranges::views::zip_with(std::multiplies{}, s, repeating_pattern);
}

Signal applyPhase(Signal const& s);

Signal calculateTransmission(Signal const& s, int n);

#endif
