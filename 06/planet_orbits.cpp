#include <planet_orbits.hpp>

#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/range/conversion.hpp>

#include <algorithm>
#include <cassert>
#include <limits>
#include <numeric>
#include <string>
#include <unordered_set>


std::vector<Orbit> parseInput(std::string_view input)
{
    std::vector<Orbit> ret;
    for (auto const& line : input | ranges::views::split('\n')) {
        auto const r = line | ranges::views::split(')');
        std::string const center = ranges::to<std::string>(*ranges::begin(r));
        std::string const orbiter = ranges::to<std::string>(*ranges::next(ranges::begin(r)));
        ret.push_back(Orbit{ center, orbiter });
    }
    return ret;
}

PlanetTree buildPlanetTree(std::vector<Orbit> const& input)
{
    PlanetTree ret;
    std::unordered_map<Planet, Node*> node_list;
    auto get_or_insert_node = [&ret, &node_list](Planet const& p) {
        auto it_node = node_list.find(p);
        if (it_node != end(node_list)) { return it_node; }
        ret.nodes.push_back(Node{ p, nullptr, {} });
        return node_list.insert(std::make_pair(p, &ret.nodes.back())).first;
    };
    for (auto const& o : input) {
        auto it_orbiter = get_or_insert_node(o.orbiter);
        auto it_center = get_or_insert_node(o.center);
        it_center->second->orbiters.push_back(it_orbiter->second);
        assert(!it_orbiter->second->parent);
        it_orbiter->second->parent = it_center->second;
    }
    // get root
    Node* root_candidate = &ret.nodes.front();
    while (root_candidate->parent) { root_candidate = root_candidate->parent; }
    ret.root = root_candidate;
    return ret;
}

int countAllOrbits(PlanetTree const& t)
{
    int acc = 0;
    for (auto const& node : t.nodes) {
        Node const* it = &node;
        while (it->parent) { it = it->parent; ++acc; }
    }
    return acc;
}

Node const* findPlanet(PlanetTree const& t, Planet const& p)
{
    return &(*ranges::find_if(t.nodes, [&p](Node const& n) { return n.planet == p; }));
}

std::vector<Node const*> pathToRoot(PlanetTree const& t, Planet const& p)
{
    Node const* n = findPlanet(t, p);
    std::vector<Node const*> ret;
    while (n->parent) { n = n->parent; ret.push_back(n); }
    return ret;
}

int commonElement(PlanetTree const& t, Planet const& p1, Planet const& p2)
{
    auto path1 = pathToRoot(t, p1);
    auto path2 = pathToRoot(t, p2);
    auto [it_start1, it_start2] = std::mismatch(rbegin(path1), rend(path1), rbegin(path2), rend(path2));
    assert((*it_start1)->parent == (*it_start2)->parent);
    auto const d1 = std::distance(begin(path1), it_start1.base());
    auto const d2 = std::distance(begin(path2), it_start2.base());
    return static_cast<int>(d1 + d2);
}
