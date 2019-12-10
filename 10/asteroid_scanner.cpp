#include <asteroid_scanner.hpp>

#include <algorithm>
#include <cassert>
#include <numeric>

Map parseInput(std::string_view input)
{
    Map ret;
    ret.width = ret.height = -1;
    int x = 0;
    int y = 0;
    for (auto& c : input) {
        if (c == '\n') {
            if (ret.width == -1) {
                ret.width = x;
            } else {
                assert(ret.width == x);
            }
            x = 0;
            ++y;
        } else {
            if (c == '#') {
                ret.asteroid_positions.push_back(Vector2(x, y));
            } else {
                assert(c == '.');
                // empty space
            }
            ++x;
        }
    }
    ret.height = y;
    return ret;
}

Line::Line(Vector2 const& origin, Vector2 const& target)
{
    Vector2 direction(target.x - origin.x, target.y - origin.y);
    distance = std::gcd(direction.x, direction.y);
    if (distance != 0) {
        angle = Vector2(direction.x / distance, direction.y / distance);
    } else {
        assert((direction.x == 0) && (direction.y == 0));
        angle = Vector2(0, 0);
    }
}

void determineLinesOfSight(Map& m)
{
    m.lines_of_sight.resize(m.asteroid_positions.size());
    for (auto& v : m.lines_of_sight) { v.reserve(m.asteroid_positions.size()); }
    for (std::size_t origin = 0; origin < m.asteroid_positions.size(); ++origin) {
        for (std::size_t target = 0; target < m.asteroid_positions.size(); ++target) {
            m.lines_of_sight[origin].push_back(Line(m.asteroid_positions[origin], m.asteroid_positions[target]));
        }
    }
}

int asteroidsVisibleFrom(Map& m, std::size_t origin_index)
{
    assert(origin_index < m.lines_of_sight.size());
    auto const& lines = m.lines_of_sight[origin_index];
    return static_cast<int>(std::count_if(lines.begin(), lines.end(), [&lines](Line const& l) {
        if (l.distance == 0) {
            // don't count yourself
            return false;
        }
        auto closer = std::find_if(lines.begin(), lines.end(),
            [&l](Line const& l2) { return (l.angle == l2.angle) && (l.distance < l2.distance); });
        if (closer != lines.end()) {
            // line of sight is blocked;
            return false;
        }
        return true;
    }));
}

std::tuple<int, std::size_t> bestVantagePoint(Map& m)
{
    int current_max = 0;
    std::size_t ret_index = m.asteroid_positions.size();
    for (std::size_t i = 0; i < m.asteroid_positions.size(); ++i) {
        int const visible = asteroidsVisibleFrom(m, i);
        if (visible > current_max) {
            current_max = visible;
            ret_index = i;
        }
    }
    return std::make_tuple(current_max, ret_index);
}
