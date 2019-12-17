#include <vacuum_robot.hpp>

#include <algorithm>
#include <cassert>
#include <ostream>
#include <numeric>

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

Tile::Tile()
    :isIntersection(false)
{}

Map parseMap(std::vector<Word> const& intcode_output)
{
    Map ret;
    ret.map_width = -1;
    int current_x = 0;
    int current_y = 0;
    for (auto const& w : intcode_output) {
        if (w == '#') {
            ret.map.emplace(Vector2(current_x, current_y), Tile{});
        } else if (w == '^') {
            ret.map.emplace(Vector2(current_x, current_y), Tile{});
            ret.robot_position = Vector2(current_x, current_y);
            ret.robot_direction = Direction::Up;
        } else if (w == 'v') {
            ret.map.emplace(Vector2(current_x, current_y), Tile{});
            ret.robot_position = Vector2(current_x, current_y);
            ret.robot_direction = Direction::Down;
        } else if (w == '<') {
            ret.map.emplace(Vector2(current_x, current_y), Tile{});
            ret.robot_position = Vector2(current_x, current_y);
            ret.robot_direction = Direction::Left;
        } else if (w == '>') {
            ret.map.emplace(Vector2(current_x, current_y), Tile{});
            ret.robot_position = Vector2(current_x, current_y);
            ret.robot_direction = Direction::Right;
        } else if (w == '\n') {
            if (ret.map_width == -1) { ret.map_width = current_x; }
            if (current_x == 0) { assert(&w == &intcode_output.back()); continue; }
            assert(ret.map_width == current_x);
            current_x = 0;
            ++current_y;
            continue;
        } else {
            assert(w == '.');
            // do nothing
        }
        ++current_x;
    }
    ret.map_height = current_y;
    return ret;
}

void markIntersections(Map& m)
{
    for (auto& [position, tile] : m.map) {
        auto const it_up = m.map.find(Vector2(position.x, position.y - 1));
        auto const it_down = m.map.find(Vector2(position.x, position.y + 1));
        auto const it_left = m.map.find(Vector2(position.x - 1, position.y));
        auto const it_right = m.map.find(Vector2(position.x + 1, position.y));
        if ((it_up != m.map.end()) && (it_down != m.map.end()) &&
            (it_left != m.map.end()) && (it_right != m.map.end()))
        {
            tile.isIntersection = true;
        }
    }
}

int alignment1(Map const& m)
{
    return std::accumulate(m.map.begin(), m.map.end(), 0, [](int acc, auto const& p) {
        auto const [position, tile] = p;
        return acc + (tile.isIntersection ? (position.x * position.y) : 0);
    });
}


std::ostream& operator<<(std::ostream& os, Map const& m)
{
    for (int iy = 0; iy < m.map_height; ++iy) {
        for (int ix = 0; ix < m.map_width; ++ix) {
            if (Vector2(ix, iy) == m.robot_position) {
                if (m.robot_direction == Direction::Up) {
                    os << '^';
                } else if (m.robot_direction == Direction::Down) {
                    os << 'v';
                } else if (m.robot_direction == Direction::Left) {
                    os << '<';
                } else {
                    assert(m.robot_direction == Direction::Right);
                    os << '>';
                }
            } else {
                auto const it = m.map.find(Vector2(ix, iy));
                if (it != m.map.end()) {
                    if (it->second.isIntersection) {
                        os << 'O';
                    } else {
                        os << '#';
                    }
                } else {
                    os << '.';
                }
            }
        }
        os << '\n';
    }
    return os;
}
