/*

*/

#include <wires_crossing.hpp>

#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/algorithm/min_element.hpp>

#include <algorithm>
#include <cassert>
#include <limits>
#include <ostream>
#include <string>

Field parseInput(std::string_view input)
{
    Field f;
    int i = 0;
    for (auto const& line : input | ranges::views::split('\n')) {
        Wire& wire = f.wires[i];
        for (auto const& token : line | ranges::views::split(',')) {
            std::string tt = ranges::to<std::string>(token);
            wire.path.emplace_back();
            PathSegment& l = wire.path.back();
            switch (tt[0]) {
            case 'U': l.direction = Direction::Up; break;
            case 'D': l.direction = Direction::Down; break;
            case 'L': l.direction = Direction::Left; break;
            case 'R': l.direction = Direction::Right; break;
            default: assert(false); break;
            }
            l.length = std::stoi(tt.substr(1));
        }
        ++i;
    }
    return f;
}

void layoutWires(Field& f)
{
    for (int wi : ranges::views::iota(0, static_cast<int>(f.wires.size()))) {
        Wire& w = f.wires[wi];
        w.lines.reserve(w.path.size());
        Coordinates start{ 0, 0 };
        for (auto const& ps : w.path) {
            Coordinates end = start;
            switch (ps.direction) {
            case Direction::Up:    end.y += ps.length; break;
            case Direction::Down:  end.y -= ps.length; break;
            case Direction::Left:  end.x -= ps.length; break;
            case Direction::Right: end.x += ps.length; break;;
            }
            w.lines.push_back(Line{ start, end });
            start = end;
        }
    }
}

bool isVertical(Line const& l)
{
    return l.start.x == l.end.x;
}

int length(Line const& l)
{
    return isVertical(l) ? std::abs(l.start.y - l.end.y) : std::abs(l.start.x - l.end.x);
}

std::optional<Coordinates> intersect(Line const& l1, Line const& l2)
{
    bool const v1 = isVertical(l1);
    bool const v2 = isVertical(l2);
    if (v1 != v2) {
        Line const& lv = (v1) ? l1 : l2;
        Line const& lh = (v1) ? l2 : l1;
        int lhx = std::min(lh.start.x, lh.end.x);
        int dx = lv.start.x - lhx;
        if ((dx < 0) || (dx > length(lh))) { return std::nullopt; }
        int lvy = std::min(lv.start.y, lv.end.y);
        int dy = lh.start.y - lvy;
        if ((dy < 0) || (dy > length(lv))) { return std::nullopt; }
        return Coordinates{ lv.start.x, lh.start.y };
    }
    return std::nullopt;
}

std::tuple<Coordinates, int> closestIntersection(Wire const& w1, Wire const& w2)
{
    auto const manhattanDistance = [](Coordinates const& c) {
        return std::abs(c.x) + std::abs(c.y);
    };

    auto intersection_points =
        ranges::views::cartesian_product(w1.lines, w2.lines) |
        ranges::views::transform([](auto t) {
            auto const& [l1, l2] = t;
            return intersect(l1, l2);
            }) |
        ranges::views::drop(1) |
        ranges::views::filter([](auto opt) { return opt.has_value(); });
    auto v = ranges::to<std::vector<std::optional<Coordinates>>>(intersection_points);
    Coordinates const ret = **ranges::min_element(intersection_points,
        [manhattanDistance](std::optional<Coordinates> const& c1, std::optional<Coordinates> const& c2) {
            return manhattanDistance(*c1) < manhattanDistance(*c2);
        });
    return std::make_tuple(ret, manhattanDistance(ret));
}

