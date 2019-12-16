#include <frequency_transmission.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>

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
    for (int i = 0; i<static_cast<int>(s.size()); ++i)
    {
        auto pattern = ranges::to<std::vector>(generatePattern(i + 1));
        ret[i] = std::abs(ranges::accumulate(multiplyPattern(s, pattern), 0)) % 10;
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
