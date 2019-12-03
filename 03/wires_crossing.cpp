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
    assert(!(l.start == l.end));
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

bool intersect(Line const& l, Coordinates const& p)
{
    auto const [minx, maxx] = std::minmax(l.start.x, l.end.x);
    auto const [miny, maxy] = std::minmax(l.start.y, l.end.y);
    return (p.x >= minx) && (p.x <= maxx) && (p.y >= miny) && (p.y <= maxy);
}

auto intersectionPoints(std::vector<Line> const& lines1, std::vector<Line> const& lines2)
{
    return ranges::to<std::vector<Coordinates>>(
        ranges::views::cartesian_product(lines1, lines2) |
        ranges::views::transform([](auto t) {
            auto const& [l1, l2] = t;
            return intersect(l1, l2);
            }) |
        ranges::views::drop(1) |
        ranges::views::filter([](auto opt) { return opt.has_value(); }) |
        ranges::views::transform([](auto opt) { return *opt; }));
}

std::tuple<Coordinates, int> closestIntersection(Wire const& w1, Wire const& w2)
{
    auto const manhattanDistance = [](Coordinates const& c) {
        return std::abs(c.x) + std::abs(c.y);
    };

    auto const intersection_points = intersectionPoints(w1.lines, w2.lines);
    Coordinates const ret = *ranges::min_element(intersection_points,
        [manhattanDistance](Coordinates const& c1, Coordinates const& c2) {
            return manhattanDistance(c1) < manhattanDistance(c2);
        });
    return std::make_tuple(ret, manhattanDistance(ret));
}

std::array<int, 2> walkIntersectionPoints(Field const& f)
{
    std::array<int, 2> ret{ std::numeric_limits<int>::max(), 0};
    auto const intersection_points = intersectionPoints(f.wires[0].lines, f.wires[1].lines);

    for (auto const& p : intersection_points) {
        int acc[] = { 0, 0 };
        for (int i = 0; i < 2; ++i) {
            auto const& w = f.wires[i];
            for (auto const& l : w.lines) {
                if (intersect(l, p)) {
                    if (isVertical(l)) {
                        acc[i] += std::abs(l.start.y - p.y);
                    } else {
                        acc[i] += std::abs(l.start.x - p.x);
                    }
                    break;
                }
                acc[i] += length(l);
            }
        }
        if (acc[0] + acc[1] < ret[0] + ret[1]) {
            ret[0] = acc[0];
            ret[1] = acc[1];
        }
    }
    return ret;
}

