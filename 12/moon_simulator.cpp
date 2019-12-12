#include <moon_simulator.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <ostream>
#include <regex>
#include <tuple>

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

bool operator==(Moon const& lhs, Moon const& rhs)
{
    return (lhs.position == rhs.position) && (lhs.velocity == rhs.velocity);
}

PlanetarySystem parseInput(std::string_view input)
{
    PlanetarySystem ret;
    std::regex rx_line(R"(<x=(-?[\d]+), y=(-?[\d]+), z=(-?[\d]+)>)");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();
    std::transform(it_begin, it_end, ret.begin(),
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

int64_t findRepeatingState_brute_force(PlanetarySystem p)
{
    PlanetarySystem const original = p;
    p = simulate(p);
    int64_t steps = 1;
    while (p != original) {
        p = simulate(p);
        ++steps;
    }
    return steps;
}

struct PvPair {
    int p;
    int v;
};
bool operator==(PvPair const& lhs, PvPair const& rhs) {
    return (lhs.p == rhs.p) && (lhs.v == rhs.v);
}

int64_t findRepeatingState_clever(PlanetarySystem p)
{
    std::array<std::array<PvPair, 4>, 3> states_xyz;
    auto getPvForAxis = [](int Vector3::* axis, PlanetarySystem const& p) -> std::array<PvPair, 4> {
        std::array<PvPair, 4> ret;
        for (std::size_t planet_index = 0; planet_index < ret.size(); ++planet_index) {
            ret[planet_index] = PvPair{ p[planet_index].position.*axis, p[planet_index].velocity.*axis };
        }
        return ret;
    };
    states_xyz[0] = getPvForAxis(&Vector3::x, p);
    states_xyz[1] = getPvForAxis(&Vector3::y, p);
    states_xyz[2] = getPvForAxis(&Vector3::z, p);

    auto check_for_circle_in_dimension =
        [&states_xyz, getPvForAxis] (int Vector3::* axis, PlanetarySystem const& p, int64_t steps) -> int64_t
        {
            int const index = (axis == &Vector3::x) ? 0 : (axis == &Vector3::y ? 1 : 2);
            std::array<PvPair, 4> const planets = getPvForAxis(axis, p);
            if (states_xyz[index] == planets) { return steps; }
            return 0;
        };

    std::array<int64_t, 3> steps_xyz{};
    for (int64_t steps = 1; std::count(begin(steps_xyz), end(steps_xyz), 0) != 0; ++steps) {
        p = simulate(p);
        steps_xyz[0] = (steps_xyz[0] != 0) ? steps_xyz[0] : check_for_circle_in_dimension(&Vector3::x, p, steps);
        steps_xyz[1] = (steps_xyz[1] != 0) ? steps_xyz[1] : check_for_circle_in_dimension(&Vector3::y, p, steps);
        steps_xyz[2] = (steps_xyz[2] != 0) ? steps_xyz[2] : check_for_circle_in_dimension(&Vector3::z, p, steps);
    }

    int64_t ret = std::lcm(steps_xyz[0], std::lcm(steps_xyz[1], steps_xyz[2]));
    return ret;
}
