#ifndef ADVENT_OF_CODE_17_VACUUM_ROBOT_INCLUDE_GUARD
#define ADVENT_OF_CODE_17_VACUUM_ROBOT_INCLUDE_GUARD

#include <intcode.hpp>

#include <string_view>
#include <iosfwd>

struct Vector2 {
    int x;
    int y;

    Vector2();
    Vector2(int xx, int yy);
};

bool operator==(Vector2 const& lhs, Vector2 const& rhs);
std::ostream& operator<<(std::ostream& os, Vector2 const& v);

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace std
{
template<> struct hash<Vector2>
{
    std::size_t operator()(Vector2 const& v) const noexcept
    {
        std::size_t seed = std::hash<int>{}(v.x);
        hash_combine(seed, v.y);
        return seed;
    }
};
}

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct Tile {
    bool isIntersection;
    Tile();
};

struct Map {
    int map_width;
    int map_height;
    Vector2 robot_position;
    Direction robot_direction;
    std::unordered_map<Vector2, Tile> map;
};

Map parseMap(std::vector<Word> const& intcode_output);

void markIntersections(Map& m);

int alignment1(Map const& m);

std::ostream& operator<<(std::ostream& os, Map const& m);

#endif
