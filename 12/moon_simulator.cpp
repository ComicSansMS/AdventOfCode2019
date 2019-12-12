#include <moon_simulator.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <ostream>
#include <regex>

Vector3::Vector3()
    :x(0), y(0), z(0)
{
}

Vector3::Vector3(int xx, int yy, int zz)
    :x(xx), y(yy), z(zz)
{
}

Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs)
{
    return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

bool operator==(Vector3 const& lhs, Vector3 const& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

std::ostream& operator<<(std::ostream& os, Vector3 const& v)
{
    return os << '[' << v.x << ',' << v.y << ',' << v.z << ']';
}

PlanetarySystem parseInput(std::string_view input)
{
    PlanetarySystem ret;
    std::regex rx_line(R"(<x=(-?[\d]+), y=(-?[\d]+), z=(-?[\d]+)>)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> Moon {
            return Moon{Vector3(std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3])), Vector3() };
        });
    return ret;
}

void applyGravity(Moon& m1, Moon& m2)
{
    auto applyToAxis = [](int Vector3::* axis, Moon& m1, Moon& m2) {
        auto const& p1 = m1.position.*axis;
        auto const& p2 = m2.position.*axis;
        auto& v1 = m1.velocity.*axis;
        auto& v2 = m2.velocity.*axis;
        if (p1 < p2) {
            ++v1;
            --v2;
        } else if (p1 > p2) {
            --v1;
            ++v2;
        } else {
            assert(p1 == p2);
            // no change
        }
    };

    applyToAxis(&Vector3::x, m1, m2);
    applyToAxis(&Vector3::y, m1, m2);
    applyToAxis(&Vector3::z, m1, m2);
}

void applyVelocity(Moon& m1)
{
    m1.position = m1.position + m1.velocity;
}

PlanetarySystem simulate(PlanetarySystem const& p)
{
    PlanetarySystem ret = p;
    for (std::size_t i = 0; i < p.size(); ++i) {
        for (std::size_t j = i + 1; j < p.size(); ++j) {
            applyGravity(ret[i], ret[j]);
        }
    }
    for (Moon& m : ret) { applyVelocity(m); }
    return ret;
}

int potentialEnergy(Moon const& m)
{
    return std::abs(m.position.x) + std::abs(m.position.y) + std::abs(m.position.z);
}

int kineticEnergy(Moon const& m)
{
    return std::abs(m.velocity.x) + std::abs(m.velocity.y) + std::abs(m.velocity.z);
}

int totalEnergy(PlanetarySystem const& p)
{
    return std::accumulate(p.begin(), p.end(), 0,
        [](int acc, Moon const& m) -> int { return acc + (potentialEnergy(m) * kineticEnergy(m)); });
}
