#include <moon_simulator.hpp>

#include <algorithm>
#include <array>
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

int64_t findRepeatingState_brute_force(PlanetarySystem p)
{
    //std::unordered_set<PlanetarySystem> states;
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

namespace std {

template<> struct hash<PvPair>
{
    std::size_t operator()(PvPair const& pv) const noexcept
    {
        std::hash<int> hasher;
        std::size_t seed = hasher(pv.p);
        hash_combine(seed, pv.v);
        return seed;
    }
};

template<> struct hash<std::vector<PvPair>>
{
    std::size_t operator()(std::vector<PvPair> const& v) const noexcept
    {
        if (v.empty()) { return 0; }
        std::size_t seed = std::hash<PvPair>{}(v.front());
        for (auto it = v.begin() + 1; it != v.end(); ++it) { hash_combine(seed, *it); }
        return seed;
    }
};

}

int64_t findRepeatingState_clever(PlanetarySystem p)
{
    std::array<std::vector<PvPair>, 3> states_xyz;
    for (std::size_t i = 0; i < p.size(); ++i) {
        states_xyz[0].push_back(PvPair{ p[i].position.x, p[i].velocity.x });
        states_xyz[1].push_back(PvPair{ p[i].position.y, p[i].velocity.y });
        states_xyz[2].push_back(PvPair{ p[i].position.z, p[i].velocity.z });
    }
    std::array<int64_t, 3> steps_xyz{};
    auto check_state_dimension =
        [&steps_xyz, &states_xyz, planets = std::vector<PvPair>(p.size())]
        (int Vector3::* axis, PlanetarySystem const& p, int64_t steps) mutable -> bool
        {
            int const index = (axis == &Vector3::x) ? 0 : (axis == &Vector3::y ? 1 : 2);
            if (steps_xyz[index] == 0) {
                for (std::size_t i = 0; i < planets.size(); ++i) {
                    planets[i] = PvPair{ p[i].position.*axis, p[i].velocity.*axis };
                }
                if (states_xyz[index] == planets) {
                    steps_xyz[index] = steps;
                }
                return true;
            } else {
                return false;
            }
        };
    for (int64_t steps = 1; ; ++steps) {
        bool do_continue = false;
        p = simulate(p);
        do_continue |= check_state_dimension(&Vector3::x, p, steps);
        do_continue |= check_state_dimension(&Vector3::y, p, steps);
        do_continue |= check_state_dimension(&Vector3::z, p, steps);
        if (!do_continue) { break; }
    }

    int64_t ret = std::lcm(steps_xyz[0], std::lcm(steps_xyz[1], steps_xyz[2]));
    return ret;
}
