#include <frequency_transmission.hpp>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/cycle.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/repeat_n.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip_with.hpp>
#include <range/v3/range/conversion.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <string>

#include <iostream>

static constexpr inline std::array<int, 4> basePattern = { 0, 1, 0, -1 };

Pattern generatePattern(int n_iteration)
{
    return ranges::to<Pattern>(ranges::views::for_each(basePattern, [n_iteration](int x) {
        return ranges::yield_from(ranges::views::repeat_n(x, n_iteration));
    }));
}

Signal multiplyPattern(Signal const& s, Pattern const& p)
{
    auto repeating_pattern = ranges::views::cycle(p) | ranges::views::drop(1);
    return ranges::to<Signal>(ranges::views::zip_with(std::multiplies{}, s, repeating_pattern));
}

Signal parseInput(std::string_view input)
{
    Signal ret;
    for (auto const& c : input) {
        if (c == '\n') { break; }
        assert((c >= '0') && (c <= '9'));
        ret.push_back(static_cast<int>(c - '0'));
    }
    return ret;
}

Signal applyPhase(Signal const& s)
{
    Signal ret(s.size());
    int const limit = static_cast<int>(s.size());
    for (int i = 0; i < limit; ++i)
    {
        int acc = 0;
        int const iteration = i + 1;
        int const span = 4 * iteration;
        // 1
        for (int j = iteration - 1; j < limit; j += span) {
            int const klimit = std::min(limit, j + iteration);
            for (int k = j; k < klimit; ++k) {
                acc += s[k];
            }
        }

        // -1
        for (int j = 3*iteration - 1; j < limit; j += span) {
            int const klimit = std::min(limit, j + iteration);
            for (int k = j; k < klimit; ++k) {
                acc -= s[k];
            }
        }
        ret[i] = std::abs(acc) % 10;
    }
    return ret;
}

Signal applyPhase_10k(Signal const& s, int skip)
{
    int const limit = 10'000 * static_cast<int>(s.size());
    Signal ret(limit - skip);
    for (int i = skip; i < limit; ++i)
    {
        int acc = 0;
        int const iteration = i + 1;
        int const span = 4 * iteration;
        // 1
        for (int j = iteration - 1; j < limit; j += span) {
            int const klimit = std::min(limit, j + iteration);
            for (int k = j; k < klimit; ++k) {
                acc += s[k % s.size()];
            }
        }

        // -1
        for (int j = 3*iteration - 1; j < limit; j += span) {
            int const klimit = std::min(limit, j + iteration);
            for (int k = j; k < klimit; ++k) {
                acc -= s[k % s.size()];
            }
        }
        ret[i - skip] = std::abs(acc) % 10;
    }
    return ret;
}


Signal calculateTransmission(Signal const& s, int n)
{
    Signal acc = s;
    for (int i = 0; i < n; ++i) {
        acc = applyPhase(acc);
    }
    return acc;
}

Signal calculateTransmission_10k(Signal const& s, int n)
{
    int skip = 0;
    int pow = 1;
    for (int i = 0; i < 7; ++i) {
        skip += (s[6 - i]) * pow;
        pow *= 10;
    }

    int const limit = 10'000 * static_cast<int>(s.size());
    Signal acc(limit - skip);
    for (int i = skip; i < limit; ++i) {
        acc[i - skip] = s[i % s.size()];
    }
    for (int i = 0; i < n; ++i) {
        for (int j = static_cast<int>(acc.size()) - 2; j >= 0; --j) {
            acc[j] += acc[j + 1];
            acc[j] = acc[j] % 10;
        }
    }
    return acc;
}
