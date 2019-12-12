#ifndef ADVENT_OF_CODE_12_MOON_SIMULATOR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_12_MOON_SIMULATOR_HPP_INCLUDE_GUARD

#include <functional>
#include <iosfwd>
#include <string_view>
#include <unordered_set>
#include <vector>

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct Vector3 {
    int x;
    int y;
    int z;

    Vector3();
    Vector3(int xx, int yy, int zz);
};

Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs);
bool operator==(Vector3 const& lhs, Vector3 const& rhs);
std::ostream& operator<<(std::ostream& os, Vector3 const& v);

struct Moon {
    Vector3 position;
    Vector3 velocity;
};
bool operator==(Moon const& lhs, Moon const& rhs);

using PlanetarySystem = std::vector<Moon>;

namespace std
{
template<> struct hash<Vector3>
{
    std::size_t operator()(Vector3 const& v) const noexcept
    {
        std::size_t seed = std::hash<int>{}(v.x);
        hash_combine(seed, v.y);
        hash_combine(seed, v.z);
        return seed;
    }
};

template<> struct hash<Moon>
{
    std::size_t operator()(Moon const& m) const noexcept
    {
        std::size_t seed = std::hash<Vector3>{}(m.position);
        hash_combine(seed, m.velocity);
        return seed;
    }
};

template<> struct hash<PlanetarySystem>
{
    std::size_t operator()(PlanetarySystem const& p) const noexcept
    {
        if (p.empty()) { return 0; }
        std::size_t seed = std::hash<Moon>{}(p.front());
        for (auto it = p.begin() + 1; it != p.end(); ++it) { hash_combine(seed, *it); }
        return seed;
    }
};
}

PlanetarySystem parseInput(std::string_view input);

void applyGravity(Moon& m1, Moon& m2);

void applyVelocity(Moon& m1);

PlanetarySystem simulate(PlanetarySystem const& p);

int potentialEnergy(Moon const& m);
int kineticEnergy(Moon const& m);

int totalEnergy(PlanetarySystem const& p);

int64_t findRepeatingState_brute_force(PlanetarySystem p);

int64_t findRepeatingState_clever(PlanetarySystem p);

#endif
