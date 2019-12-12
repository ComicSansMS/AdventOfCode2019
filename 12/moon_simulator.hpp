#ifndef ADVENT_OF_CODE_12_MOON_SIMULATOR_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_12_MOON_SIMULATOR_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <string_view>
#include <vector>

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

using PlanetarySystem = std::vector<Moon>;

PlanetarySystem parseInput(std::string_view input);

void applyGravity(Moon& m1, Moon& m2);

void applyVelocity(Moon& m1);

PlanetarySystem simulate(PlanetarySystem const& p);

int potentialEnergy(Moon const& m);
int kineticEnergy(Moon const& m);

int totalEnergy(PlanetarySystem const& p);

#endif
