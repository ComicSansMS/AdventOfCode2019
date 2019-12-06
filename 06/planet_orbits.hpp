#ifndef ADVENT_OF_CODE_06_PLANET_ORBITS_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_06_PLANET_ORBITS_HPP_INCLUDE_GUARD

#include <list>
#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>

using Planet = std::string;

struct Orbit {
    Planet center;
    Planet orbiter;
};

struct Node {
    Planet planet;
    Node* parent;
    std::vector<Node*> orbiters;
};

struct PlanetTree {
    std::list<Node> nodes;
    Node* root;
};

std::vector<Orbit> parseInput(std::string_view input);

PlanetTree buildPlanetTree(std::vector<Orbit> const& input);

int countAllOrbits(PlanetTree const& t);

Node const* findPlanet(PlanetTree const& t, Planet const& p);

std::vector<Node const*> pathToRoot(PlanetTree const& t, Planet const& p);

int commonElement(PlanetTree const& t, Planet const& p1, Planet const& p2);

#endif
