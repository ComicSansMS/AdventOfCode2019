#include <fuel_refinement.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>
#include <string>

bool operator==(Component const& lhs, Component const& rhs)
{
    return (lhs.quantity == rhs.quantity) && (lhs.name == rhs.name);
}

std::ostream& operator<<(std::ostream& os, Component const& rhs)
{
    return os << rhs.quantity << ' ' << rhs.name;
}

Factory parseInput(std::string_view input)
{
    Factory ret;
    std::stringstream sstr{ std::string(input) };
    std::string line;
    auto parseComponent = [](std::string const& str) -> Component {
        auto const separator = str.find(' ');
        Component ret;
        auto s2 = str.substr(0, separator);
        ret.quantity = std::stoi(str.substr(0, separator));
        ret.name = str.substr(separator + 1);
        return ret;
    };
    while (std::getline(sstr, line)) {
        Reaction r;
        char const separation_token[] = " => ";
        auto separator = line.find(separation_token);
        r.result = parseComponent(line.substr(separator + (sizeof(separation_token) - 1)));
        std::string const str_ingredients = line.substr(0, separator);
        auto it_start = str_ingredients.begin();
        auto it_separator = std::find(it_start, str_ingredients.end(), ',');
        while (it_start != str_ingredients.end()) {
            r.ingredients.push_back(parseComponent(std::string(it_start, it_separator)));
            it_start = (it_separator == str_ingredients.end()) ? it_separator : it_separator + 2;
            it_separator = std::find(it_start, str_ingredients.end(), ',');
        }
        ret.push_back(r);
    }
    return ret;
}

ReactionMap buildReactionMap(Factory const& f)
{
    ReactionMap reactions_map;
    reactions_map.reserve(f.size());
    for (auto const& r : f) {
        assert(reactions_map.find(r.result.name) == reactions_map.end());
        reactions_map[r.result.name] = &r;
    }
    return reactions_map;
}

int64_t oreRequired(ReactionMap const& m, Reaction const* r, int64_t components_required,
    std::unordered_map<std::string, int64_t>& global_reservoir, std::unordered_map<std::string, int64_t>& totals)
{
    // first check the global reservoir for components already produced earlier
    int64_t& result_reservoir = global_reservoir[r->result.name];
    assert(result_reservoir >= 0);
    if (result_reservoir > 0) {
        int64_t const components_taken = std::min(result_reservoir, components_required);
        components_required -= components_taken;
        result_reservoir -= components_taken;
        if (components_required == 0) { return 0; }
    }
    // build the rest up from reactions
    int64_t acc = 0;
    int64_t n_reactions = 1 + ((components_required - 1) / r->result.quantity);
    totals[r->result.name] += n_reactions * r->result.quantity;
    for (auto const& i :r->ingredients) {
        if (i.name == "ORE") {
            acc += i.quantity * n_reactions;
        } else {
            auto it = m.find(i.name);
            assert(it != m.end());
            acc += oreRequired(m, it->second, i.quantity * n_reactions, global_reservoir, totals);
        }
    }
    // return leftover productions to the reservoir
    int64_t const components_produced = r->result.quantity * n_reactions;
    int64_t const components_leftover = components_produced - components_required;
    global_reservoir[r->result.name] += components_leftover;
    return acc;
}

int64_t oreForOneFuel(ReactionMap const& m)
{
    auto const it = m.find("FUEL");
    assert(it != m.end());
    std::unordered_map<std::string, int64_t> global_reservoir;
    global_reservoir.reserve(m.size());
    std::unordered_map<std::string, int64_t> totals;
    int64_t const res = oreRequired(m, it->second, 1, global_reservoir, totals);
    return res;
}

int64_t fuelFromOneTrillion(ReactionMap const& m)
{
    auto const it = m.find("FUEL");
    assert(it != m.end());
    std::unordered_map<std::string, int64_t> global_reservoir;
    global_reservoir.reserve(m.size());
    std::unordered_map<std::string, int64_t> totals;

    int64_t const initial_ore = 1'000'000'000'000;
    int64_t ore_source = initial_ore;
    int64_t fuel_count = 0;
    for (;;) {
        int64_t const res = oreRequired(m, it->second, 1, global_reservoir, totals);
        ore_source -= res;
        if (ore_source < 0) { break; }
        ++fuel_count;
        if (std::all_of(global_reservoir.begin(), global_reservoir.end(), [](auto const& p) { return p.second == 0; }))
        {
            // shortcut: subtract the full cycles that we can obtain from the current ore left in source
            int64_t const ore_consumed_so_far = initial_ore - ore_source;
            int64_t const full_cycles = ore_source / ore_consumed_so_far;
            fuel_count += (full_cycles * fuel_count);
            ore_source -= full_cycles * ore_consumed_so_far;
        }
    }
    return fuel_count;
}
