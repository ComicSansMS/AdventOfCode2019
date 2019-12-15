#ifndef ADVENT_OF_CODE_15_PATH_FINDER_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_15_PATH_FINDER_HPP_INCLUDE_GUARD

#include <integer_program_mk3.hpp>

#include <algorithm>
#include <cassert>
#include <deque>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

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

enum class Direction : Word {
    North = 1, 
    South = 2,
    West  = 3,
    East  = 4
};

enum class Tile {
    Empty = 0,
    Wall,
    Target
};

struct MockProgram {
    Vector2 position;
    std::unordered_map<Vector2, Tile> map;
    std::vector<Word> input;
    std::vector<Word> output;
    int pc = 0;
    int resume_point = 0;

    static MockProgram fromString(std::string_view map_string);
};

void executeProgram(MockProgram& m);

class Scanner {
public:
    std::variant<IntegerProgram, MockProgram> vp;
    Vector2 position;
public:
    Scanner(IntegerProgram p);
    Scanner(MockProgram m);

    void executeProgram();
    void resumeProgram();
    std::vector<Word>& input();
    std::vector<Word> const& input() const;
    std::vector<Word>& output();
    std::vector<Word> const& output() const;
    Address pc() const;
};

struct MapTile {
    Tile t;
    std::vector<Direction> path;
    MapTile();
};

struct Step {
    Vector2 initial_position;
    Direction step;

    Step(Vector2 p, Direction d);
    Vector2 getTargetPosition() const;
};

Direction reverseDirection(Direction d);

using MazeMap = std::unordered_map<Vector2, MapTile>;

std::vector<Direction> findPath(MazeMap const& map, Vector2 start, Vector2 destination);

void walkToKnownPosition(Scanner& s, MazeMap const& map, Vector2 destination);

Tile scanField(Scanner& s, Step next);

std::vector<Word> toInput(std::vector<Direction> const& v);

MazeMap floodFill(Scanner& s, std::ostream* os = nullptr);

std::ostream& operator<<(std::ostream& os, MazeMap const& m);

#endif
