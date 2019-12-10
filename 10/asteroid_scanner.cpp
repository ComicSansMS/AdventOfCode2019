#include <asteroid_scanner.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <ostream>

Vector2::Vector2()
    :x(0), y(0)
{}

Vector2::Vector2(int xx, int yy)
    :x(xx), y(yy)
{}

bool operator==(Vector2 const& lhs, Vector2 const& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::ostream& operator<<(std::ostream& os, Vector2 const& v) {
    return os << '[' << v.x << ',' << v.y << ']';
}

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

int dot(Vector2 const& v1, Vector2 const& v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}

std::vector<Target> determineTargets(Map& m, std::size_t origin)
{
    assert(origin < m.lines_of_sight.size());
    std::vector<Line> const& targets = m.lines_of_sight[origin];
    std::vector<Target> ret;
    ret.reserve(targets.size() - 1);
    Vector2 const reference(0, -1);
    float constexpr PI = 3.1415926535897932384626433832795f;
    for (std::size_t i = 0; i < targets.size(); ++i) {
        Line const& target_los = targets[i];
        if (target_los.distance == 0) { /* skip yourself */ continue; }
        Vector2 const& target_position = m.asteroid_positions[i];
        int const length_los_sq = dot(target_los.angle, target_los.angle);
        float angle = std::acos(
            static_cast<float>(dot(reference, target_los.angle)) / std::sqrt(static_cast<float>(length_los_sq)));
        if (target_los.angle.x < 0) { angle = (2.f * PI) - angle; }
        ret.push_back(Target{ i, target_position, target_los, angle });
    }
    std::sort(ret.begin(), ret.end(), [](Target const& t1, Target const& t2) { return t1.angle < t2.angle; });
    return ret;
}

std::vector<Vector2> vaporize(std::vector<Target> targets)
{
    std::vector<Vector2> ret;
    std::size_t constexpr INVALID_INDEX = std::numeric_limits<std::size_t>::max();
    ret.reserve(targets.size());
    while (!targets.empty()) {
        std::size_t beginning_of_range = 0;
        Vector2 current_angle_range = targets.front().line_of_sight.angle;
        for (std::size_t i = 0; i < targets.size(); ++i) {
            if (!(targets[i].line_of_sight.angle == current_angle_range)) {
                beginning_of_range = i;
                current_angle_range = targets[i].line_of_sight.angle;
            }
            bool is_obstructed = false;
            for (std::size_t j = beginning_of_range; j < targets.size(); ++j) {
                if (!(targets[j].line_of_sight.angle == targets[i].line_of_sight.angle)) { break; }
                if (targets[j].line_of_sight.distance < targets[i].line_of_sight.distance) {
                    is_obstructed = true;
                    break;
                }
            }
            if (is_obstructed) { continue; }
            ret.push_back(targets[i].position);
            // mark for zapping
            targets[i].index = INVALID_INDEX;
        }
        // zzzzzzzap
        targets.erase(
            std::remove_if(targets.begin(), targets.end(), [](Target const& t) {
                return t.index == INVALID_INDEX;
            }), targets.end());
    }
    return ret;
}
