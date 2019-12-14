#ifndef ADVENT_OF_CODE_14_FUEL_REFINEMENT_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_FUEL_REFINEMENT_HPP_INCLUDE_GUARD

#include <iosfwd>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Component {
    int quantity;
    std::string name;
};
bool operator==(Component const& lhs, Component const& rhs);
std::ostream& operator<<(std::ostream& os, Component const& rhs);

struct Reaction
{
    Component result;
    std::vector<Component> ingredients;
};

using Factory = std::vector<Reaction>;

Factory parseInput(std::string_view input);

using ReactionMap = std::unordered_map<std::string, Reaction const*>;
ReactionMap buildReactionMap(Factory const& f);

int64_t oreForOneFuel(ReactionMap const& m);

int64_t fuelFromOneTrillion(ReactionMap const& m);

#endif
